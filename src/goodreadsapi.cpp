/*
The MIT License (MIT)

Copyright (c) 2018-2019 Oleg Linkin <maledictusdemagog@gmail.com>

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/

#include "goodreadsapi.h"

#include <QNetworkAccessManager>
#include <QNetworkDiskCache>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QRegularExpression>
#include <QUrlQuery>
#include <QXmlQuery>

#include "application.h"
#include "objects/book.h"
#include "objects/group.h"
#include "objects/review.h"
#include "objects/series.h"
#include "oauth1.h"
#include "rpcutils.h"

namespace Sailreads
{
const QString GoodReadsApi::UnreadTopicsFolderId = QString("unread");

GoodReadsApi::GoodReadsApi(QObject *parent)
: QObject(parent)
, m_ConsumerKey("YzXuHShjEIJ9VR0viEw7MQ")
, m_ConsumerSecretKey("q9T9p1EMdQD1ldgMlNF6dVF3S3hYikBWcj5O2yeGHA")
, m_BaseUrl("https://www.goodreads.com")
, m_NAM(new QNetworkAccessManager(this))
, m_OAuth1(new OAuth1(m_ConsumerKey, m_ConsumerSecretKey, m_NAM, this))
{
    QNetworkDiskCache *cache = new QNetworkDiskCache();

    cache->setCacheDirectory(Application::GetPath(Application::ApplicationDirectory::CacheDirectory));
    m_NAM->setCache(cache);
}

GoodReadsApi::~GoodReadsApi()
{
    ClearCache();
}

void GoodReadsApi::ClearCache()
{
    m_NAM->cache()->clear();
}

void GoodReadsApi::AbortRequest(QObject *object)
{
    if (m_Requester2Reply.contains(object) && m_Requester2Reply[object]) {
        m_Requester2Reply[object]->abort();
        m_Requester2Reply.remove(object);
    }
}

void GoodReadsApi::RequestRedirectedUrl(const QUrl& url,
        const std::function<void(QObject*)>& func)
{
    QNetworkReply *reply = m_NAM->get(QNetworkRequest(url));
    connect(reply, &QNetworkReply::finished, [reply, func](){ func(reply); });
}

void GoodReadsApi::UpdateCredentials(const QString& accessToken, const QString& accessTokenSecret)
{
    m_AccessToken = accessToken;
    m_AccessTokenSecret = accessTokenSecret;
}

void GoodReadsApi::ObtainRequestToken() const
{
    QNetworkReply *reply = m_OAuth1->RequestTemporaryCredentials(QNetworkAccessManager::GetOperation,
            QUrl(m_BaseUrl + "/oauth/request_token"));
    connect(reply, &QNetworkReply::finished,
            this, &GoodReadsApi::handleObtainRequestToken);
}

void GoodReadsApi::RequestAccessToken() const
{
    QNetworkReply *reply = m_OAuth1->RequestTokenCredentials(QNetworkAccessManager::GetOperation,
            QUrl(m_BaseUrl + "/oauth/access_token"), m_RequestToken, m_RequestTokenSecret);
    connect(reply, &QNetworkReply::finished,
            this, &GoodReadsApi::handleRequestAccessToken);
}

void GoodReadsApi::AuthUser()
{
    QNetworkReply *reply = m_OAuth1->Get(m_AccessToken, m_AccessTokenSecret,
            QUrl(m_BaseUrl + "/api/auth_user"));
    connect(reply, &QNetworkReply::finished,
             this, &GoodReadsApi::handleAuthUser);
}

void GoodReadsApi::GetUserInfo(QObject *requester, const QString& id, bool useCache)
{
    QNetworkReply *reply = m_OAuth1->Get(m_AccessToken, m_AccessTokenSecret,
            QUrl(m_BaseUrl + QString("/user/show/%1").arg(id)), { { "format", "xml" } }, useCache);
    m_Requester2Reply[requester] = reply;
    connect(reply, &QNetworkReply::finished,
             this, &GoodReadsApi::handleGetUserInfo);
}

void GoodReadsApi::CompareBooks(const QString&)
{
//    //TODO make simple request
//    const auto& pair = m_OAuthWrapper->MakeSignedUrl(m_AccessToken, m_AccessTokenSecret,
//            QUrl(QString("https://www.goodreads.com/user/compare/%1.xml").arg(userId)), "GET");
//    auto reply = m_NAM->get(QNetworkRequest(pair.first));
//    connect(reply, &QNetworkReply::finished,
//             this, &GoodReadsApi::handleCompareBooks);
}

void GoodReadsApi::GetUpdates(QObject *requester, const QString& scope, const QString& items,
        const QDateTime& updateDt)
{
    QVariantMap params = { { "update_item", items }, { "update_scope", scope },
            { "update_filter", scope }, { "format", "xml" } };
    if (updateDt.isValid() && !updateDt.isNull()) {
        params.insert("max_updated_at", updateDt.toTime_t());
    }
    QNetworkReply *reply = m_OAuth1->Get(m_AccessToken, m_AccessTokenSecret,
            QUrl(m_BaseUrl + "/updates/friends"), params);
    m_Requester2Reply[requester] = reply;
    connect(reply, &QNetworkReply::finished,
             this, &GoodReadsApi::handleGetUpdates);
}

void GoodReadsApi::GetNotifications(QObject *requester, const QString& pageToken)
{
    QVariantMap params( { { "format", "xml" } } );
    if (!pageToken.isEmpty()) {
        params.insert("next_page_token", pageToken);
    }
    QNetworkReply *reply = m_OAuth1->Get(m_AccessToken, m_AccessTokenSecret,
            QUrl(m_BaseUrl + "/notifications"), params);
    m_Requester2Reply[requester] = reply;
    connect(reply, &QNetworkReply::finished,
            this, [this]() { handleGetNotifications(); });
}

void GoodReadsApi::GetMessages(QObject *requester, const QString& folder, int page, bool useCache)
{
    QNetworkReply *reply = m_OAuth1->Get(m_AccessToken, m_AccessTokenSecret,
            QUrl(m_BaseUrl + QString("/message/%1").arg(folder)),
            { { "format", "xml" }, { "page", page } }, useCache);
    m_Requester2Reply[requester] = reply;
    connect(reply, &QNetworkReply::finished,
            this, [this]() { handleGetMessages(); });
}

void GoodReadsApi::GetMessage(QObject *requester, quint64 messageId, bool useCache)
{
    QNetworkReply *reply = m_OAuth1->Get(m_AccessToken, m_AccessTokenSecret,
            QUrl(m_BaseUrl + QString("/message/show/%1").arg(messageId)),
            { { "format", "xml" } }, useCache);
    m_Requester2Reply[requester] = reply;
    connect(reply, &QNetworkReply::finished,
            this, [this]() { handleGetMessage(); });
}

void GoodReadsApi::MarkMessageAsRead(quint64 messageId)
{
    QNetworkReply *reply = m_OAuth1->Get(m_AccessToken, m_AccessTokenSecret,
            QUrl(m_BaseUrl + QString("/message/show/%1").arg(messageId)),
            { { "format", "xml" } });
    connect(reply, &QNetworkReply::finished,
            this, [this]() { handleMarkMessageAsRead(); });
}

void GoodReadsApi::GetBookShelves(QObject *requester, const QString& userId, int page,
        bool useCache)
{
    QNetworkReply *reply = m_OAuth1->Get(m_AccessToken, m_AccessTokenSecret,
            QUrl(m_BaseUrl + "/shelf/list"),
            { { "user_id", userId }, { "page", page }, { "format", "xml" } }, useCache);
    m_Requester2Reply[requester] = reply;
    connect(reply, &QNetworkReply::finished,
            this, [this, userId]() { handleGetBookShelves(userId); });
}

void GoodReadsApi::GetAllBookShelves(QObject *requester, const QString& userId, int page,
        bool useCache)
{
    QNetworkReply *reply = m_OAuth1->Get(m_AccessToken, m_AccessTokenSecret,
            QUrl(m_BaseUrl + "/shelf/list"),
            { { "user_id", userId }, { "page", page }, { "format", "xml" } }, useCache);
    m_Requester2Reply[requester] = reply;
    connect(reply, &QNetworkReply::finished,
            this, [this, requester, userId, page]()
            {
                handleGetAllBookShelves(requester, userId, page);
            });
}

void GoodReadsApi::AddBookShelf(const QString& name, bool exclusive, bool /*sortable*/, bool /*featured*/,
        bool recommendFor)
{
    const QVariantMap params = { { "user_shelf[name]", name },
        { "user_shelf[exclusive_flag]", (exclusive ? "true" : "false") },
        { "user_shelf[recommend_for]", (recommendFor ? "true" : "false") } };
    QNetworkReply *reply = m_OAuth1->Post(m_AccessToken, m_AccessTokenSecret,
            QUrl(m_BaseUrl + "/user_shelves.xml"), params);
    connect(reply, &QNetworkReply::finished,
            this, &GoodReadsApi::handleAddBookShelf);
}

