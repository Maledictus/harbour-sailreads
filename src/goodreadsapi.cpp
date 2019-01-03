/*
The MIT License (MIT)

Copyright (c) 2018 Oleg Linkin <maledictusdemagog@gmail.com>

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
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QRegularExpression>
#include <QUrlQuery>
#include <QXmlQuery>

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
}

void GoodReadsApi::AbortRequest()
{
    if (m_CurrentReply) {
        m_CurrentReply->abort();
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

void GoodReadsApi::GetUserInfo(quint64 id)
{
    QNetworkReply *reply = m_OAuth1->Get(m_AccessToken, m_AccessTokenSecret,
            QUrl(m_BaseUrl + QString("/user/show/%1.xml").arg(id)));
    m_CurrentReply = reply;
    connect(reply, &QNetworkReply::finished,
             this, &GoodReadsApi::handleGetUserInfo);
}

void GoodReadsApi::CompareBooks(quint64 userId)
{
//    //TODO make simple request
//    const auto& pair = m_OAuthWrapper->MakeSignedUrl(m_AccessToken, m_AccessTokenSecret,
//            QUrl(QString("https://www.goodreads.com/user/compare/%1.xml").arg(userId)), "GET");
//    auto reply = m_NAM->get(QNetworkRequest(pair.first));
//    connect(reply, &QNetworkReply::finished,
//             this, &GoodReadsApi::handleCompareBooks);
}

void GoodReadsApi::GetUpdates()
{
//    const auto& pair = m_OAuthWrapper->MakeSignedUrl(m_AccessToken, m_AccessTokenSecret,
//            QUrl("https://www.goodreads.com/updates/friends.xml"), "GET");
//    auto reply = m_NAM->get(QNetworkRequest(pair.first));
//    m_CurrentReply = reply;
//    connect(reply, &QNetworkReply::finished,
//             this, &GoodReadsApi::handleGetUpdates);
}

void GoodReadsApi::GetNotifications(int page)
{
//    const auto& pair = m_OAuthWrapper->MakeSignedUrl(m_AccessToken, m_AccessTokenSecret,
//            QUrl(QString("https://www.goodreads.com/notifications.xml?page=%1").arg(page)), "GET");
//    auto reply = m_NAM->get(QNetworkRequest(pair.first));
//    m_CurrentReply = reply;
//    connect(reply, &QNetworkReply::finished,
//             this, &GoodReadsApi::handleGetNotifications);
}

void GoodReadsApi::GetBookShelves(quint64 userId, int page)
{
    QNetworkReply *reply = m_OAuth1->Get(m_AccessToken, m_AccessTokenSecret,
            QUrl(m_BaseUrl + "/shelf/list.xml"), { { "user_id", userId }, { "page", page } });
    m_CurrentReply = reply;
    connect(reply, &QNetworkReply::finished,
            this, [this, userId]() { handleGetBookShelves(userId); });
}

void GoodReadsApi::AddBookShelf(const QString& name, bool exclusive, bool sortable, bool featured,
        bool recommendFor)
{
    const QVariantMap params = { { "user_shelf[name]", name },
        { "user_shelf[exclusive_flag]", (exclusive ? "true" : "false") },
//        { "user_shelf[sortable_flag]", (sortable ? "true" : "false") },
//        { "user_shelf[featured]", (featured ? "true" : "false") },
        { "user_shelf[recommend_for]", (recommendFor ? "true" : "false") } };
    QNetworkReply *reply = m_OAuth1->Post(m_AccessToken, m_AccessTokenSecret,
            QUrl(m_BaseUrl + "/user_shelves.xml"), params);
    connect(reply, &QNetworkReply::finished,
            this, &GoodReadsApi::handleAddBookShelf);
}

void GoodReadsApi::EditBookShelf(quint64 id, const QString& name, bool exclusive,
        bool sortable, bool featured, bool recommendFor)
{
    const QVariantMap params = { { "user_shelf[name]", name },
        { "user_shelf[exclusive_flag]", (exclusive ? "true" : "false") },
//        { "user_shelf[sortable_flag]", (sortable ? "true" : "false") },
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

void GoodReadsApi::GetReviews(quint64 userId, const QString& bookShelf, const QString& sortField,
        Qt::SortOrder order, int page)
{
    const QVariantMap params = { { "v", 2 }, { "shelf", bookShelf }, { "page", page },
        { "per_page", 50 }, { "sort", sortField },
        { "order", (order == Qt::AscendingOrder ? "a" : "d") }
    };
    auto reply = m_OAuth1->Get(m_AccessToken, m_AccessTokenSecret,
            QUrl(m_BaseUrl + QString("/review/list/%1.xml").arg(userId)), params);
    m_CurrentReply = reply;
    connect(reply, &QNetworkReply::finished,
            this, &GoodReadsApi::handleGetReviews);
}

void GoodReadsApi::GetReview(quint64 reviewId, int commentsPage)
{
    auto reply = m_OAuth1->Get(m_AccessToken, m_AccessTokenSecret,
            QUrl(m_BaseUrl + "/review/show.xml"), { { "id", reviewId }, { "page", commentsPage } });
    connect(reply, &QNetworkReply::finished,
            this, &GoodReadsApi::handleGetReview);
}

void GoodReadsApi::AddReview(quint64 bookId, const QString& review, int rating,
    const QDateTime& readAt, const QString& shelf)
{
//    QString url = QString("https://www.goodreads.com/review.xml?book_id=%1&review[review]=%2")
//            .arg(bookId).arg(review);
//    if (rating > 0) {
//        url.append("&review[rating]=" + QString::number(rating));
//    }
//    if (readAt.isValid()) {
//        url.append("&review[read_at]=" + readAt.toString("yyyy-MM-dd"));
//    }
//    if (!shelf.isEmpty()) {
//        url.append("&shelf=" + shelf);
//    }
//    const auto& pair = m_OAuthWrapper->MakeSignedUrl(m_AccessToken, m_AccessTokenSecret,
//            QUrl(url), "POST");
//    auto reply = m_NAM->post(PreparePostRequest(pair.first), pair.second);
//    connect(reply, &QNetworkReply::finished,
//            this, &GoodReadsApi::handleAddReview);
}

void GoodReadsApi::EditReview(quint64 reviewId, const QString& review, int rating,
    const QDateTime& readAt, bool finished, const QString& shelf)
{
//    QString url = QString("https://www.goodreads.com/review/%1.xml?review[review]=%2")
//            .arg(reviewId).arg(review);
//    if (rating > 0) {
//        url.append("&review[rating]=" + QString::number(rating));
//    }
//    if (readAt.isValid()) {
//        url.append("&review[read_at]=" + readAt.toString("yyyy-MM-dd"));
//    }
//    if (finished) {
//        url.append("&finished=true");
//    }
//    if (!shelf.isEmpty()) {
//        url.append("&shelf=" + shelf);
//    }
//    const auto& pair = m_OAuthWrapper->MakeSignedUrl(m_AccessToken, m_AccessTokenSecret,
//            QUrl(url), "PUT");
//    auto reply = m_NAM->put(QNetworkRequest(pair.first), pair.second);
//    connect(reply, &QNetworkReply::finished,
//            this, &GoodReadsApi::handleEditReview);
}

void GoodReadsApi::DeleteReview(quint64 reviewId)
{
//    const auto& pair = m_OAuthWrapper->MakeSignedUrl(m_AccessToken, m_AccessTokenSecret,
//            QUrl(QString("https://www.goodreads.com/review/destroy/%1?format=xml").arg(reviewId)),
//                    "DELETE");
//    auto reply = m_NAM->deleteResource(QNetworkRequest(pair.first));
//    connect(reply, &QNetworkReply::finished,
//            this, &GoodReadsApi::handleDeleteReview);
}

void GoodReadsApi::GetBook(quint64 bookId)
{
    auto reply = m_OAuth1->Get(m_AccessToken, m_AccessTokenSecret,
            QUrl(m_BaseUrl + "/book/show.xml"), { { "id", bookId } });
    m_CurrentReply = reply;
    connect(reply, &QNetworkReply::finished,
            this, &GoodReadsApi::handleGetBook);
}

void GoodReadsApi::SearchBooks(const QString& query, const QString& key, int page)
{
//    //TODO make simple request
//    const auto& pair = m_OAuthWrapper->MakeSignedUrl(m_AccessToken, m_AccessTokenSecret,
//            QUrl(QString("https://www.goodreads.com/search/index.xml?q=%1&search[field]=%2&page=%3")
//                 .arg(query).arg(key).arg(page)), "GET");
//    auto reply = m_NAM->get(QNetworkRequest(pair.first));
//    m_CurrentReply = reply;
//    connect(reply, &QNetworkReply::finished,
//            this, &GoodReadsApi::handleSearchBook);
}

void GoodReadsApi::GetSeries(quint64 seriesId)
{
    auto reply = m_OAuth1->Get(m_AccessToken, m_AccessTokenSecret,
            QUrl(m_BaseUrl + QString("/series/show/%1.xml").arg(seriesId)));
    m_CurrentReply = reply;
    connect(reply, &QNetworkReply::finished,
            this, &GoodReadsApi::handleGetSeries);
}

void GoodReadsApi::GetAuthorSeries(quint64 authorId)
{
//    //TODO make simple request
//    const auto& pair = m_OAuthWrapper->MakeSignedUrl(m_AccessToken, m_AccessTokenSecret,
//            QUrl(QString("https://www.goodreads.com/list/%1.xml").arg(authorId)), "GET");
//    auto reply = m_NAM->get(QNetworkRequest(pair.first));
//    m_CurrentReply = reply;
//    connect(reply, &QNetworkReply::finished,
//            this, &GoodReadsApi::handleGetAuthorSeries);
}

void GoodReadsApi::GetWorkSeries(quint64 workId)
{
//    //TODO make simple request
//    const auto& pair = m_OAuthWrapper->MakeSignedUrl(m_AccessToken, m_AccessTokenSecret,
//            QUrl(QString("https://www.goodreads.com/work/%1/series?format=xml").arg(workId)), "GET");
//    auto reply = m_NAM->get(QNetworkRequest(pair.first));
//    m_CurrentReply = reply;
//    connect(reply, &QNetworkReply::finished,
//            this, &GoodReadsApi::handleGetWorkSeries);
}

void GoodReadsApi::AddBookToShelf(quint64 bookId, const QString& shelfName)
{
//    const auto& pair = m_OAuthWrapper->MakeSignedUrl(m_AccessToken, m_AccessTokenSecret,
//            QUrl(QString("https://www.goodreads.com/shelf/add_to_shelf.xml?book_id=%1&name=%2")
//                 .arg(bookId).arg(shelfName)), "POST");
//    auto reply = m_NAM->post(PreparePostRequest(pair.first), pair.second);
//    connect(reply, &QNetworkReply::finished,
//            this, &GoodReadsApi::handleAddBookToShelf);
}

void GoodReadsApi::AddBooksToShelves(const QList<quint64>& bookIds, const QStringList& shelvesName)
{
//    QStringList bookIdsStringList;
//    std::transform(bookIds.begin(), bookIds.end(),
//            std::back_inserter(bookIdsStringList),
//            [](decltype(bookIds.front()) bookId)
//            { return QString::number(bookId); });

//    const auto& pair = m_OAuthWrapper->MakeSignedUrl(m_AccessToken, m_AccessTokenSecret,
//            QUrl(QString("https://www.goodreads.com/shelf/add_books_to_shelves.xml?bookids=%1&shelves=%2")
//                 .arg(bookIdsStringList.join(",")).arg(shelvesName.join(","))), "POST");
//    auto reply = m_NAM->post(PreparePostRequest(pair.first), pair.second);
//    connect(reply, &QNetworkReply::finished,
//            this, &GoodReadsApi::handleAddBooksToShelves);
}

void GoodReadsApi::RemoveBookFromShelf(quint64 bookId, const QString& shelfName)
{
//    const auto& pair = m_OAuthWrapper->MakeSignedUrl(m_AccessToken, m_AccessTokenSecret,
//            QUrl(QString("https://www.goodreads.com/shelf/add_to_shelf.xml?book_id=%1&name=%2&a=remove")
//                 .arg(bookId).arg(shelfName)), "POST");
//    auto reply = m_NAM->post(PreparePostRequest(pair.first), pair.second);
//    connect(reply, &QNetworkReply::finished,
//            this, &GoodReadsApi::handleRemoveBookFromShelf);
}

void GoodReadsApi::GetAuthor(quint64 authorId)
{
    auto reply = m_OAuth1->Get(m_AccessToken, m_AccessTokenSecret,
            QUrl(m_BaseUrl + "/author/show.xml"), { { "id", authorId } });
    m_CurrentReply = reply;
    connect(reply, &QNetworkReply::finished,
            this, &GoodReadsApi::handleGetAuthor);
}

void GoodReadsApi::GetAuthorBooks(quint64 authorId, int page)
{
    auto reply = m_OAuth1->Get(m_AccessToken, m_AccessTokenSecret,
            QUrl(m_BaseUrl + "/author/list.xml"), { { "id", authorId }, { "page", page } });
    m_CurrentReply = reply;
    connect(reply, &QNetworkReply::finished,
            this, [this, authorId]() {
                handleGetAuthorBooks(authorId);
            });
}

void GoodReadsApi::FollowAuthor(quint64 authorId)
{
    auto reply = m_OAuth1->Post(m_AccessToken, m_AccessTokenSecret,
            QUrl(m_BaseUrl + QString("/author_followings?id=%1&format=%2").arg(authorId).arg("xml")));
    connect(reply, &QNetworkReply::finished,
            this, &GoodReadsApi::handleFollowAuthor);
}

void GoodReadsApi::UnfollowAuthor(quint64 authorId, quint64 authorFollowingId)
{
    auto reply = m_OAuth1->DeleteResource(m_AccessToken, m_AccessTokenSecret,
            QUrl(m_BaseUrl + QString("/author_followings/%1?format=xml").arg(authorFollowingId)));
    connect(reply, &QNetworkReply::finished,
            this, [this, authorId]() {
                handleUnfollowAuthor(authorId);
            });
}

void GoodReadsApi::GetGroups(quint64 userId, int page)
{
    auto reply = m_OAuth1->Get(m_AccessToken, m_AccessTokenSecret,
            QUrl(m_BaseUrl + QString("/group/list/%1.xml").arg(userId)),
            { { "sort", "last_activity" }, { "page", page } });
    m_CurrentReply = reply;
    connect(reply, &QNetworkReply::finished,
            this, [this, userId]() { handleGetGroups(userId); });
}

void GoodReadsApi::GetGroup(quint64 groupId)
{
    auto reply = m_OAuth1->Get(m_AccessToken, m_AccessTokenSecret,
            QUrl(m_BaseUrl + QString("/group/show/%1.xml").arg(groupId)),
            { { "sort", "updated_at" } });
    m_CurrentReply = reply;
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

void GoodReadsApi::SearchGroup(const QString& text, int page)
{
    auto reply = m_OAuth1->Get(m_AccessToken, m_AccessTokenSecret,
            QUrl(m_BaseUrl + "/group/search.xml"),
            { { "q", QUrl::toPercentEncoding(text) }, { "page", page } });
    m_CurrentReply = reply;
    connect(reply, &QNetworkReply::finished,
            this, &GoodReadsApi::handleSearchGroup);
}

void GoodReadsApi::GetGroupMembers(quint64 groupId, int page)
{
    auto reply = m_OAuth1->Get(m_AccessToken, m_AccessTokenSecret,
            QUrl(m_BaseUrl + QString("/group/members/%1.xml").arg(groupId)),
            { { "page", page }, { "sort", "num_comment" } });
    m_CurrentReply = reply;
    connect(reply, &QNetworkReply::finished,
            this, [this, groupId] { handleGetGroupMembers(groupId); });
}

void GoodReadsApi::GetGroupFolderTopics(const QString& groupFolderId, quint64 groupId, int page)
{
    QNetworkReply *reply = nullptr;
    const QVariantMap params = { { "format", "xml" }, { "page", page },
            { "sort", "updated_at" }, { "order", "d" } };
    if (groupFolderId == GoodReadsApi::UnreadTopicsFolderId) {
        reply = m_OAuth1->Get(m_AccessToken, m_AccessTokenSecret,
                QUrl(m_BaseUrl + QString("/topic/unread_group/%1").arg(groupId)), params);
    }
    else {
        reply = m_OAuth1->Get(m_AccessToken, m_AccessTokenSecret,
                QUrl(m_BaseUrl + QString("/topic/group_folder/%1").arg(groupFolderId)), params);
    }
    m_CurrentReply = reply;
    connect(reply, &QNetworkReply::finished,
            this,
            [this, groupFolderId, groupId] {
                handleGetGroupFolderTopics(groupFolderId, groupId);
            });
}

void GoodReadsApi::GetGroupFolderTopic(quint64 topicId, int page)
{
    auto reply = m_OAuth1->Get(m_AccessToken, m_AccessTokenSecret,
            QUrl(m_BaseUrl + "/topic/show.xml"), { { "id", topicId }, { "page", page } });
    m_CurrentReply = reply;
    connect(reply, &QNetworkReply::finished,
            this, &GoodReadsApi::handleGetGroupFolderTopic);
}

void GoodReadsApi::AddNewTopic(const QString& topic, const QString& subject, quint64 subjectId,
        const QString& folderId, bool question, bool updateFeed, bool digest, const QString& comment)
{
    const QVariantMap params = { { "topic[subject_type]", subject },
            { "topic[subject_id]", subjectId }, { "topic[folder_id]", folderId },
            { "topic[title]", topic }, { "topic[question_flag]", (question ? "1" : "0") },
            { "update_feed", (updateFeed ? "1" : "0") }, { "digest", (digest ? "1" : "0") },
            { "comment[body_usertext]", comment } };
    auto reply = m_OAuth1->Post(m_AccessToken, m_AccessTokenSecret,
            QUrl(m_BaseUrl + "/topic.xml"), params);
    connect(reply, &QNetworkReply::finished,
             this, &GoodReadsApi::handleTopicAdded);
}

void GoodReadsApi::AddNewComment(const QString& type, quint64 resourceId, const QString& comment)
{
    const QVariantMap params = { { "type", type }, { "id", resourceId },
            { "comment[body]", comment } };
    auto reply = m_OAuth1->Post(m_AccessToken, m_AccessTokenSecret,
            QUrl(m_BaseUrl + "/comment.xml"), params);
    connect(reply, &QNetworkReply::finished,
             this, &GoodReadsApi::handleNewCommentAdded);
}

void GoodReadsApi::GetFriends(quint64 userId, int page)
{
    auto reply = m_OAuth1->Get(m_AccessToken, m_AccessTokenSecret,
            QUrl(m_BaseUrl + QString("/friend/user/%1").arg(userId)),
            { { "format", "xml" }, { "page", page } });
    m_CurrentReply = reply;
    connect(reply, &QNetworkReply::finished,
             this, [this, userId]() { handleGetFriends(userId); });
}

void GoodReadsApi::GetUserFollowers(quint64 userId, int page)
{
    auto reply = m_OAuth1->Get(m_AccessToken, m_AccessTokenSecret,
            QUrl(m_BaseUrl + QString("/user/%1/followers.xml").arg(userId)), { { "page", page } } );
    connect(reply, &QNetworkReply::finished,
             this, [this, userId]() { handleGetUserFollowers(userId); });
}

void GoodReadsApi::GetUserFollowings(quint64 userId, int page)
{
    auto reply = m_OAuth1->Get(m_AccessToken, m_AccessTokenSecret,
            QUrl(m_BaseUrl + QString("/user/%1/following.xml").arg(userId)), { { "page", page } } );
    connect(reply, &QNetworkReply::finished,
             this, [this, userId]() { handleGetUserFollowings(userId); });
}

void GoodReadsApi::GetFriendRequests(int page)
{
    auto reply = m_OAuth1->Get(m_AccessToken, m_AccessTokenSecret,
            QUrl(m_BaseUrl + "/friend/requests.xml"), { { "page", page } });
    m_CurrentReply = reply;
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

void GoodReadsApi::ConfirmFriendRecommendation(quint64 friendRecommendationId)
{
//    const auto& pair = m_OAuthWrapper->MakeSignedUrl(m_AccessToken, m_AccessTokenSecret,
//            QUrl(QString("https://www.goodreads.com/friend/confirm_recommendation.xml?id=%1&response=%2")
//                 .arg(friendRecommendationId).arg("Y")), "POST");
//    auto reply = m_NAM->post(PreparePostRequest(pair.first), pair.second);
//    connect(reply, &QNetworkReply::finished,
//             this, &GoodReadsApi::handleConfirmFriendRecommendation);
}

void GoodReadsApi::DeclineFriendRecommendation(quint64 friendRecommendationId)
{
//    const auto& pair = m_OAuthWrapper->MakeSignedUrl(m_AccessToken, m_AccessTokenSecret,
//            QUrl(QString("https://www.goodreads.com/friend/confirm_recommendation.xml?id=%1&response=%2")
//                 .arg(friendRecommendationId).arg("N")), "POST");
//    auto reply = m_NAM->post(PreparePostRequest(pair.first), pair.second);
//    connect(reply, &QNetworkReply::finished,
//            this, &GoodReadsApi::handleDeclineFriendRecommendation);
}

void GoodReadsApi::AddFriend(quint64 userId)
{
    auto reply = m_OAuth1->Post(m_AccessToken, m_AccessTokenSecret,
            QUrl(m_BaseUrl + "/friend/add_as_friend.xml"), { { "id", userId } });
    connect(reply, &QNetworkReply::finished,
            this, [this, userId]() { handleAddFriend(userId); });
}

void GoodReadsApi::RemoveFriend(quint64 userId)
{
//    auto reply = m_OAuth1->Post(m_AccessToken, m_AccessTokenSecret,
//            QUrl(m_BaseUrl + QString("/friend/destroy/%1").arg(userId)),
//                { { "format", "xml" } });
//    connect(reply, &QNetworkReply::finished,
//            this, [this, userId]() { handleRemoveFriend(userId); });
}

void GoodReadsApi::FollowUser(quint64 userId)
{
    auto reply = m_OAuth1->Post(m_AccessToken, m_AccessTokenSecret,
            QUrl(m_BaseUrl + QString("/user/%1/followers").arg(userId)), { { "format", "xml" } });
    connect(reply, &QNetworkReply::finished,
            this, &GoodReadsApi::handleFollowUser);
}

void GoodReadsApi::UnfollowUser(quint64 userId)
{
    auto reply = m_OAuth1->DeleteResource(m_AccessToken, m_AccessTokenSecret,
            QUrl(m_BaseUrl + QString("/user/%1/followers/stop_following?format=xml").arg(userId)));
    connect(reply, &QNetworkReply::finished,
            this, [this, userId]() { handleUnfollowUser(userId); });
}

void GoodReadsApi::AddQuote(const QString& authorName, quint64 authorId, quint64 bookId,
    const QString& quote, const QStringList& tags)
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

void GoodReadsApi::GetReadStatus(quint64 readStatusId)
{
//    const auto& pair = m_OAuthWrapper->MakeSignedUrl(m_AccessToken, m_AccessTokenSecret,
//            QUrl(QString("https://www.goodreads.com/read_statuses/%1?format=xml").arg(readStatusId)));
//    auto reply = m_NAM->get(QNetworkRequest(pair.first));
//    connect(reply, &QNetworkReply::finished,
//            this, &GoodReadsApi::handleGetReadStatus);
}

void GoodReadsApi::GetRecentUserStatuses()
{
//    const auto& pair = m_OAuthWrapper->MakeSignedUrl(m_AccessToken, m_AccessTokenSecret,
//            QUrl(QString("https://www.goodreads.com/user_status/index.xml")));
//    auto reply = m_NAM->get(QNetworkRequest(pair.first));
//    connect(reply, &QNetworkReply::finished,
//            this, &GoodReadsApi::handleGetRecentUserStatuses);
}

void GoodReadsApi::GetUserStatus(quint64 userStatusId)
{
//    const auto& pair = m_OAuthWrapper->MakeSignedUrl(m_AccessToken, m_AccessTokenSecret,
//            QUrl(QString("https://www.goodreads.com/user_status/show/%1?format=xml")
//                 .arg(userStatusId)));
//    auto reply = m_NAM->get(QNetworkRequest(pair.first));
//    connect(reply, &QNetworkReply::finished,
//            this, &GoodReadsApi::handleGetUserStatus);
}

void GoodReadsApi::UpdateUserStatus(quint64 bookId, const QString& body, int percent, int page)
{
//    QString url(QString("https://www.goodreads.com/user_status.xml?user_status[book_id]=%1&"
//                "user_status[body]=%2").arg(bookId).arg(body));
//    if(page > 0) {
//        url.append("&user_status[page]=" + QString::number(page));
//    }
//    else if (percent > 0) {
//        url.append("&user_status[percent]=" + QString::number(percent));
//    }
//    const auto& pair = m_OAuthWrapper->MakeSignedUrl(m_AccessToken, m_AccessTokenSecret,
//            QUrl(url), "POST");
//    auto reply = m_NAM->post(PreparePostRequest(pair.first), pair.second);
//    connect(reply, &QNetworkReply::finished,
//             this, &GoodReadsApi::handleUpdateUserStatus);
}

void GoodReadsApi::DeleteUserStatus(quint64 userStatusId)
{
//    const auto& pair = m_OAuthWrapper->MakeSignedUrl(m_AccessToken, m_AccessTokenSecret,
//            QUrl(QString("https://www.goodreads.com/user_status/destroy/%1?format=xml")
//                    .arg(userStatusId)), "POST");
//    auto reply = m_NAM->post(PreparePostRequest(pair.first), pair.second);
//    connect(reply, &QNetworkReply::finished,
//             this, &GoodReadsApi::handleDeleteUserStatus);
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

QDomDocument ParseDocument(const QByteArray& data, bool& ok)
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
    }
    else
    {
        ok = true;
    }
    return document;
}

QUrl GetRedirectedUrl(const QDomDocument& doc)
{
    QXmlQuery query;
    query.setFocus(doc.toByteArray());
    const QString url(GetQueryResult(query, "/html/body/a/@href/data(.)"));
    return QUrl(url);
}
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
    m_CurrentReply.clear();

    if (reply->error() != QNetworkReply::NoError &&
            reply->error() != QNetworkReply::OperationCanceledError &&
            reply->error() != QNetworkReply::AuthenticationRequiredError) {
        qWarning() << Q_FUNC_INFO << "There is network error: "
                << reply->error() << reply->errorString();
        ok = false;
        return data;
    }

    //Disable warning on request cancel
    if (reply->error() == QNetworkReply::OperationCanceledError) {
        ok = true;
        return data;
    }

    if (reply->error() == QNetworkReply::AuthenticationRequiredError) {
        ok = false;
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
    emit requestFinished();
    emit requestTokenChanged(m_RequestToken);
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

    emit requestFinished();
    emit accessTokensChanged(accessToken, accessTokenSecret);
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
    emit gotAuthUserInfo(id.toLongLong(), name, link);
}

void GoodReadsApi::handleGetUserInfo()
{
    bool ok = false;
    auto doc = GetDocumentFromReply(sender(), ok);
    if (!ok) {
        emit requestFinished();
        return;
    }

    emit requestFinished();
    emit gotUserProfile(RpcUtils::Parser::ParseUser(doc));
}

void GoodReadsApi::handleCompareBooks()
{
    bool ok = false;
    auto doc = GetDocumentFromReply(sender(), ok);
    if (!ok) {
        emit requestFinished();
        return;
    }

    emit requestFinished();
    //TODO
    qDebug() << doc.toByteArray();
}

void GoodReadsApi::handleGetUpdates()
{
    bool ok = false;
    auto doc = GetDocumentFromReply(sender(), ok);
    if (!ok) {
        emit requestFinished();
        return;
    }

    emit requestFinished();
    //TODO
    qDebug() << doc.toByteArray();
}

void GoodReadsApi::handleGetNotifications()
{
    bool ok = false;
    auto doc = GetDocumentFromReply(sender(), ok);
    if (!ok) {
        emit requestFinished();
        return;
    }

    emit requestFinished();
    //TODO
    qDebug() << doc.toByteArray();
}

void GoodReadsApi::handleGetBookShelves(quint64 userId)
{
    bool ok = false;
    auto doc = GetDocumentFromReply(sender(), ok);
    if (!ok) {
        emit requestFinished();
        return;
    }

    emit requestFinished();
    emit gotUserBookShelves(userId, RpcUtils::Parser::ParseBookShelves(doc));
}

void GoodReadsApi::handleAddBookShelf()
{
    bool ok = false;
    auto doc = GetDocumentFromReply(sender(), ok);
    if (!ok) {
        emit requestFinished();
        return;
    }

    emit requestFinished();
    emit bookShelfAdded(RpcUtils::Parser::ParseBookShelf(doc.firstChildElement("user_shelf")));
}

void GoodReadsApi::handleEditBookShelf()
{
    bool ok = false;
    auto doc = GetDocumentFromReply(sender(), ok);
    if (!ok) {
        emit requestFinished();
        return;
    }

    emit requestFinished();
    emit bookShelfEdited(RpcUtils::Parser::ParseBookShelf(doc.firstChildElement("user_shelf")));
}

void GoodReadsApi::handleGetReviews()
{
    bool ok = false;
    auto doc = GetDocumentFromReply(sender(), ok);
    if (!ok) {
        emit requestFinished();
        return;
    }

    emit requestFinished();
    const auto& pair = RpcUtils::Parser::ParseReviews(doc);
    emit gotReviews(pair.first, pair.second);
}

void GoodReadsApi::handleGetReview()
{
    bool ok = false;
    auto doc = GetDocumentFromReply(sender(), ok);
    if (!ok) {
        emit requestFinished();
        return;
    }

    emit requestFinished();
    emit gotReview(RpcUtils::Parser::ParseReview(doc));
}

void GoodReadsApi::handleAddReview()
{
    bool ok = false;
    auto doc = GetDocumentFromReply(sender(), ok);
    if (!ok) {
        emit requestFinished();
        return;
    }

    emit requestFinished();
    //TODO
    qDebug() << doc.toByteArray();
}

void GoodReadsApi::handleEditReview()
{
    bool ok = false;
    auto doc = GetDocumentFromReply(sender(), ok);
    if (!ok) {
        emit requestFinished();
        return;
    }

    emit requestFinished();
    //TODO
    qDebug() << doc.toByteArray();
}

void GoodReadsApi::handleDeleteReview()
{
    bool ok = false;
    auto doc = GetDocumentFromReply(sender(), ok);
    if (!ok) {
        emit requestFinished();
        return;
    }

    emit requestFinished();
    //TODO
    qDebug() << doc.toByteArray();
}

void GoodReadsApi::handleGetBook()
{
    bool ok = false;
    auto doc = GetDocumentFromReply(sender(), ok);
    if (!ok) {
        emit requestFinished();
        return;
    }

    emit requestFinished();
    emit gotBook(RpcUtils::Parser::ParseBook(doc));
}

void GoodReadsApi::handleSearchBook()
{
    bool ok = false;
    auto doc = GetDocumentFromReply(sender(), ok);
    if (!ok) {
        emit requestFinished();
        return;
    }

    emit requestFinished();
    //TODO
    qDebug() << doc.toByteArray();
}

void GoodReadsApi::handleGetSeries()
{
    bool ok = false;
    auto doc = GetDocumentFromReply(sender(), ok);
    if (!ok) {
        emit requestFinished();
        return;
    }

    emit requestFinished();
    qDebug() << doc.toByteArray();
    emit gotSeries(RpcUtils::Parser::ParseSeries(doc));
}

void GoodReadsApi::handleGetAuthorSeries()
{
    bool ok = false;
    auto doc = GetDocumentFromReply(sender(), ok);
    if (!ok) {
        emit requestFinished();
        return;
    }

    emit requestFinished();
    //TODO
    qDebug() << doc.toByteArray();
}

void GoodReadsApi::handleGetWorkSeries()
{
    bool ok = false;
    auto doc = GetDocumentFromReply(sender(), ok);
    if (!ok) {
        emit requestFinished();
        return;
    }

    emit requestFinished();
    //TODO
    qDebug() << doc.toByteArray();
}

void GoodReadsApi::handleAddBookToShelf()
{
    bool ok = false;
    auto doc = GetDocumentFromReply(sender(), ok);
    if (!ok) {
        emit requestFinished();
        return;
    }

    emit requestFinished();
    //TODO
    qDebug() << doc.toByteArray();
}

void GoodReadsApi::handleAddBooksToShelves()
{
    bool ok = false;
    auto doc = GetDocumentFromReply(sender(), ok);
    if (!ok) {
        emit requestFinished();
        return;
    }

    emit requestFinished();
    //TODO
    qDebug() << doc.toByteArray();
}

void GoodReadsApi::handleRemoveBookFromShelf()
{
    bool ok = false;
    auto doc = GetDocumentFromReply(sender(), ok);
    if (!ok) {
        emit requestFinished();
        return;
    }

    emit requestFinished();
    //TODO
    qDebug() << doc.toByteArray();
}

void GoodReadsApi::handleGetAuthor()
{
    bool ok = false;
    auto doc = GetDocumentFromReply(sender(), ok);
    if (!ok) {
        emit requestFinished();
        return;
    }

    emit requestFinished();
    emit gotAuthorProfile(RpcUtils::Parser::ParseAuthor(doc));
}

void GoodReadsApi::handleGetAuthorBooks(quint64 authorId)
{
    bool ok = false;
    auto doc = GetDocumentFromReply(sender(), ok);
    if (!ok) {
        emit requestFinished();
        return;
    }

    emit requestFinished();
    emit gotAuthorBooks(authorId, RpcUtils::Parser::ParseAuthorBooks(doc));
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

    emit requestFinished();
    emit authorFollowed(authorId.toULongLong(), followingId.toULongLong());
}

void GoodReadsApi::handleUnfollowAuthor(quint64 authorId)
{
    bool ok = false;
    auto doc = GetDocumentFromReply(sender(), ok);
    if (!ok) {
        emit requestFinished();
        return;
    }

    emit requestFinished();
    if (doc.isNull()) {
        emit authorUnfollowed(authorId);
    }
}

void GoodReadsApi::handleGetGroups(quint64 userId)
{
    bool ok = false;
    auto doc = GetDocumentFromReply(sender(), ok);
    if (!ok) {
        emit requestFinished();
        return;
    }

    emit requestFinished();
    emit gotUserGroups(userId, RpcUtils::Parser::ParseGroups(doc));
}

void GoodReadsApi::handleGetGroup(quint64 groupId, QObject *senderObject)
{
    bool ok = false;
    auto doc = GetDocumentFromReply(senderObject ? senderObject : sender(), ok);
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

    emit requestFinished();
    emit gotUserGroup(groupId, RpcUtils::Parser::ParseGroup(doc));
}

void GoodReadsApi::handleJoinGroup(quint64 groupId)
{
    bool ok = false;
    auto doc = GetDocumentFromReply(sender(), ok);
    if (!ok) {
        emit requestFinished();
        return;
    }

    emit requestFinished();
    emit gotUserGroup(groupId, RpcUtils::Parser::ParseGroup(doc));
}

void GoodReadsApi::handleSearchGroup()
{
    bool ok = false;
    auto doc = GetDocumentFromReply(sender(), ok);
    if (!ok) {
        emit requestFinished();
        return;
    }

    emit requestFinished();
    emit gotFoundGroups(RpcUtils::Parser::ParseGroups(doc));
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
        RequestRedirectedUrl(redirectedUrl,
                std::bind(&GoodReadsApi::handleGetGroupMembers, this, groupId, std::placeholders::_1));
        return;
    }

    emit requestFinished();
    emit gotGroupMembers(groupId, RpcUtils::Parser::ParseGroupMembers(doc));
}

void GoodReadsApi::handleGetGroupFolderTopics(const QString& groupFolderId, quint64 groupId)
{
    bool ok = false;
    auto doc = GetDocumentFromReply(sender(), ok);
    if (!ok) {
        emit requestFinished();
        return;
    }
    emit requestFinished();
    emit gotGroupFolderTopics(groupFolderId, groupId, RpcUtils::Parser::ParseGroupFolderTopics(doc));
}

void GoodReadsApi::handleGetGroupFolderTopic()
{
    bool ok = false;
    auto doc = GetDocumentFromReply(sender(), ok);
    if (!ok) {
        emit requestFinished();
        return;
    }
    emit requestFinished();
    emit gotGroupFolderTopic(RpcUtils::Parser::ParseGroupFolderTopic(doc));
}

void GoodReadsApi::handleTopicAdded()
{
    bool ok = false;
    auto doc = GetDocumentFromReply(sender(), ok);
    if (!ok) {
        emit requestFinished();
        return;
    }
    emit requestFinished();
    emit gotNewGroupFolderTopic(RpcUtils::Parser::ParseGroupFolderTopic(doc));
}

void GoodReadsApi::handleNewCommentAdded()
{
    bool ok = false;
    auto doc = GetDocumentFromReply(sender(), ok);
    if (!ok) {
        emit requestFinished();
        return;
    }
    emit requestFinished();
    emit newCommentAdded(RpcUtils::Parser::ParseComment(doc));
}

void GoodReadsApi::handleGetFriends(quint64 userId)
{
    bool ok = false;
    auto doc = GetDocumentFromReply(sender(), ok);
    if (!ok) {
        emit requestFinished();
        return;
    }

    emit requestFinished();
    emit gotUserFriends(userId, RpcUtils::Parser::ParseFriends(doc));
}

void GoodReadsApi::handleGetUserFollowers(quint64 userId)
{
    bool ok = false;
    auto doc = GetDocumentFromReply(sender(), ok);
    if (!ok) {
        emit requestFinished();
        return;
    }

    emit requestFinished();
    emit gotUserFollowers(userId, RpcUtils::Parser::ParseFollowers(doc));
}

void GoodReadsApi::handleGetUserFollowings(quint64 userId)
{
    bool ok = false;
    auto doc = GetDocumentFromReply(sender(), ok);
    if (!ok) {
        emit requestFinished();
        return;
    }

    emit requestFinished();
    emit gotUserFollowings(userId, RpcUtils::Parser::ParseFollowings(doc));
}

void GoodReadsApi::handleGetFriendRequests()
{
    bool ok = false;
    auto doc = GetDocumentFromReply(sender(), ok);
    if (!ok) {
        emit requestFinished();
        return;
    }

    emit requestFinished();
    //TODO
    qDebug() << doc.toByteArray();
}

void GoodReadsApi::handleConfirmFriendRequest(quint64 friendRequestId, bool confirm)
{
    bool ok = false;
    auto doc = GetDocumentFromReply(sender(), ok);
    if (!ok) {
        emit requestFinished();
        return;
    }

    emit requestFinished();
    emit friendRequestConfirmed(friendRequestId, confirm);
}

void GoodReadsApi::handleConfirmFriendRecommendation()
{
    bool ok = false;
    auto doc = GetDocumentFromReply(sender(), ok);
    if (!ok) {
        emit requestFinished();
        return;
    }

    emit requestFinished();
    //TODO
    qDebug() << doc.toByteArray();
}

void GoodReadsApi::handleDeclineFriendRecommendation()
{
    bool ok = false;
    auto doc = GetDocumentFromReply(sender(), ok);
    if (!ok) {
        emit requestFinished();
        return;
    }

    emit requestFinished();
    //TODO
    qDebug() << doc.toByteArray();
}

void GoodReadsApi::handleAddFriend(quint64 userId)
{
    bool ok = false;
    auto doc = GetDocumentFromReply(sender(), ok);
    if (!ok) {
        emit requestFinished();
        return;
    }

    emit requestFinished();
    if (doc.isNull()) {
        emit friendAdded(userId);
    }
}

void GoodReadsApi::handleRemoveFriend(quint64 userId)
{
    bool ok = false;
    auto doc = GetDocumentFromReply(sender(), ok);
    if (!ok) {
        emit requestFinished();
        return;
    }

    emit requestFinished();
    if (doc.isNull()) {
        emit friendRemoved(userId);
    }
}

void GoodReadsApi::handleFollowUser()
{
    bool ok = false;
    auto doc = GetDocumentFromReply(sender(), ok);
    if (!ok) {
        emit requestFinished();
        return;
    }

    emit requestFinished();
    QXmlQuery query;
    query.setFocus(doc.toByteArray());
    const QString idStr(GetQueryResult(query, "/user-following/id/text()"));
    const QString userIdStr(GetQueryResult(query, "/user-following/user-id/text()"));
    emit userFollowed(userIdStr.toULongLong() , idStr.toLongLong() > 0);
}

void GoodReadsApi::handleUnfollowUser(quint64 userId)
{
    bool ok = false;
    auto doc = GetDocumentFromReply(sender(), ok);
    if (!ok) {
        emit requestFinished();
        return;
    }

    emit requestFinished();
    QXmlQuery query;
    query.setFocus(doc.toByteArray());
    const QString resultStr(GetQueryResult(query, "/hash/status/text()"));
    emit userUnfollowed(userId, resultStr == "success");
}

void GoodReadsApi::handleAddQuote()
{
    bool ok = false;
    auto doc = GetDocumentFromReply(sender(), ok);
    if (!ok) {
        emit requestFinished();
        return;
    }

    emit requestFinished();
    //TODO
    qDebug() << doc.toByteArray();
}

void GoodReadsApi::handleGetReadStatus()
{
    bool ok = false;
    auto doc = GetDocumentFromReply(sender(), ok);
    if (!ok) {
        emit requestFinished();
        return;
    }

    emit requestFinished();
    //TODO
    qDebug() << doc.toByteArray();
}

void GoodReadsApi::handleGetRecentUserStatuses()
{
    bool ok = false;
    auto doc = GetDocumentFromReply(sender(), ok);
    if (!ok) {
        emit requestFinished();
        return;
    }

    emit requestFinished();
    //TODO
    qDebug() << doc.toByteArray();
}

void GoodReadsApi::handleGetUserStatus()
{
    bool ok = false;
    auto doc = GetDocumentFromReply(sender(), ok);
    if (!ok) {
        emit requestFinished();
        return;
    }

    emit requestFinished();
    //TODO
    qDebug() << doc.toByteArray();
}

void GoodReadsApi::handleUpdateUserStatus()
{
    bool ok = false;
    auto doc = GetDocumentFromReply(sender(), ok);
    if (!ok) {
        emit requestFinished();
        return;
    }

    emit requestFinished();
    //TODO
    qDebug() << doc.toByteArray();
}

void GoodReadsApi::handleDeleteUserStatus()
{
    bool ok = false;
    auto doc = GetDocumentFromReply(sender(), ok);
    if (!ok) {
        emit requestFinished();
        return;
    }

    emit requestFinished();
    //TODO
    qDebug() << doc.toByteArray();
}
}