void GoodReadsApi::EditBookShelf(quint64 id, const QString& name, bool exclusive,
        bool /*sortable*/, bool featured, bool recommendFor)
{
    const QVariantMap params = { { "user_shelf[name]", name },
        { "user_shelf[exclusive_flag]", (exclusive ? "true" : "false") },
        { "user_shelf[featured]", (featured ? "true" : "false") },
        { "user_shelf[recommend_for]", (recommendFor ? "true" : "false") } };

    QUrl url(m_BaseUrl + QString("/user_shelves/%1.xml").arg(id));
    QUrlQuery query = QUrlQuery(url.query());
    for (auto it = params.begin(), end = params.end(); it != end; ++it) {
        query.addQueryItem(it.key(), it.value().toString());
    }
    url.setQuery(query);

    QNetworkReply *reply = m_OAuth1->Put(m_AccessToken, m_AccessTokenSecret,
            url, QVariantMap());
    connect(reply, &QNetworkReply::finished,
            this, &GoodReadsApi::handleEditBookShelf);
}

void GoodReadsApi::GetReviews(QObject *requester, const QString& userId, const QString& bookShelf,
        const QString& sortField, Qt::SortOrder order, int page, bool useCache)
{
    const QVariantMap params = { { "v", 2 }, { "shelf", bookShelf }, { "page", page },
        { "per_page", 50 }, { "sort", sortField },
        { "order", (order == Qt::AscendingOrder ? "a" : "d") },
        { "format", "xml" }
    };
    auto reply = m_OAuth1->Get(m_AccessToken, m_AccessTokenSecret,
            QUrl(m_BaseUrl + QString("/review/list/%1").arg(userId)), params, useCache);
    m_Requester2Reply[requester] = reply;
    connect(reply, &QNetworkReply::finished,
            this, &GoodReadsApi::handleGetReviews);
}

void GoodReadsApi::GetReview(QObject *requester, const QString& reviewId, int commentsPage,
        bool useCache)
{
    auto reply = m_OAuth1->Get(m_AccessToken, m_AccessTokenSecret,
            QUrl(m_BaseUrl + "/review/show"),
            { { "id", reviewId }, { "page", commentsPage }, { "format", "xml" } }, useCache);
    m_Requester2Reply[requester] = reply;
    connect(reply, &QNetworkReply::finished,
            this, &GoodReadsApi::handleGetReview);
}

void GoodReadsApi::SearchReviews(QObject *requester, const QString& userId,
        const QString& searchText, int page)
{
    const QVariantMap params = { { "v", 2 }, { "search[query]", searchText }, { "page", page },
        { "per_page", 50 }, { "format", "xml" } };
    auto reply = m_OAuth1->Get(m_AccessToken, m_AccessTokenSecret,
            QUrl(m_BaseUrl + QString("/review/list/%1").arg(userId)), params);
    m_Requester2Reply[requester] = reply;
    connect(reply, &QNetworkReply::finished,
            this, &GoodReadsApi::handleSearchReviews);
}

void GoodReadsApi::AddReview(const QString& bookId, int rating, const QString& review)
{
    const QVariantMap params = { { "book_id", bookId },
        { "review[rating]", rating },
        { "review[review]", review } };
    QNetworkReply *reply = m_OAuth1->Post(m_AccessToken, m_AccessTokenSecret,
            QUrl(m_BaseUrl + "/review.xml"), params);
    connect(reply, &QNetworkReply::finished,
            this, &GoodReadsApi::handleAddReview);
}

void GoodReadsApi::EditReview(const QString& reviewId, int rating, const QString& review,
        bool finished)
{
    QVariantMap params = { { "id", reviewId },
        { "review[rating]", rating },
        { "review[review]", review } };
    if (finished) {
        params.insert("finished", finished);
    }

    QUrl url(m_BaseUrl + QString("/review/%1.xml").arg(reviewId));
    QUrlQuery query = QUrlQuery(url.query());
    for (auto it = params.begin(), end = params.end(); it != end; ++it) {
        query.addQueryItem(it.key(), it.value().toString());
    }
    url.setQuery(query);

    QNetworkReply *reply = m_OAuth1->Put(m_AccessToken, m_AccessTokenSecret,
            url, QVariantMap());
    connect(reply, &QNetworkReply::finished,
            this, &GoodReadsApi::handleEditReview);
}

void GoodReadsApi::DeleteReview(const QString& bookId, const QString& reviewId)
{
    auto reply = m_OAuth1->DeleteResource(m_AccessToken, m_AccessTokenSecret,
            QUrl(m_BaseUrl + QString("/review/destroy/%1?id=%2&format=xml").arg(bookId, reviewId)));
    connect(reply, &QNetworkReply::finished,
            this, [this, reviewId]() { handleDeleteReview(reviewId); });
}

void GoodReadsApi::GetBook(QObject *requester, const QString& bookId, bool useCache)
{
    auto reply = m_OAuth1->Get(m_AccessToken, m_AccessTokenSecret,
            QUrl(m_BaseUrl + "/book/show"), { { "id", bookId }, { "format", "xml" } }, useCache);
    m_Requester2Reply[requester] = reply;
    connect(reply, &QNetworkReply::finished,
            this, &GoodReadsApi::handleGetBook);
}

void GoodReadsApi::GetBookEditions(QObject *requester, quint64 workId, int page, bool useCache)
{
    auto reply = m_OAuth1->Get(m_AccessToken, m_AccessTokenSecret,
            QUrl(m_BaseUrl + QString("/work/editions/%1").arg(workId)),
            { { "format", "xml" }, { "page", page } }, useCache);
    m_Requester2Reply[requester] = reply;
    connect(reply, &QNetworkReply::finished,
            this, [this, workId]() { handleGetBookEditions(workId); });
}

void GoodReadsApi::SearchBooks(QObject *requester, const QString& query, const QString& key, int page)
{
    QVariantMap params = {
        { "q", query }, { "search[field]", key },
        { "format", "xml" }, { "page", page }
    };
    auto reply = m_OAuth1->Get(m_AccessToken, m_AccessTokenSecret,
            QUrl(m_BaseUrl + "/search/index.xml"), params);
    m_Requester2Reply[requester] = reply;
    connect(reply, &QNetworkReply::finished,
            this, &GoodReadsApi::handleSearchBook);
}

void GoodReadsApi::SwitchToBookEdition(const QString& reviewId, const QString& bookId)
{
    auto reply = m_OAuth1->Post(m_AccessToken, m_AccessTokenSecret,
            QUrl(m_BaseUrl + QString("/review/switch_edition/%1").arg(reviewId)),
            { { "book_id", bookId }, { "format", "xml" } });
    connect(reply, &QNetworkReply::finished,
            this, [this, reviewId, bookId]()
            { handleSwitchToBookEdition(reviewId, bookId); });
}

void GoodReadsApi::GetSeries(QObject *requester, quint64 seriesId, bool useCache)
{
    auto reply = m_OAuth1->Get(m_AccessToken, m_AccessTokenSecret,
            QUrl(m_BaseUrl + QString("/series/show/%1").arg(seriesId)),
            { { "format", "xml" } }, useCache);
    m_Requester2Reply[requester] = reply;
    connect(reply, &QNetworkReply::finished,
            this, &GoodReadsApi::handleGetSeries);
}

void GoodReadsApi::GetAuthorSeries(QObject *requester, const QString& authorId, bool useCache)
{
    auto reply = m_OAuth1->Get(m_AccessToken, m_AccessTokenSecret,
            QUrl(m_BaseUrl + QString("/series/list/%1.xml").arg(authorId)),
            { { "format", "xml" } }, useCache);
    m_Requester2Reply[requester] = reply;
    connect(reply, &QNetworkReply::finished,
            this, [this, authorId]() { handleGetAuthorSeries(authorId); });
}

void GoodReadsApi::GetWorkSeries(QObject */*requester*/, quint64 /*workId*/, bool /*useCache*/)
{
}

void GoodReadsApi::AddBookToShelf(const QString& bookId, const QString& shelfName,
        const QStringList& oldShelves)
{
    QNetworkReply *reply = m_OAuth1->Post(m_AccessToken, m_AccessTokenSecret,
            QUrl(m_BaseUrl + QString("/shelf/add_to_shelf.xml?book_id=%1&name=%2&a=")
                    .arg(bookId, shelfName)), {});
    connect(reply, &QNetworkReply::finished,
            this, [this, bookId, shelfName, oldShelves]()
            { handleAddBookToShelf(bookId, shelfName, oldShelves); });
}

void GoodReadsApi::AddBooksToShelves(const QStringList& bookIds, const QStringList& shelvesName,
        const QStringList& oldShelves)
{
    QNetworkReply *reply = m_OAuth1->Post(m_AccessToken, m_AccessTokenSecret,
            QUrl(m_BaseUrl + QString("/shelf/add_books_to_shelves.xml?bookids=%1&shelves=%2")
                    .arg(bookIds.join(','), shelvesName.join(','))), {});
    connect(reply, &QNetworkReply::finished,
            this, [this, bookIds, shelvesName, oldShelves]()
            { handleAddBooksToShelves(bookIds, shelvesName, oldShelves); });
}

void GoodReadsApi::RemoveBookFromShelf(const QString& bookId, const QString& shelfName)
{
    QNetworkReply *reply = m_OAuth1->Post(m_AccessToken, m_AccessTokenSecret,
            QUrl(m_BaseUrl + QString("/shelf/add_to_shelf.xml?book_id=%1&name=%2&a=remove")
                    .arg(bookId, shelfName)), {});
    connect(reply, &QNetworkReply::finished,
            this, [this, bookId]() { handleRemoveBookFromShelf(bookId); });
}

void GoodReadsApi::GetAuthor(QObject *requester, const QString& authorId, bool useCache)
{
    auto reply = m_OAuth1->Get(m_AccessToken, m_AccessTokenSecret,
            QUrl(m_BaseUrl + "/author/show"), { { "id", authorId }, { "format", "xml" } }, useCache);
    m_Requester2Reply[requester] = reply;
    connect(reply, &QNetworkReply::finished,
            this, &GoodReadsApi::handleGetAuthor);
}

void GoodReadsApi::GetAuthorBooks(QObject *requester, const QString& authorId, int page,
        bool useCache)
{
    auto reply = m_OAuth1->Get(m_AccessToken, m_AccessTokenSecret,
            QUrl(m_BaseUrl + "/author/list"),
            { { "id", authorId }, { "page", page }, { "format", "xml" } }, useCache);
    m_Requester2Reply[requester] = reply;
    connect(reply, &QNetworkReply::finished,
            this, [this, authorId]() {
                handleGetAuthorBooks(authorId);
            });
}

void GoodReadsApi::FollowAuthor(const QString& authorId)
{
    auto reply = m_OAuth1->Post(m_AccessToken, m_AccessTokenSecret,
            QUrl(m_BaseUrl + QString("/author_followings?id=%1&format=%2").arg(authorId).arg("xml")));
    connect(reply, &QNetworkReply::finished,
            this, &GoodReadsApi::handleFollowAuthor);
}

void GoodReadsApi::UnfollowAuthor(const QString& authorId, quint64 authorFollowingId)
{
    auto reply = m_OAuth1->DeleteResource(m_AccessToken, m_AccessTokenSecret,
            QUrl(m_BaseUrl + QString("/author_followings/%1?format=xml").arg(authorFollowingId)));
    connect(reply, &QNetworkReply::finished,
            this, [this, authorId]() {
                handleUnfollowAuthor(authorId);
            });
}

void GoodReadsApi::GetGroups(QObject *requester, const QString& userId, int page, bool useCache)
{
    auto reply = m_OAuth1->Get(m_AccessToken, m_AccessTokenSecret,
            QUrl(m_BaseUrl + QString("/group/list/%1").arg(userId)),
            { { "sort", "last_activity" }, { "page", page }, { "format", "xml" } }, useCache);
    m_Requester2Reply[requester] = reply;
    connect(reply, &QNetworkReply::finished,
            this, [this, userId]() { handleGetGroups(userId); });
}

void GoodReadsApi::GetGroup(QObject *requester, quint64 groupId, bool useCache)
{
    auto reply = m_OAuth1->Get(m_AccessToken, m_AccessTokenSecret,
            QUrl(m_BaseUrl + QString("/group/show/%1").arg(groupId)),
            { { "sort", "updated_at" }, { "format", "xml" } }, useCache);
    m_Requester2Reply[requester] = reply;
    connect(reply, &QNetworkReply::finished,
            this, [this, groupId]() { handleGetGroup(groupId); });
}

void GoodReadsApi::JoinGroup(quint64 groupId)
{
    auto reply = m_OAuth1->Post(m_AccessToken, m_AccessTokenSecret,
            QUrl(m_BaseUrl + "/group/join"), { { "format", "xml" }, { "id", groupId } });
    connect(reply, &QNetworkReply::finished,
            this, [this, groupId] { handleJoinGroup(groupId); });
}

void GoodReadsApi::SearchGroup(QObject *requester, const QString& text, int page)
{
    auto reply = m_OAuth1->Get(m_AccessToken, m_AccessTokenSecret,
            QUrl(m_BaseUrl + "/group/search"),
            { { "q", QUrl::toPercentEncoding(text) }, { "page", page }, { "format", "xml" } });
    m_Requester2Reply[requester] = reply;
    connect(reply, &QNetworkReply::finished,
            this, &GoodReadsApi::handleSearchGroup);
}

void GoodReadsApi::GetGroupMembers(QObject *requester, quint64 groupId, int page, bool useCache)
{
    auto reply = m_OAuth1->Get(m_AccessToken, m_AccessTokenSecret,
            QUrl(m_BaseUrl + QString("/group/members/%1").arg(groupId)),
            { { "page", page }, { "sort", "num_comments" }, { "format", "xml" } }, useCache);
    m_Requester2Reply[requester] = reply;
    connect(reply, &QNetworkReply::finished,
            this, [this, groupId] { handleGetGroupMembers(groupId); });
}

void GoodReadsApi::GetGroupFolderTopics(QObject *requester, const QString& groupFolderId,
        quint64 groupId, int page, bool useCache)
{
    QNetworkReply *reply = nullptr;
    const QVariantMap params = { { "format", "xml" }, { "page", page },
            { "sort", "updated_at" }, { "order", "d" }, { "group_id", groupId } };
    if (groupFolderId == GoodReadsApi::UnreadTopicsFolderId) {
        reply = m_OAuth1->Get(m_AccessToken, m_AccessTokenSecret,
                QUrl(m_BaseUrl + QString("/topic/unread_group/%1").arg(groupId)), params, useCache);
    }
    else {
        reply = m_OAuth1->Get(m_AccessToken, m_AccessTokenSecret,
                QUrl(m_BaseUrl + QString("/topic/group_folder/%1").arg(groupFolderId)), params,
                useCache);
    }
    m_Requester2Reply[requester] = reply;
    connect(reply, &QNetworkReply::finished,
            this,
            [this, groupFolderId, groupId] {
                handleGetGroupFolderTopics(groupFolderId, groupId);
            });
}

void GoodReadsApi::GetGroupFolderTopic(QObject *requester, const QString& topicId, int page)
{
    auto reply = m_OAuth1->Get(m_AccessToken, m_AccessTokenSecret,
            QUrl(m_BaseUrl + "/topic/show"),
            { { "id", topicId }, { "page", page }, { "format", "xml" } });
    m_Requester2Reply[requester] = reply;
    connect(reply, &QNetworkReply::finished,
            this, &GoodReadsApi::handleGetGroupFolderTopic);
}

void GoodReadsApi::AddNewTopic(const QString& topic, const QString& subject, quint64 subjectId,
        const QString& folderId, bool question, bool updateFeed, bool digest, const QString& comment)
{
    QVariantMap params = { { "topic[subject_type]", subject },
            { "topic[subject_id]", subjectId },
            { "topic[title]", topic }, { "topic[question_flag]", (question ? "1" : "0") },
            { "update_feed", (updateFeed ? "1" : "0") }, { "digest", (digest ? "1" : "0") },
            { "comment[body_usertext]", comment } };
    if (!folderId.isEmpty() && folderId != "0") {
        params.insert("topic[folder_id]", folderId);
    }
    auto reply = m_OAuth1->Post(m_AccessToken, m_AccessTokenSecret,
            QUrl(m_BaseUrl + "/topic.xml"), params);
    connect(reply, &QNetworkReply::finished,
            this, [this, folderId]() { handleTopicAdded(folderId); });
}

void GoodReadsApi::GetComments(QObject *requester,
        const QString& resourceId, const QString& resourceType, int page)
{
    auto reply = m_OAuth1->Get(m_AccessToken, m_AccessTokenSecret,
            QUrl(m_BaseUrl + QString("/%1/%2/comments").arg(resourceType, resourceId)),
            { { "format", "xml" }, { "page", page} });
    m_Requester2Reply[requester] = reply;
    connect(reply, &QNetworkReply::finished,
            this, [this, resourceId]() { handleGetComments(resourceId); });
}

void GoodReadsApi::AddNewComment(const QString& type, const QString& resourceId, const QString& comment)
{
    const QVariantMap params = { { "type", type }, { "id", resourceId },
            { "comment[body]", comment } };
    auto reply = m_OAuth1->Post(m_AccessToken, m_AccessTokenSecret,
            QUrl(m_BaseUrl + "/comment.xml"), params);
    connect(reply, &QNetworkReply::finished,
             this, [this, resourceId]() { handleNewCommentAdded(resourceId); });
}

void GoodReadsApi::GetFriends(QObject *requester, const QString& userId, int page, bool useCache)
{
    auto reply = m_OAuth1->Get(m_AccessToken, m_AccessTokenSecret,
            QUrl(m_BaseUrl + QString("/friend/user/%1").arg(userId)),
            { { "format", "xml" }, { "page", page } }, useCache);
    m_Requester2Reply[requester] = reply;
    connect(reply, &QNetworkReply::finished,
             this, [this, userId]() { handleGetFriends(userId); });
}

void GoodReadsApi::GetUserFollowers(QObject *requester, const QString& userId, int page,
        bool useCache)
{
    auto reply = m_OAuth1->Get(m_AccessToken, m_AccessTokenSecret,
            QUrl(m_BaseUrl + QString("/user/%1/followers").arg(userId)),
            { { "page", page }, { "format", "xml" } }, useCache );
    m_Requester2Reply[requester] = reply;
    connect(reply, &QNetworkReply::finished,
             this, [this, userId]() { handleGetUserFollowers(userId); });
}

void GoodReadsApi::GetUserFollowings(QObject *requester, const QString& userId, int page,
        bool useCache)
{
    auto reply = m_OAuth1->Get(m_AccessToken, m_AccessTokenSecret,
            QUrl(m_BaseUrl + QString("/user/%1/following").arg(userId)),
            { { "page", page }, { "format", "xml" } }, useCache );
    m_Requester2Reply[requester] = reply;
    connect(reply, &QNetworkReply::finished,
             this, [this, userId]() { handleGetUserFollowings(userId); });
}

void GoodReadsApi::GetFriendRequests(QObject *requester, int page)
{
    auto reply = m_OAuth1->Get(m_AccessToken, m_AccessTokenSecret,
            QUrl(m_BaseUrl + "/friend/requests"), { { "page", page }, { "format", "xml" } });
    m_Requester2Reply[requester] = reply;
    connect(reply, &QNetworkReply::finished,
             this, &GoodReadsApi::handleGetFriendRequests);
}

void GoodReadsApi::ConfirmFriendRequest(quint64 friendRequestId, bool confirm)
{
    auto reply = m_OAuth1->Post(m_AccessToken, m_AccessTokenSecret,
            QUrl(m_BaseUrl + "/friend/confirm_request.xml"),
            { { "id", friendRequestId }, { "response", (confirm ? "Y" : "N") } });
    connect(reply, &QNetworkReply::finished,
            this,
            [this, friendRequestId, confirm]() {
                handleConfirmFriendRequest(friendRequestId, confirm);
            });
}

void GoodReadsApi::ConfirmFriendRecommendation(quint64 friendRecommendationId, bool confirm)
{
    auto reply = m_OAuth1->Post(m_AccessToken, m_AccessTokenSecret,
            QUrl(m_BaseUrl + "/friend/confirm_recommendation.xml"),
            { { "id", friendRecommendationId }, { "response", (confirm ? "Y" : "N") } });
    connect(reply, &QNetworkReply::finished,
            this,
            [this, friendRecommendationId, confirm]() {
                handleConfirmFriendRecommendation(friendRecommendationId, confirm);
            });
}

void GoodReadsApi::AddFriend(const QString& userId)
{
    auto reply = m_OAuth1->Post(m_AccessToken, m_AccessTokenSecret,
            QUrl(m_BaseUrl + "/friend/add_as_friend.xml"), { { "id", userId } });
    connect(reply, &QNetworkReply::finished,
            this, [this, userId]() { handleAddFriend(userId); });
}

void GoodReadsApi::RemoveFriend(const QString& /*userId*/)
{
}

void GoodReadsApi::FollowUser(const QString& userId)
{
    auto reply = m_OAuth1->Post(m_AccessToken, m_AccessTokenSecret,
            QUrl(m_BaseUrl + QString("/user/%1/followers").arg(userId)), { { "format", "xml" } });
    connect(reply, &QNetworkReply::finished,
            this, &GoodReadsApi::handleFollowUser);
}

void GoodReadsApi::UnfollowUser(const QString& userId)
{
    auto reply = m_OAuth1->DeleteResource(m_AccessToken, m_AccessTokenSecret,
            QUrl(m_BaseUrl + QString("/user/%1/followers/stop_following?format=xml").arg(userId)));
    connect(reply, &QNetworkReply::finished,
            this, [this, userId]() { handleUnfollowUser(userId); });
}

void GoodReadsApi::LoadUserQuotes(QObject *requester, const QString &userId, int page, bool useCache)
{
    auto reply = m_OAuth1->Get(m_AccessToken, m_AccessTokenSecret,
            QUrl(m_BaseUrl + QString("/quotes/list_rss/%1").arg(userId)),
            { { "page", page } }, useCache);
    m_Requester2Reply[requester] = reply;
    connect(reply, &QNetworkReply::finished,
            this, [this, userId]() { handleGotUserQuotes(userId); });
}

void GoodReadsApi::LoadBookQuotes(QObject *requester, quint64 workId, int page, bool useCache)
{
    auto reply = m_OAuth1->Get(m_AccessToken, m_AccessTokenSecret,
            QUrl(m_BaseUrl + QString("/work/quotes/%1").arg(workId)),
            { { "format", "json" }, { "page", page } }, useCache);
    m_Requester2Reply[requester] = reply;
    connect(reply, &QNetworkReply::finished,
            this, [this, workId]() { handleGotBookQuotes(workId); });
}

void GoodReadsApi::LoadAuthorQuotes(QObject *requester, const QString& authorId, int page,
        bool useCache)
{
    auto reply = m_OAuth1->Get(m_AccessToken, m_AccessTokenSecret,
            QUrl(m_BaseUrl + QString("/author/quotes/%1").arg(authorId)),
            { { "format", "json" }, { "page", page } }, useCache);
    m_Requester2Reply[requester] = reply;
    connect(reply, &QNetworkReply::finished,
            this, [this, authorId]() { handleGotAuthorQuotes(authorId); });
}

void GoodReadsApi::AddQuote(const QString&/* authorName*/, quint64/* authorId*/, quint64/* bookId*/,
    const QString&/* quote*/, const QStringList&/* tags*/)
{
//    const auto& pair = m_OAuthWrapper->MakeSignedUrl(m_AccessToken, m_AccessTokenSecret,
//            QUrl(QString("https://www.goodreads.com/quotes?format=xml&quote[author_name]=%1&"
//                         "quote[author_id]=%2&quote[book_id]=%3&quote[body]=%4&quote[tags]=%5")
//                    .arg(authorName).arg(authorId).arg(bookId).arg(quote).arg(tags.join(","))),
//            "POST");
//    auto reply = m_NAM->post(PreparePostRequest(pair.first), pair.second);
//    connect(reply, &QNetworkReply::finished,
//            this, &GoodReadsApi::handleAddQuote);
}

void GoodReadsApi::GetReadStatus(QObject *requester, const QString& readStatusId,
        int page, bool useCache)
{
    auto reply = m_OAuth1->Get(m_AccessToken, m_AccessTokenSecret,
            QUrl(m_BaseUrl + QString("/read_statuses/%1").arg(readStatusId)),
            { { "format", "xml" }, { "page", page } }, useCache);
    m_Requester2Reply[requester] = reply;
    connect(reply, &QNetworkReply::finished,
            this, &GoodReadsApi::handleGetReadStatus);
}

void GoodReadsApi::GetRecentUserStatuses(QObject */*requester*/)
{
//    const auto& pair = m_OAuthWrapper->MakeSignedUrl(m_AccessToken, m_AccessTokenSecret,
//            QUrl(QString("https://www.goodreads.com/user_status/index.xml")));
//    auto reply = m_NAM->get(QNetworkRequest(pair.first));
//    connect(reply, &QNetworkReply::finished,
//            this, &GoodReadsApi::handleGetRecentUserStatuses);
}

void GoodReadsApi::GetUserStatus(QObject *requester, const QString& userStatusId, int page,
        bool useCache)
{
    auto reply = m_OAuth1->Get(m_AccessToken, m_AccessTokenSecret,
            QUrl(m_BaseUrl + QString("/user_status/show/%1").arg(userStatusId)),
            { { "format", "xml" }, { "page", page } }, useCache);
    m_Requester2Reply[requester] = reply;
    connect(reply, &QNetworkReply::finished,
            this, &GoodReadsApi::handleGetUserStatus);
}

void GoodReadsApi::UpdateReadingProgress(const QString& bookId, const QString& key, int value,
        const QString& comment)
{
    auto reply = m_OAuth1->Post(m_AccessToken, m_AccessTokenSecret,
            QUrl(m_BaseUrl + "/user_status.xml"),
            { { "user_status[book_id]", bookId },
              { QString("user_status[%1]").arg(key), value },
              { "user_status[body]", comment } });
    connect(reply, &QNetworkReply::finished,
            this, &GoodReadsApi::handleUpdateReadingProgress);
}

void GoodReadsApi::DeleteUserStatus(quint64/* userStatusId*/)
{
//    const auto& pair = m_OAuthWrapper->MakeSignedUrl(m_AccessToken, m_AccessTokenSecret,
//            QUrl(QString("https://www.goodreads.com/user_status/destroy/%1?format=xml")
//                    .arg(userStatusId)), "POST");
//    auto reply = m_NAM->post(PreparePostRequest(pair.first), pair.second);
//    connect(reply, &QNetworkReply::finished,
    //             this, &GoodReadsApi::handleDeleteUserStatus);
}

void GoodReadsApi::GetRecommendation(QObject *requester, const QString& id, int page, bool useCache)
{
    auto reply = m_OAuth1->Get(m_AccessToken, m_AccessTokenSecret,
            QUrl(m_BaseUrl + QString("/recommendations/%1").arg(id)),
            { { "format", "xml" }, { "page", page } }, useCache);
    m_Requester2Reply[requester] = reply;
    connect(reply, &QNetworkReply::finished,
            this, &GoodReadsApi::handleGetRecommendation);
}

void GoodReadsApi::LikeResource(const QString& resourceId, const QString& resourceType)
{
    auto reply = m_OAuth1->Post(m_AccessToken, m_AccessTokenSecret,
            QUrl(m_BaseUrl + "/rating?format=xml"),
            { { "rating[rating]", 1 },
              { "rating[resource_id]", resourceId },
              { "rating[resource_type]", resourceType } });
    connect(reply, &QNetworkReply::finished,
            this, [this, resourceId]() { handleLikeResource(resourceId); });
}

void GoodReadsApi::UnlikeResource(const QString& resourceId, quint64 ratingId)
{
    auto reply = m_OAuth1->DeleteResource(m_AccessToken, m_AccessTokenSecret,
            QUrl(m_BaseUrl + QString("/rating?id=%1&format=xml").arg(ratingId)));
    connect(reply, &QNetworkReply::finished,
            this, [this, resourceId]() { handleUnlikeResource(resourceId); });
}

namespace
{
QString GetQueryResult(QXmlQuery& query, const QString& request)
{
    QString result;
    query.setQuery(request);
    if (!query.evaluateTo(&result)) {
        result = QString();
    }
    return result.trimmed();
}

QUrl GetRedirectedUrl(const QDomDocument& doc)
{
    QXmlQuery query;
    query.setFocus(doc.toByteArray());
    const QString url(GetQueryResult(query, "/html/body/a/@href/data(.)"));
    return QUrl(url);
}
}

QDomDocument GoodReadsApi::ParseDocument(const QByteArray& data, bool& ok)
{
    QDomDocument document;
    QString errorMsg;
    int errorLine = -1;
    int errorColumn = -1;
    if (!document.setContent(data, &errorMsg, &errorLine, &errorColumn))
    {
        qWarning() << Q_FUNC_INFO
                << errorMsg
                << "in line:"
                << errorLine
                << "column:"
                << errorColumn;
        ok = false;
        emit error(QObject::tr("Reply data is corrupted"));
    }
    else
    {
        ok = true;
    }
    return document;
}

QJsonDocument GoodReadsApi::ParseJsonDocument(const QByteArray& data, bool& ok)
{
    QJsonParseError err;
    QJsonDocument document = QJsonDocument::fromJson(QString::fromUtf8(data).toUtf8(), &err);
    if (document.isNull()) {
        qWarning() << Q_FUNC_INFO
                << err.errorString();
        ok = false;
    }
    else
    {
        ok = true;
    }
    return document;
}

QByteArray GoodReadsApi::GetReply(QObject *sender, bool& ok)
{
    auto reply = qobject_cast<QNetworkReply*>(sender);
    QByteArray data;
    if (!reply) {
        qWarning() << "Invalid reply";
        ok = false;
        return data;
    }
    reply->deleteLater();
    m_Requester2Reply.remove(m_Requester2Reply.key(reply));

    if (reply->error() != QNetworkReply::NoError &&
            reply->error() != QNetworkReply::OperationCanceledError &&
            reply->error() != QNetworkReply::AuthenticationRequiredError) {
        qWarning() << Q_FUNC_INFO << "There is network error: "
                << reply->error() << reply->errorString();
        ok = false;
        emit error(tr("Network error: %1").arg(reply->errorString()));
        return data;
    }

#ifdef QT_DEBUG
    qDebug() << reply->error() << reply->errorString();
#endif
    //Disable warning on request cancel
    if (reply->error() == QNetworkReply::OperationCanceledError) {
        ok = true;
        return data;
    }

    if (reply->error() == QNetworkReply::AuthenticationRequiredError) {
        ok = false;
        emit error(tr("Network error: %1").arg(reply->errorString()));
        emit authenticationFailed();
        return data;
    }

    ok = true;
    data = reply->readAll();
#ifdef QT_DEBUG
    qDebug() << data;
#endif
    return data;
}

QDomDocument GoodReadsApi::GetDocumentFromReply(QObject *sender, bool& ok)
{
    QDomDocument doc;
    QByteArray data = GetReply(sender, ok);
    if (!ok || data.isEmpty()) {
        return doc;
    }

    ok = false;
    return ParseDocument(data, ok);
}

QJsonDocument GoodReadsApi::GetJsonDocumentFromReply(QObject *sender, bool& ok)
{
    QJsonDocument doc;
    QByteArray data = GetReply(sender, ok);
    if (!ok || data.isEmpty()) {
        return doc;
    }

    ok = false;
    return ParseJsonDocument(data, ok);
}

void GoodReadsApi::handleObtainRequestToken()
{
    bool ok = false;
    QByteArray data = GetReply(sender(), ok);
    if (!ok || data.isEmpty()) {
        emit requestFinished();
        return;
    }

    QList<QByteArray> tokens = data.split('&');
    if (tokens.size() != 2 ||
            !tokens.value(0).contains("oauth_token") ||
            !tokens.value(1).contains("oauth_token_secret")) {
        qWarning() << Q_FUNC_INFO << "Invalid answer";
        emit requestFinished();
        return;
    }

    m_RequestToken = QString::fromUtf8(tokens.value(0).mid(12));
    m_RequestTokenSecret = QString::fromUtf8(tokens.value(1).mid(19));
    emit requestTokenChanged(m_RequestToken);
    emit requestFinished();
}

void GoodReadsApi::handleRequestAccessToken()
{
    bool ok = false;
    QByteArray data = GetReply(sender(), ok);
    if (!ok || data.isEmpty()) {
        emit requestFinished();
        return;
    }

    QList<QByteArray> tokens = data.split('&');
    if (tokens.size() != 2 ||
            !tokens.value(0).contains("oauth_token") ||
            !tokens.value(1).contains("oauth_token_secret")) {
        qWarning() << Q_FUNC_INFO << "Invalid answer";
        emit requestFinished();
        return;
    }

    QString accessToken = QString::fromUtf8(tokens.value(0).mid(12));
    QString accessTokenSecret = QString::fromUtf8(tokens.value(1).mid(19));

    emit accessTokensChanged(accessToken, accessTokenSecret);
    emit requestFinished();
}

void GoodReadsApi::handleAuthUser()
{
    bool ok = false;
    QByteArray data = GetReply(sender(), ok);
    if (!ok || data.isEmpty()) {
        emit requestFinished();
        return;
    }

    QXmlQuery query;
    query.setFocus(data);
    const QString id(GetQueryResult(query, "/GoodreadsResponse/user/@id/string()"));
    const QString name(GetQueryResult(query, "/GoodreadsResponse/user/name/text()"));
    const QString link(GetQueryResult(query, "/GoodreadsResponse/user/link/text()"));

    emit requestFinished();
    emit gotAuthUserInfo(id, name, link);
}

void GoodReadsApi::handleGetUserInfo()
{
    bool ok = false;
    auto doc = GetDocumentFromReply(sender(), ok);
    if (!ok) {
        emit requestFinished();
        return;
    }

    emit gotUserProfile(RpcUtils::Parser::ParseUser(doc));
    emit requestFinished();
}

void GoodReadsApi::handleCompareBooks()
{
    bool ok = false;
    auto doc = GetDocumentFromReply(sender(), ok);
    if (!ok) {
        emit requestFinished();
        return;
    }

    //TODO
    qDebug() << doc.toByteArray();
    emit requestFinished();
}

void GoodReadsApi::handleGetUpdates()
{
    bool ok = false;
    auto doc = GetDocumentFromReply(sender(), ok);
    if (!ok) {
        emit requestFinished();
        return;
    }

    emit gotUpdates(RpcUtils::Parser::ParseUpdates(doc));
    emit requestFinished();
}

void GoodReadsApi::handleGetNotifications()
{
    bool ok = false;
    auto doc = GetDocumentFromReply(sender(), ok);
    if (!ok) {
        emit requestFinished();
        return;
    }

    QXmlQuery query;
    query.setFocus(doc.toByteArray());
    const QString nextPageToken(GetQueryResult(query,
            "/GoodreadsResponse/notifications/@next_page_token/string()"));
    emit gotNotifications(nextPageToken, RpcUtils::Parser::ParseNotifications(doc));
    emit requestFinished();
}

void GoodReadsApi::handleGetMessages()
{
    bool ok = false;
    auto doc = GetDocumentFromReply(sender(), ok);
    if (!ok) {
        emit requestFinished();
        return;
    }

    QXmlQuery query;
    query.setFocus(doc.toByteArray());
    const QString folder(GetQueryResult(query, "/GoodreadsResponse/message_folder/folder_name/text()"));
    emit gotMessages(folder, RpcUtils::Parser::ParseMessages(doc));
    emit requestFinished();
}

void GoodReadsApi::handleGetMessage()
{
    bool ok = false;
    auto doc = GetDocumentFromReply(sender(), ok);
    if (!ok) {
        emit requestFinished();
        return;
    }

    emit gotMessage(RpcUtils::Parser::ParseMessage(doc));
    emit requestFinished();
}

void GoodReadsApi::handleMarkMessageAsRead()
{
    bool ok = false;
    auto doc = GetDocumentFromReply(sender(), ok);
    if (!ok) {
        emit requestFinished();
        return;
    }

    emit gotMessage(RpcUtils::Parser::ParseMessage(doc));
    emit requestFinished();
}

void GoodReadsApi::handleGetBookShelves(const QString& userId)
{
    bool ok = false;
    auto doc = GetDocumentFromReply(sender(), ok);
    if (!ok) {
        emit requestFinished();
        return;
    }

    emit gotUserBookShelves(userId, RpcUtils::Parser::ParseBookShelves(doc));
    emit requestFinished();
}

void GoodReadsApi::handleGetAllBookShelves(QObject *requester, const QString& userId, int page)
{
    bool ok = false;
    auto doc = GetDocumentFromReply(sender(), ok);
    if (!ok) {
        emit requestFinished();
        return;
    }

    CountedItems<BookShelf> bookShelves = RpcUtils::Parser::ParseBookShelves(doc);
    emit gotUserBookShelves(userId, bookShelves);
    if (bookShelves.m_EndIndex != bookShelves.m_Count) {
        GetAllBookShelves(requester, userId, ++page);
    }
    else {
        emit requestFinished();
    }
}

void GoodReadsApi::handleAddBookShelf()
{
    bool ok = false;
    auto doc = GetDocumentFromReply(sender(), ok);
    if (!ok) {
        emit requestFinished();
        return;
    }

    emit bookShelfAdded(RpcUtils::Parser::ParseBookShelf(doc.firstChildElement("user_shelf")));
    emit requestFinished();
}

void GoodReadsApi::handleEditBookShelf()
{
    bool ok = false;
    auto doc = GetDocumentFromReply(sender(), ok);
    if (!ok) {
        emit requestFinished();
        return;
    }

    emit bookShelfEdited(RpcUtils::Parser::ParseBookShelf(doc.firstChildElement("user_shelf")));
    emit requestFinished();
}

void GoodReadsApi::handleGetReviews()
{
    bool ok = false;
    auto doc = GetDocumentFromReply(sender(), ok);
    if (!ok) {
        emit requestFinished();
        return;
    }

    const auto& pair = RpcUtils::Parser::ParseReviews(doc);
    emit gotReviews(pair.first, pair.second);
    emit requestFinished();
}

void GoodReadsApi::handleGetReview()
{
    bool ok = false;
    auto doc = GetDocumentFromReply(sender(), ok);
    if (!ok) {
        emit requestFinished();
        return;
    }

    emit gotReview(RpcUtils::Parser::ParseReview(doc));
    emit requestFinished();
}

void GoodReadsApi::handleSearchReviews()
{
    bool ok = false;
    auto doc = GetDocumentFromReply(sender(), ok);
    if (!ok) {
        emit requestFinished();
        return;
    }

    emit gotFoundReviews(RpcUtils::Parser::ParseFoundReviews(doc));
    emit requestFinished();
}

void GoodReadsApi::handleAddReview()
{
    bool ok = false;
    auto doc = GetDocumentFromReply(sender(), ok);
    if (!ok) {
        emit requestFinished();
        return;
    }

    emit gotReviewInfo(RpcUtils::Parser::ParseReviewInfo(doc));
    emit requestFinished();
}

void GoodReadsApi::handleEditReview()
{
    bool ok = false;
    auto doc = GetDocumentFromReply(sender(), ok);
    if (!ok) {
        emit requestFinished();
        return;
    }

    emit gotReviewInfo(RpcUtils::Parser::ParseReviewInfo(doc));
    emit requestFinished();
}

void GoodReadsApi::handleDeleteReview(const QString&)
{
    bool ok = false;
    auto doc = GetDocumentFromReply(sender(), ok);
    if (!ok) {
        emit requestFinished();
        return;
    }

    //TODO
    qDebug() << doc.toByteArray();
    emit requestFinished();
}

void GoodReadsApi::handleGetBook()
{
    bool ok = false;
    auto doc = GetDocumentFromReply(sender(), ok);
    if (!ok) {
        emit requestFinished();
        return;
    }

    emit gotBook(RpcUtils::Parser::ParseBook(doc));
    emit requestFinished();
}

void GoodReadsApi::handleGetBookEditions(quint64 workId)
{
    bool ok = false;
    auto doc = GetDocumentFromReply(sender(), ok);
    if (!ok) {
        emit requestFinished();
        return;
    }

    emit gotBookEditions(workId, RpcUtils::Parser::ParseBookEditions(doc));
    emit requestFinished();
}

void GoodReadsApi::handleSearchBook()
{
    bool ok = false;
    auto doc = GetDocumentFromReply(sender(), ok);
    if (!ok) {
        emit requestFinished();
        return;
    }

    emit gotFoundBooks(RpcUtils::Parser::ParseFoundBooks(doc));
    emit requestFinished();
}

void GoodReadsApi::handleSwitchToBookEdition(const QString& reviewId, const QString& bookId)
{
    bool ok = false;
    auto doc = GetDocumentFromReply(sender(), ok);
    if (!ok) {
        emit requestFinished();
        return;
    }

    emit bookEditionSwitched(reviewId, bookId);
    emit requestFinished();
}

void GoodReadsApi::handleGetSeries()
{
    bool ok = false;
    auto doc = GetDocumentFromReply(sender(), ok);
    if (!ok) {
        emit requestFinished();
        return;
    }

    emit gotSeries(RpcUtils::Parser::ParseSeries(doc));
    emit requestFinished();
}

void GoodReadsApi::handleGetAuthorSeries(const QString& authorId)
{
    bool ok = false;
    auto doc = GetDocumentFromReply(sender(), ok);
    if (!ok) {
        emit requestFinished();
        return;
    }

    emit gotAuthorSeries(authorId, RpcUtils::Parser::ParseAuthorSeries(doc));
    emit requestFinished();
}

void GoodReadsApi::handleGetWorkSeries()
{
    bool ok = false;
    auto doc = GetDocumentFromReply(sender(), ok);
    if (!ok) {
        emit requestFinished();
        return;
    }

    //TODO
    qDebug() << doc.toByteArray();
    emit requestFinished();
}

void GoodReadsApi::handleAddBookToShelf(const QString& bookId, const QString& shelfName,
        const QStringList& oldShelves)
{
    bool ok = false;
    auto doc = GetDocumentFromReply(sender(), ok);
    if (!ok) {
        emit requestFinished();
        return;
    }

    emit bookAddedToShelf(bookId, shelfName, oldShelves,
            RpcUtils::Parser::ParseBookShelfAddedReview(doc));
    emit requestFinished();
}

void GoodReadsApi::handleAddBooksToShelves(const QStringList& /*bookIds*/,
        const QStringList& shelvesName, const QStringList& oldShelves)
{
    bool ok = false;
    auto doc = GetDocumentFromReply(sender(), ok);
    if (!ok) {
        emit requestFinished();
        return;
    }

    Reviews_t reviews = RpcUtils::Parser::ParseBookShelfAddedReviews(doc);
    for (const auto& review : reviews) {
        emit bookAddedToShelves(review->GetBook()->GetId(), shelvesName, oldShelves,
                review);
    }
    emit requestFinished();
}

void GoodReadsApi::handleRemoveBookFromShelf(const QString& bookId)
{
    bool ok = false;
    auto doc = GetDocumentFromReply(sender(), ok);
    if (!ok) {
        emit requestFinished();
        return;
    }

    QXmlQuery query;
    query.setFocus(doc.toByteArray());
    const QString shelfName(GetQueryResult(query, "/hash/name/text()"));
    emit bookRemovedFromShelf(bookId, shelfName);
    emit requestFinished();
}

void GoodReadsApi::handleGetAuthor()
{
    bool ok = false;
    auto doc = GetDocumentFromReply(sender(), ok);
    if (!ok) {
        emit requestFinished();
        return;
    }

    emit gotAuthorProfile(RpcUtils::Parser::ParseAuthor(doc));
    emit requestFinished();
}

void GoodReadsApi::handleGetAuthorBooks(const QString& authorId)
{
    bool ok = false;
    auto doc = GetDocumentFromReply(sender(), ok);
    if (!ok) {
        emit requestFinished();
        return;
    }

    emit gotAuthorBooks(authorId, RpcUtils::Parser::ParseAuthorBooks(doc));
    emit requestFinished();
}

void GoodReadsApi::handleFollowAuthor()
{
    bool ok = false;
    auto doc = GetDocumentFromReply(sender(), ok);
    if (!ok) {
        emit requestFinished();
        return;
    }

    QXmlQuery query;
    query.setFocus(doc.toByteArray());
    const QString followingId(GetQueryResult(query, "/GoodreadsResponse/author_following/id/text()"));
    const QString authorId(GetQueryResult(query, "/GoodreadsResponse/author_following/author/id/text()"));

    emit authorFollowed(authorId, followingId.toULongLong());
    emit requestFinished();
}

void GoodReadsApi::handleUnfollowAuthor(const QString& authorId)
{
    bool ok = false;
    auto doc = GetDocumentFromReply(sender(), ok);
    if (!ok) {
        emit requestFinished();
        return;
    }

    if (doc.isNull()) {
        emit authorUnfollowed(authorId);
    }
    emit requestFinished();
}

void GoodReadsApi::handleGetGroups(const QString& userId)
{
    bool ok = false;
    auto doc = GetDocumentFromReply(sender(), ok);
    if (!ok) {
        emit requestFinished();
        return;
    }

    emit gotUserGroups(userId, RpcUtils::Parser::ParseGroups(doc));
    emit requestFinished();
}

namespace
{
GroupPtr CreateSecretGroup(quint64 groupId)
{
    GroupPtr group = std::make_shared<Group>();
    group->SetId(groupId);
    group->SetGroupAccess(Group::Secret);
    return group;
}
}

void GoodReadsApi::handleGetGroup(quint64 groupId, QObject *senderObject)
{
    auto reply = qobject_cast<QNetworkReply*>(senderObject ? senderObject : sender());
    if (!reply) {
        qWarning() << "Invalid reply";
        emit requestFinished();
        return;
    }
    reply->deleteLater();
    m_Requester2Reply.remove(m_Requester2Reply.key(reply));

    if (reply->error() == QNetworkReply::ContentOperationNotPermittedError) {
        emit gotUserGroup(groupId, CreateSecretGroup(groupId));
        emit requestFinished();
        return;
    }

    if (reply->error() != QNetworkReply::NoError &&
            reply->error() != QNetworkReply::OperationCanceledError &&
            reply->error() != QNetworkReply::AuthenticationRequiredError) {
        qWarning() << Q_FUNC_INFO << "There is network error: "
                << reply->error() << reply->errorString();
        emit requestFinished();
        return;
    }

    if (reply->error() == QNetworkReply::OperationCanceledError) {
        emit requestFinished();
        return;
    }

    if (reply->error() == QNetworkReply::AuthenticationRequiredError) {
        emit authenticationFailed();
        emit requestFinished();
        return;
    }
    QByteArray data = reply->readAll();
#ifdef QT_DEBUG
    qDebug() << data;
#endif
    bool ok = false;
    QDomDocument doc = ParseDocument(data, ok);
    if (!ok) {
        emit requestFinished();
        return;
    }

    const QUrl& redirectedUrl = GetRedirectedUrl(doc);
    if (!redirectedUrl.isEmpty()) {
        RequestRedirectedUrl(redirectedUrl,
                std::bind(&GoodReadsApi::handleGetGroup, this, groupId, std::placeholders::_1));
        return;
    }

    emit gotUserGroup(groupId, RpcUtils::Parser::ParseGroup(doc));
    emit requestFinished();
}

void GoodReadsApi::handleJoinGroup(quint64 groupId)
{
    bool ok = false;
    auto doc = GetDocumentFromReply(sender(), ok);
    if (!ok) {
        emit requestFinished();
        return;
    }

    emit gotUserGroup(groupId, RpcUtils::Parser::ParseGroup(doc));
    emit requestFinished();
}

void GoodReadsApi::handleSearchGroup()
{
    bool ok = false;
    auto doc = GetDocumentFromReply(sender(), ok);
    if (!ok) {
        emit requestFinished();
        return;
    }

    emit gotFoundGroups(RpcUtils::Parser::ParseGroups(doc));
    emit requestFinished();
}

void GoodReadsApi::handleGetGroupMembers(quint64 groupId, QObject *senderObject)
{
    bool ok = false;
    auto doc = GetDocumentFromReply(senderObject ? senderObject : sender(), ok);
    if (!ok) {
        emit requestFinished();
        return;
    }

    const QUrl& redirectedUrl = GetRedirectedUrl(doc);
    if (!redirectedUrl.isEmpty()) {
        qDebug() << Q_FUNC_INFO << "Redirected url for" << groupId;
        emit gotGroupMembers(groupId, {});
        emit requestFinished();
        return;
    }

    emit gotGroupMembers(groupId, RpcUtils::Parser::ParseGroupMembers(doc));
    emit requestFinished();
}

void GoodReadsApi::handleGetGroupFolderTopics(const QString& groupFolderId, quint64 groupId)
{
    bool ok = false;
    auto doc = GetDocumentFromReply(sender(), ok);
    if (!ok) {
        emit requestFinished();
        return;
    }
    emit gotGroupFolderTopics(groupFolderId, groupId, RpcUtils::Parser::ParseGroupFolderTopics(doc));
    emit requestFinished();
}

void GoodReadsApi::handleGetGroupFolderTopic()
{
    bool ok = false;
    auto doc = GetDocumentFromReply(sender(), ok);
    if (!ok) {
        emit requestFinished();
        return;
    }
    emit gotGroupFolderTopic(RpcUtils::Parser::ParseGroupFolderTopic(doc));
    emit requestFinished();
}

void GoodReadsApi::handleTopicAdded(const QString& folderId)
{
    bool ok = false;
    auto doc = GetDocumentFromReply(sender(), ok);
    if (!ok) {
        emit requestFinished();
        return;
    }
    emit gotNewGroupFolderTopic(folderId, RpcUtils::Parser::ParseGroupFolderTopic(doc));
    emit requestFinished();
}

void GoodReadsApi::handleGetComments(const QString& resourceId)
{
    bool ok = false;
    auto doc = GetDocumentFromReply(sender(), ok);
    if (!ok) {
        emit requestFinished();
        return;
    }
    emit gotComments(resourceId, RpcUtils::Parser::ParseComments(doc));
    emit requestFinished();
}

void GoodReadsApi::handleNewCommentAdded(const QString& resourceId)
{
    bool ok = false;
    auto doc = GetDocumentFromReply(sender(), ok);
    if (!ok) {
        emit requestFinished();
        return;
    }
    emit newCommentAdded(resourceId, RpcUtils::Parser::ParseComment(doc));
    emit requestFinished();
}

void GoodReadsApi::handleGetFriends(const QString& userId)
{
    bool ok = false;
    auto doc = GetDocumentFromReply(sender(), ok);
    if (!ok) {
        emit requestFinished();
        return;
    }

    emit gotUserFriends(userId, RpcUtils::Parser::ParseFriends(doc));
    emit requestFinished();
}

void GoodReadsApi::handleGetUserFollowers(const QString& userId)
{
    bool ok = false;
    auto doc = GetDocumentFromReply(sender(), ok);
    if (!ok) {
        emit requestFinished();
        return;
    }

    emit gotUserFollowers(userId, RpcUtils::Parser::ParseFollowers(doc));
    emit requestFinished();
}

void GoodReadsApi::handleGetUserFollowings(const QString& userId)
{
    bool ok = false;
    auto doc = GetDocumentFromReply(sender(), ok);
    if (!ok) {
        emit requestFinished();
        return;
    }

    emit gotUserFollowings(userId, RpcUtils::Parser::ParseFollowings(doc));
    emit requestFinished();
}

void GoodReadsApi::handleGetFriendRequests()
{
    bool ok = false;
    auto doc = GetDocumentFromReply(sender(), ok);
    if (!ok) {
        emit requestFinished();
        return;
    }

    emit gotFriendsRequests(RpcUtils::Parser::ParseFriendsRequests(doc));
    emit gotFriendsRecommendations(RpcUtils::Parser::ParseFriendsRecommendations(doc));
    emit requestFinished();
}

void GoodReadsApi::handleConfirmFriendRequest(quint64 friendRequestId, bool confirm)
{
    bool ok = false;
    auto doc = GetReply(sender(), ok);
    if (!ok) {
        emit requestFinished();
        return;
    }

    if (doc.isEmpty()) {
        emit friendRequestConfirmed(friendRequestId, confirm);
    }
    emit requestFinished();
}

void GoodReadsApi::handleConfirmFriendRecommendation(quint64 friendRecommendationId, bool confirm)
{
    bool ok = false;
    auto doc = GetReply(sender(), ok);
    if (!ok) {
        emit requestFinished();
        return;
    }

    if (doc.isEmpty()) {
        emit friendRecommendationConfirmed(friendRecommendationId, confirm);
    }
    emit requestFinished();
}

void GoodReadsApi::handleAddFriend(const QString& userId)
{
    bool ok = false;
    auto doc = GetDocumentFromReply(sender(), ok);
    if (!ok) {
        emit requestFinished();
        return;
    }

    if (doc.isNull()) {
        emit friendAdded(userId);
    }
    emit requestFinished();
}

void GoodReadsApi::handleRemoveFriend(const QString& userId)
{
    bool ok = false;
    auto doc = GetDocumentFromReply(sender(), ok);
    if (!ok) {
        emit requestFinished();
        return;
    }

    if (doc.isNull()) {
        emit friendRemoved(userId);
    }
    emit requestFinished();
}

void GoodReadsApi::handleFollowUser()
{
    bool ok = false;
    auto doc = GetDocumentFromReply(sender(), ok);
    if (!ok) {
        emit requestFinished();
        return;
    }

    QXmlQuery query;
    query.setFocus(doc.toByteArray());
    const QString idStr(GetQueryResult(query, "/user-following/id/text()"));
    const QString userIdStr(GetQueryResult(query, "/user-following/user-id/text()"));
    emit userFollowed(userIdStr , !idStr.isEmpty());
    emit requestFinished();
}

void GoodReadsApi::handleUnfollowUser(const QString& userId)
{
    bool ok = false;
    auto doc = GetDocumentFromReply(sender(), ok);
    if (!ok) {
        emit requestFinished();
        return;
    }

    QXmlQuery query;
    query.setFocus(doc.toByteArray());
    const QString resultStr(GetQueryResult(query, "/hash/status/text()"));
    emit userUnfollowed(userId, resultStr == "success");
    emit requestFinished();
}

void GoodReadsApi::handleGotUserQuotes(const QString& userId)
{
    bool ok = false;
    auto doc = GetDocumentFromReply(sender(), ok);
    if (!ok) {
        emit requestFinished();
        return;
    }

    emit gotUserQuotes(userId, RpcUtils::Parser::ParseUserQuotes(doc));
    emit requestFinished();
}

void GoodReadsApi::handleGotBookQuotes(quint64 workId)
{
    bool ok = false;
    auto doc = GetJsonDocumentFromReply(sender(), ok);
    if (!ok) {
        emit requestFinished();
        return;
    }

    emit gotBookQuotes(workId, RpcUtils::Parser::ParseJsonQuotes(doc));
    emit requestFinished();
}

void GoodReadsApi::handleGotAuthorQuotes(const QString& authorId)
{
    bool ok = false;
    auto doc = GetJsonDocumentFromReply(sender(), ok);
    if (!ok) {
        emit requestFinished();
        return;
    }

    emit gotAuthorQuotes(authorId, RpcUtils::Parser::ParseJsonQuotes(doc));
    emit requestFinished();
}

void GoodReadsApi::handleAddQuote()
{
    bool ok = false;
    auto doc = GetDocumentFromReply(sender(), ok);
    if (!ok) {
        emit requestFinished();
        return;
    }

    //TODO
    qDebug() << doc.toByteArray();
    emit requestFinished();
}

void GoodReadsApi::handleGetReadStatus()
{
    bool ok = false;
    auto doc = GetDocumentFromReply(sender(), ok);
    if (!ok) {
        emit requestFinished();
        return;
    }

    emit gotReadStatus(RpcUtils::Parser::ParseReadStatus(doc));
    emit requestFinished();
}

void GoodReadsApi::handleGetRecentUserStatuses()
{
    bool ok = false;
    auto doc = GetDocumentFromReply(sender(), ok);
    if (!ok) {
        emit requestFinished();
        return;
    }

    //TODO
    qDebug() << doc.toByteArray();
    emit requestFinished();
}

void GoodReadsApi::handleGetUserStatus()
{
    bool ok = false;
    auto doc = GetDocumentFromReply(sender(), ok);
    if (!ok) {
        emit requestFinished();
        return;
    }

    emit gotUserStatus(RpcUtils::Parser::ParseUserStatus(doc));
    emit requestFinished();
}

void GoodReadsApi::handleUpdateReadingProgress()
{
    bool ok = false;
    auto doc = GetDocumentFromReply(sender(), ok);
    if (!ok) {
        emit requestFinished();
        return;
    }

    //TODO
    emit requestFinished();
}

void GoodReadsApi::handleDeleteUserStatus()
{
    bool ok = false;
    auto doc = GetDocumentFromReply(sender(), ok);
    if (!ok) {
        emit requestFinished();
        return;
    }

    //TODO
    qDebug() << doc.toByteArray();
    emit requestFinished();
}

void GoodReadsApi::handleGetRecommendation()
{
    bool ok = false;
    auto doc = GetDocumentFromReply(sender(), ok);
    if (!ok) {
        emit requestFinished();
        return;
    }

    emit gotRecommendation(RpcUtils::Parser::ParseRecommendation(doc));
    emit requestFinished();
}

void GoodReadsApi::handleLikeResource(const QString& resourceId)
{
    bool ok = false;
    auto doc = GetDocumentFromReply(sender(), ok);
    if (!ok) {
        emit requestFinished();
        return;
    }

    QXmlQuery query;
    query.setFocus(doc.toByteArray());
    const QString ratingId(GetQueryResult(query, "/GoodreadsResponse/rating/id/text()"));
    emit likeAdded(resourceId, ratingId.toULongLong());
    emit requestFinished();
}

void GoodReadsApi::handleUnlikeResource(const QString& resourceId)
{
    bool ok = false;
    auto doc = GetReply(sender(), ok);
    if (!ok) {
        emit requestFinished();
        return;
    }

    if (doc.isEmpty()) {
        emit likeRemoved(resourceId);
    }
    emit requestFinished();
}
}
