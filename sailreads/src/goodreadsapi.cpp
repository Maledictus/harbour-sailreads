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
#include <QXmlQuery>

#include "oauthwrapper.h"
#include "rpcutils.h"

namespace Sailreads
{
GoodReadsApi::GoodReadsApi(QObject *parent)
: QObject(parent)
, m_ConsumerKey("GRGhcLIXU8M8u1NcnoMVFg")
, m_ConsumerSecretKey("Epo3MYIT3V1JcuC6OkZxptHLEuD8yqUgAj7mLNw")
, m_BaseUrl("https://www.goodreads.com")
, m_NAM(new QNetworkAccessManager(this))
, m_OAuthWrapper(new OAuthWrapper(m_ConsumerKey, m_ConsumerSecretKey, m_BaseUrl, this))
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
    QUrl requestTokenUrl = m_OAuthWrapper->GetRequestTokenUrl();

    QNetworkReply *reply = m_NAM->get(QNetworkRequest(requestTokenUrl));

    connect(reply, &QNetworkReply::finished,
            this, &GoodReadsApi::handleObtainRequestToken);
}

void GoodReadsApi::RequestAccessToken() const
{
    QUrl accessTokenUrl = m_OAuthWrapper->GetAccessTokenUrl(m_RequestToken, m_RequestTokenSecret);
    QNetworkReply *reply = m_NAM->get(QNetworkRequest(accessTokenUrl));

    connect(reply, &QNetworkReply::finished,
            this, &GoodReadsApi::handleRequestAccessToken);
}

void GoodReadsApi::AuthUser()
{
    const auto& pair = m_OAuthWrapper->MakeSignedUrl(m_AccessToken, m_AccessTokenSecret,
            QUrl("https://www.goodreads.com/api/auth_user"));
    auto reply = m_NAM->get(QNetworkRequest(pair.first));
    connect (reply, &QNetworkReply::finished,
             this, &GoodReadsApi::handleAuthUser);
}

void GoodReadsApi::GetUserInfo(quint64 id)
{
    const QUrl url(QString("https://www.goodreads.com/user/show/%1.xml?key=%2")
            .arg(id)
            .arg(m_ConsumerKey));
    auto reply = m_NAM->get(QNetworkRequest(url));
    m_CurrentReply = reply;
    connect (reply, &QNetworkReply::finished,
             this, &GoodReadsApi::handleGetUserInfo);
}

void GoodReadsApi::CompareBooks(quint64 userId)
{
    const auto& pair = m_OAuthWrapper->MakeSignedUrl(m_AccessToken, m_AccessTokenSecret,
            QUrl(QString("https://www.goodreads.com/user/compare/%1.xml").arg(userId)));
    auto reply = m_NAM->get(QNetworkRequest(pair.first));
    connect (reply, &QNetworkReply::finished,
             this, &GoodReadsApi::handleCompareBooks);
}

void GoodReadsApi::GetUserFollowers(quint64 userId, int page)
{
    const auto& pair = m_OAuthWrapper->MakeSignedUrl(m_AccessToken, m_AccessTokenSecret,
            QUrl(QString("https://www.goodreads.com/user/%1/followers.xml?page=%2")
                 .arg(userId).arg(page)));
    auto reply = m_NAM->get(QNetworkRequest(pair.first));
    connect (reply, &QNetworkReply::finished,
             this, &GoodReadsApi::handleGetUserFollowers);
}

void GoodReadsApi::GetUserFollowings(quint64 userId, int page)
{
    const auto& pair = m_OAuthWrapper->MakeSignedUrl(m_AccessToken, m_AccessTokenSecret,
            QUrl(QString("https://www.goodreads.com/user/%1/following.xml?page=%2")
                 .arg(userId).arg(page)));
    auto reply = m_NAM->get(QNetworkRequest(pair.first));
    connect (reply, &QNetworkReply::finished,
             this, &GoodReadsApi::handleGetUserFollowings);
}

void GoodReadsApi::GetUpdates()
{
    const auto& pair = m_OAuthWrapper->MakeSignedUrl(m_AccessToken, m_AccessTokenSecret,
            QUrl("https://www.goodreads.com/updates/friends.xml"));
    auto reply = m_NAM->get(QNetworkRequest(pair.first));
    m_CurrentReply = reply;
    connect (reply, &QNetworkReply::finished,
             this, &GoodReadsApi::handleGetUpdates);
}

void GoodReadsApi::GetNotifications(int page)
{
    const auto& pair = m_OAuthWrapper->MakeSignedUrl(m_AccessToken, m_AccessTokenSecret,
            QUrl(QString("https://www.goodreads.com/notifications.xml?page=%1").arg(page)));
    auto reply = m_NAM->get(QNetworkRequest(pair.first));
    m_CurrentReply = reply;
    connect (reply, &QNetworkReply::finished,
             this, &GoodReadsApi::handleGetNotifications);
}

void GoodReadsApi::GetBookShelves(quint64 userId)
{
    const QUrl url(QString("https://www.goodreads.com/shelf/list.xml?key=%1&user_id=%2")
            .arg(m_ConsumerKey)
            .arg(userId));
    auto reply = m_NAM->get(QNetworkRequest(url));
    m_CurrentReply = reply;
    connect(reply,
            &QNetworkReply::finished,
            this,
            [this, userId]() {
                handleGetBookShelves(userId);
            });
}

namespace
{
QNetworkRequest PreparePostRequest(const QUrl& url)
{
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    return request;
}
}

void GoodReadsApi::AddBookShelf(const QString& name, bool exclusive)
{
    QString urlString = QString("https://www.goodreads.com/user_shelves.xml?user_shelf[name]=%1&"
            "user_shelf[exclusive_flag]=%2").arg(name).arg(exclusive ? "true" : "false");
    const auto& pair = m_OAuthWrapper->MakeSignedUrl(m_AccessToken, m_AccessTokenSecret,
            QUrl(urlString), "POST");

    auto reply = m_NAM->post(PreparePostRequest(pair.first), pair.second);
    connect(reply, &QNetworkReply::finished,
            this, &GoodReadsApi::handleAddBookShelf);
}

void GoodReadsApi::EditBookShelf(quint64 id, const QString& name, bool exclusive)
{
    const auto& pair = m_OAuthWrapper->MakeSignedUrl(m_AccessToken, m_AccessTokenSecret,
            QUrl(QString("https://www.goodreads.com/user_shelves/%1.xml?user_shelf[name]=%2&"
                    "user_shelf[exclusive_flag]=%3").arg(id).arg(name)
                        .arg(exclusive ? "true" : "false")), "PUT");
    auto reply = m_NAM->put(QNetworkRequest(pair.first), QByteArray());
    connect(reply, &QNetworkReply::finished,
            this, &GoodReadsApi::handleEditBookShelf);
}

void GoodReadsApi::GetReviews(quint64 userId, const QString& bookShelf, const QString& sortField,
        Qt::SortOrder order, int page)
{
    const auto& pair = m_OAuthWrapper->MakeSignedUrl(m_AccessToken, m_AccessTokenSecret,
            QUrl(QString("https://www.goodreads.com/review/list/%1.xml?v=2&shelf=%2&page=%3&"
                         "per_page=200&sort=%4&order=%5")
                 .arg(userId).arg(bookShelf).arg(page).arg(sortField)
                 .arg(order == Qt::AscendingOrder ? "a" : "d")), "GET");
    auto reply = m_NAM->get(QNetworkRequest(pair.first));
    m_CurrentReply = reply;
    connect(reply, &QNetworkReply::finished,
            this, &GoodReadsApi::handleGetReviews);
}

void GoodReadsApi::GetReview(quint64 reviewId, int page)
{
    QString url = QString("https://www.goodreads.com/review/show.xml?id=%1&key=%2&page=3")
            .arg(reviewId).arg(m_ConsumerKey).arg(page);
    auto reply = m_NAM->get(QNetworkRequest(url));
    connect(reply, &QNetworkReply::finished,
            this, &GoodReadsApi::handleGetReview);
}

void GoodReadsApi::AddReview(quint64 bookId, const QString& review, int rating,
    const QDateTime& readAt, const QString& shelf)
{
    QString url = QString("https://www.goodreads.com/review.xml?book_id=%1&review[review]=%2")
            .arg(bookId).arg(review);
    if (rating > 0) {
        url.append("&review[rating]=" + QString::number(rating));
    }
    if (readAt.isValid()) {
        url.append("&review[read_at]=" + readAt.toString("yyyy-MM-dd"));
    }
    if (!shelf.isEmpty()) {
        url.append("&shelf=" + shelf);
    }
    const auto& pair = m_OAuthWrapper->MakeSignedUrl(m_AccessToken, m_AccessTokenSecret,
            QUrl(url), "POST");
    auto reply = m_NAM->post(PreparePostRequest(pair.first), pair.second);
    connect(reply, &QNetworkReply::finished,
            this, &GoodReadsApi::handleAddReview);
}

void GoodReadsApi::EditReview(quint64 reviewId, const QString& review, int rating,
    const QDateTime& readAt, bool finished, const QString& shelf)
{
    QString url = QString("https://www.goodreads.com/review/%1.xml?review[review]=%2")
            .arg(reviewId).arg(review);
    if (rating > 0) {
        url.append("&review[rating]=" + QString::number(rating));
    }
    if (readAt.isValid()) {
        url.append("&review[read_at]=" + readAt.toString("yyyy-MM-dd"));
    }
    if (finished) {
        url.append("&finished=true");
    }
    if (!shelf.isEmpty()) {
        url.append("&shelf=" + shelf);
    }
    const auto& pair = m_OAuthWrapper->MakeSignedUrl(m_AccessToken, m_AccessTokenSecret,
            QUrl(url), "PUT");
    auto reply = m_NAM->put(QNetworkRequest(pair.first), pair.second);
    connect(reply, &QNetworkReply::finished,
            this, &GoodReadsApi::handleEditReview);
}

void GoodReadsApi::DeleteReview(quint64 reviewId)
{
    const auto& pair = m_OAuthWrapper->MakeSignedUrl(m_AccessToken, m_AccessTokenSecret,
            QUrl(QString("https://www.goodreads.com/review/destroy/%1?format=xml").arg(reviewId)),
                    "DELETE");
    auto reply = m_NAM->deleteResource(QNetworkRequest(pair.first));
    connect(reply, &QNetworkReply::finished,
            this, &GoodReadsApi::handleDeleteReview);
}

void GoodReadsApi::GetBook(quint64 bookId)
{
    const auto& pair = m_OAuthWrapper->MakeSignedUrl(m_AccessToken, m_AccessTokenSecret,
            QUrl(QString("https://www.goodreads.com/book/show.xml?id=%1").arg(bookId)), "GET");
    auto reply = m_NAM->get(QNetworkRequest(pair.first));
    m_CurrentReply = reply;
    connect(reply, &QNetworkReply::finished,
            this, &GoodReadsApi::handleGetBook);
}

void GoodReadsApi::SearchBooks(const QString& query, const QString& key, int page)
{
    const QUrl url(QString("https://www.goodreads.com/search/index.xml?q=%1key=%2&search[field]=%3page=%4")
            .arg(query).arg(m_ConsumerKey).arg(key).arg(page));
    auto reply = m_NAM->get(QNetworkRequest(url));
    m_CurrentReply = reply;
    connect(reply, &QNetworkReply::finished,
            this, &GoodReadsApi::handleSearchBook);
}

void GoodReadsApi::GetSeries(quint64 seriesId)
{
    const QUrl url(QString("https://www.goodreads.com/series/show/%1.xmlkey=%2")
            .arg(seriesId).arg(m_ConsumerKey));
    auto reply = m_NAM->get(QNetworkRequest(url));
    m_CurrentReply = reply;
    connect(reply, &QNetworkReply::finished,
            this, &GoodReadsApi::handleGetSeries);
}

void GoodReadsApi::GetAuthorSeries(quint64 authorId)
{
    const QUrl url(QString("https://www.goodreads.com/list/%1.xmlkey=%2")
            .arg(authorId).arg(m_ConsumerKey));
    auto reply = m_NAM->get(QNetworkRequest(url));
    m_CurrentReply = reply;
    connect(reply, &QNetworkReply::finished,
            this, &GoodReadsApi::handleGetAuthorSeries);
}

void GoodReadsApi::GetWorkSeries(quint64 workId)
{
    const QUrl url(QString("https://www.goodreads.com/work/%1/series?format=xml&key=%2")
            .arg(workId).arg(m_ConsumerKey));
    auto reply = m_NAM->get(QNetworkRequest(url));
    m_CurrentReply = reply;
    connect(reply, &QNetworkReply::finished,
            this, &GoodReadsApi::handleGetWorkSeries);
}

void GoodReadsApi::AddBookToShelf(quint64 bookId, const QString& shelfName)
{
    const auto& pair = m_OAuthWrapper->MakeSignedUrl(m_AccessToken, m_AccessTokenSecret,
            QUrl(QString("https://www.goodreads.com/shelf/add_to_shelf.xml?book_id=%1&name=%2")
                 .arg(bookId).arg(shelfName)), "POST");
    auto reply = m_NAM->post(PreparePostRequest(pair.first), pair.second);
    connect(reply, &QNetworkReply::finished,
            this, &GoodReadsApi::handleAddBookToShelf);
}

void GoodReadsApi::AddBooksToShelves(QList<quint64>& bookIds, const QStringList& shelvesName)
{
    QStringList bookIdsStringList;
    std::transform(bookIds.begin(), bookIds.end(),
            std::back_inserter(bookIdsStringList),
            [](decltype(bookIds.front()) bookId)
            { return QString::number(bookId); });

    const auto& pair = m_OAuthWrapper->MakeSignedUrl(m_AccessToken, m_AccessTokenSecret,
            QUrl(QString("https://www.goodreads.com/shelf/add_books_to_shelves.xml?bookids=%1&shelves=%2")
                 .arg(bookIdsStringList.join(",")).arg(shelvesName.join(","))), "POST");
    auto reply = m_NAM->post(PreparePostRequest(pair.first), pair.second);
    connect(reply, &QNetworkReply::finished,
            this, &GoodReadsApi::handleAddBooksToShelves);
}

void GoodReadsApi::RemoveBookFromShelf(quint64 bookId, const QString& shelfName)
{
    const auto& pair = m_OAuthWrapper->MakeSignedUrl(m_AccessToken, m_AccessTokenSecret,
            QUrl(QString("https://www.goodreads.com/shelf/add_to_shelf.xml?book_id=%1&name=%2&a=remove")
                 .arg(bookId).arg(shelfName)), "POST");
    auto reply = m_NAM->post(PreparePostRequest(pair.first), pair.second);
    connect(reply, &QNetworkReply::finished,
            this, &GoodReadsApi::handleRemoveBookFromShelf);
}

void GoodReadsApi::GetAuthor(quint64 authorId)
{
    const QUrl url(QString("https://www.goodreads.com/author/list/%1.xml?key=%2")
            .arg(authorId).arg(m_ConsumerKey));
    auto reply = m_NAM->get(QNetworkRequest(url));
    m_CurrentReply = reply;
    connect(reply, &QNetworkReply::finished,
            this, &GoodReadsApi::handleGetAuthor);
}

void GoodReadsApi::GetAuthorBooks(quint64 authorId, int page)
{
    const QUrl url(QString("https://www.goodreads.com/author/show/%1.xml?key=%2&page=%3")
            .arg(authorId).arg(m_ConsumerKey).arg(page));
    auto reply = m_NAM->get(QNetworkRequest(url));
    m_CurrentReply = reply;
    connect(reply, &QNetworkReply::finished,
            this, &GoodReadsApi::handleGetAuthorBooks);
}

void GoodReadsApi::FollowAuthor(quint64 authorId)
{
    const auto& pair = m_OAuthWrapper->MakeSignedUrl(m_AccessToken, m_AccessTokenSecret,
            QUrl(QString("https://www.goodreads.com/author_followings?id=%1&format=xml")
                 .arg(authorId)), "POST");
    auto reply = m_NAM->post(PreparePostRequest(pair.first), pair.second);
    connect(reply, &QNetworkReply::finished,
            this, &GoodReadsApi::handleFollowAuthor);
}

void GoodReadsApi::UnfollowAuthor(quint64 authorFollowingId)
{
    const auto& pair = m_OAuthWrapper->MakeSignedUrl(m_AccessToken, m_AccessTokenSecret,
            QUrl(QString("https://www.goodreads.com/author_followings/%1?format=xml")
                 .arg(authorFollowingId)), "DELETE");
    auto reply = m_NAM->deleteResource(QNetworkRequest(pair.first));
    connect(reply, &QNetworkReply::finished,
            this, &GoodReadsApi::handleUnfollowAuthor);
}

void GoodReadsApi::ShowAuthorFollowingInformation(quint64 authorFollowingId)
{
    const auto& pair = m_OAuthWrapper->MakeSignedUrl(m_AccessToken, m_AccessTokenSecret,
            QUrl(QString("https://www.goodreads.com/author_followings/%1?format=xml")
                 .arg(authorFollowingId)), "GET");
    auto reply = m_NAM->get(QNetworkRequest(pair.first));
    connect(reply, &QNetworkReply::finished,
            this, &GoodReadsApi::handleShowAuthorFollowingInformation);
}

void GoodReadsApi::GetGroups(quint64 userId)
{
    const QUrl url(QString("https://www.goodreads.com/group/list/%1.xml?key=%2&sort=last_activity")
            .arg(userId)
            .arg(m_ConsumerKey));
    auto reply = m_NAM->get(QNetworkRequest(url));
    m_CurrentReply = reply;
    connect(reply, &QNetworkReply::finished,
            this,
            [this, userId]() {
                handleGetGroups(userId);
            });
}

void GoodReadsApi::GetGroup(quint64 groupId)
{
    const QUrl url(QString("https://www.goodreads.com/group/show/%1.xml?key=%2&sort=updated_at")
                   .arg(groupId)
                   .arg(m_ConsumerKey));
    auto reply = m_NAM->get(QNetworkRequest(url));
    m_CurrentReply = reply;
    connect(reply, &QNetworkReply::finished,
            this,
            [this, groupId]() {
                handleGetGroup(groupId);
            });
}

void GoodReadsApi::JoinGroup(quint64 groupId)
{
    const auto& pair = m_OAuthWrapper->MakeSignedUrl(m_AccessToken, m_AccessTokenSecret,
            QUrl(QString("https://www.goodreads.com/group/join?format=xml&id=%1")
                 .arg(groupId)), "POST");
    auto reply = m_NAM->post(PreparePostRequest(pair.first), pair.second);
    connect(reply, &QNetworkReply::finished,
            this, &GoodReadsApi::handleJoinGroup);
}

void GoodReadsApi::SearchGroup(const QString& text, int page)
{
    const QUrl url(QString("https://www.goodreads.com/group/search.xml?q=%1&page=%2&key=%3")
                   .arg(QString(QUrl::toPercentEncoding(text)))
                   .arg(page)
                   .arg(m_ConsumerKey));
    auto reply = m_NAM->get(QNetworkRequest(url));
    m_CurrentReply = reply;
    connect(reply, &QNetworkReply::finished,
            this, &GoodReadsApi::handleSearchGroup);
}

void GoodReadsApi::GetGroupMembers(quint64 groupId, int page)
{
    const QUrl url(QString("https://www.goodreads.com/group/members/%1.xml?page=%2&key=%3&sort=num_comments")
                   .arg(groupId)
                   .arg(page)
                   .arg(m_ConsumerKey));
    auto reply = m_NAM->get(QNetworkRequest(url));
    m_CurrentReply = reply;
    connect(reply, &QNetworkReply::finished,
            this,
            [this, groupId] {
                handleGetGroupMembers(groupId);
            });
}

void GoodReadsApi::GetGroupFolderTopics(quint64 groupFolderId, quint64 groupId, int page)
{
    const QUrl url(QString("https://www.goodreads.com/topic/group_folder/%1?group_id=%2&page=%3&key=%4&sort=updated_at")
                   .arg(groupFolderId)
                   .arg(groupId)
                   .arg(page)
                   .arg(m_ConsumerKey));
    auto reply = m_NAM->get(QNetworkRequest(url));
    m_CurrentReply = reply;
    connect(reply, &QNetworkReply::finished,
            this,
            [this, groupFolderId, groupId] {
                handleGetGroupFolderTopics(groupFolderId, groupId);
            });
}

void GoodReadsApi::GetGroupFolderTopic(quint64 topicId, int page)
{
    const QUrl url(QString("https://www.goodreads.com/topic/show.xml?id=%1&page=%2&key=%3")
                   .arg(topicId)
                   .arg(page)
                   .arg(m_ConsumerKey));
    auto reply = m_NAM->get(QNetworkRequest(url));
    m_CurrentReply = reply;
    connect(reply, &QNetworkReply::finished,
            this, &GoodReadsApi::handleGetGroupFolderTopic);
}

void GoodReadsApi::AddNewTopic(const QString& topic, const QString& subject, quint64 subjectId,
        quint64 folderId, bool question, bool updateFeed, bool digest, const QString& comment)
{
    QString url = QString("https://www.goodreads.com/topic.xml?topic[subject_type]=%1&"
            "topic[subject_id]=%2&topic[folder_id]=%3&topic[title]=%4&topic[question_flag]=%5&"
            "update_feed=%6&digest=%7&comment[body_usertext]=%8").arg(subject)
                    .arg(subjectId).arg(folderId).arg(topic)
                    .arg(question ? "1" : "0").arg(updateFeed ? "1" : "0")
                    .arg(digest ? "1" : "0").arg(comment);
    const auto& pair = m_OAuthWrapper->MakeSignedUrl(m_AccessToken, m_AccessTokenSecret,
            QUrl(url), "POST");
    auto reply = m_NAM->post(PreparePostRequest(pair.first), pair.second);
    connect (reply, &QNetworkReply::finished,
             this, &GoodReadsApi::handleTopicAdded);
}

void GoodReadsApi::AddNewComment(const QString& type, quint64 resourceId, const QString& comment)
{
    QString url = QString("https://www.goodreads.com/comment.xml?type=%1&"
            "id=%2&comment[body]=%3").arg(type).arg(resourceId).arg(comment);
    const auto& pair = m_OAuthWrapper->MakeSignedUrl(m_AccessToken, m_AccessTokenSecret,
            QUrl(url), "POST");
    auto reply = m_NAM->post(PreparePostRequest(pair.first), pair.second);
    connect (reply, &QNetworkReply::finished,
             this, &GoodReadsApi::handleNewCommentAdded);
}

void GoodReadsApi::GetFriends(quint64 userId)
{
    const auto& pair = m_OAuthWrapper->MakeSignedUrl(m_AccessToken, m_AccessTokenSecret,
            QUrl(QString("https://www.goodreads.com/friend/user/%1?format=xml").arg(userId)));
    auto reply = m_NAM->get(QNetworkRequest(pair.first));
    m_CurrentReply = reply;
    connect (reply, &QNetworkReply::finished,
             this,
             [this, userId]() {
                 handleGetFriends(userId);
            });
}

void GoodReadsApi::GetFriendRequests(int page)
{
    const auto& pair = m_OAuthWrapper->MakeSignedUrl(m_AccessToken, m_AccessTokenSecret,
            QUrl(QString("https://www.goodreads.com/friend/requests.xml?page=%1").arg(page)));
    auto reply = m_NAM->get(QNetworkRequest(pair.first));
    m_CurrentReply = reply;
    connect (reply, &QNetworkReply::finished,
             this, &GoodReadsApi::handleGetFriendRequests);
}

void GoodReadsApi::ConfirmFriendRequest(quint64 friendRequestId)
{
    const auto& pair = m_OAuthWrapper->MakeSignedUrl(m_AccessToken, m_AccessTokenSecret,
            QUrl(QString("https://www.goodreads.com/friend/confirm_request.xml?id=%1&response=%2")
                 .arg(friendRequestId).arg("Y")), "POST");
    auto reply = m_NAM->post(PreparePostRequest(pair.first), pair.second);
    connect (reply, &QNetworkReply::finished,
             this, &GoodReadsApi::handleConfirmFriendRequest);
}

void GoodReadsApi::DeclineFriendRequest(quint64 friendRequestId)
{
    const auto& pair = m_OAuthWrapper->MakeSignedUrl(m_AccessToken, m_AccessTokenSecret,
            QUrl(QString("https://www.goodreads.com/friend/confirm_request.xml?id=%1&response=%2")
                 .arg(friendRequestId).arg("N")), "POST");
    auto reply = m_NAM->post(PreparePostRequest(pair.first), pair.second);
    connect (reply, &QNetworkReply::finished,
             this, &GoodReadsApi::handleDeclineFriendRequest);
}

void GoodReadsApi::ConfirmFriendRecommendation(quint64 friendRecommendationId)
{
    const auto& pair = m_OAuthWrapper->MakeSignedUrl(m_AccessToken, m_AccessTokenSecret,
            QUrl(QString("https://www.goodreads.com/friend/confirm_recommendation.xml?id=%1&response=%2")
                 .arg(friendRecommendationId).arg("Y")), "POST");
    auto reply = m_NAM->post(PreparePostRequest(pair.first), pair.second);
    connect (reply, &QNetworkReply::finished,
             this, &GoodReadsApi::handleConfirmFriendRecommendation);
}

void GoodReadsApi::DeclineFriendRecommendation(quint64 friendRecommendationId)
{
    const auto& pair = m_OAuthWrapper->MakeSignedUrl(m_AccessToken, m_AccessTokenSecret,
            QUrl(QString("https://www.goodreads.com/friend/confirm_recommendation.xml?id=%1&response=%2")
                 .arg(friendRecommendationId).arg("N")), "POST");
    auto reply = m_NAM->post(PreparePostRequest(pair.first), pair.second);
    connect (reply, &QNetworkReply::finished,
             this, &GoodReadsApi::handleDeclineFriendRecommendation);
}

void GoodReadsApi::AddFriend(quint64 userId)
{
    const auto& pair = m_OAuthWrapper->MakeSignedUrl(m_AccessToken, m_AccessTokenSecret,
            QUrl(QString("https://www.goodreads.com/friend/user/%1?format=xml").arg(userId)), "POST");
    auto reply = m_NAM->post(PreparePostRequest(pair.first), pair.second);
    connect (reply, &QNetworkReply::finished,
             this,
             [this, userId]() {
                 handleGetFriends(userId);
            });
}

void GoodReadsApi::FollowUser(quint64 userId)
{
    const auto& pair = m_OAuthWrapper->MakeSignedUrl(m_AccessToken, m_AccessTokenSecret,
            QUrl(QString("https://www.goodreads.com/user/%1/followers?format=xml").arg(userId)), "POST");
    auto reply = m_NAM->post(PreparePostRequest(pair.first), pair.second);
    connect (reply, &QNetworkReply::finished,
             this, &GoodReadsApi::handleFollowUser);
}

void GoodReadsApi::UnfollowUser(quint64 userId)
{
    const auto& pair = m_OAuthWrapper->MakeSignedUrl(m_AccessToken, m_AccessTokenSecret,
            QUrl(QString("https://www.goodreads.com/user/%1/followers/stop_following.xml")
                    .arg(userId)), "DELETE");
    auto reply = m_NAM->deleteResource(QNetworkRequest(pair.first));
    connect (reply, &QNetworkReply::finished,
             this, &GoodReadsApi::handleUnfollowUser);
}

void GoodReadsApi::AddQuote(const QString& authorName, quint64 authorId, quint64 bookId,
    const QString& quote, const QStringList& tags)
{
    const auto& pair = m_OAuthWrapper->MakeSignedUrl(m_AccessToken, m_AccessTokenSecret,
            QUrl(QString("https://www.goodreads.com/quotes?format=xml&quote[author_name]=%1&"
                         "quote[author_id]=%2&quote[book_id]=%3&quote[body]=%4&quote[tags]=%5")
                    .arg(authorName).arg(authorId).arg(bookId).arg(quote).arg(tags.join(","))),
            "POST");
    auto reply = m_NAM->post(PreparePostRequest(pair.first), pair.second);
    connect (reply, &QNetworkReply::finished,
             this, &GoodReadsApi::handleAddQuote);
}

void GoodReadsApi::GetReadStatus(quint64 readStatusId)
{
    const QUrl url(QString("https://www.goodreads.com/read_statuses/%1?format=xml&key=%2")
                   .arg(readStatusId)
                   .arg(m_ConsumerKey));
    auto reply = m_NAM->get(QNetworkRequest(url));
    connect(reply, &QNetworkReply::finished,
            this, &GoodReadsApi::handleGetReadStatus);
}

void GoodReadsApi::GetRecentUserStatuses()
{
    auto reply = m_NAM->get(QNetworkRequest(QUrl("https://www.goodreads.com/user_status/index.xml")));
    connect(reply, &QNetworkReply::finished,
            this, &GoodReadsApi::handleGetRecentUserStatuses);
}

void GoodReadsApi::GetUserStatus(quint64 userStatusId)
{
    QUrl url(QString("https://www.goodreads.com/user_status/show/%1?format=xml&key=%2")
             .arg(userStatusId).arg(m_ConsumerKey));
    auto reply = m_NAM->get(QNetworkRequest(url));
    connect(reply, &QNetworkReply::finished,
            this, &GoodReadsApi::handleGetUserStatus);
}

void GoodReadsApi::UpdateUserStatus(quint64 bookId, const QString& body, int percent, int page)
{
    QString url(QString("https://www.goodreads.com/user_status.xml?user_status[book_id]=%1&"
                "user_status[body]=%2").arg(bookId).arg(body));
    if (page > 0) {
        url.append("&user_status[page]=" + QString::number(page));
    }
    else if (percent > 0) {
        url.append("&user_status[percent]=" + QString::number(percent));
    }
    const auto& pair = m_OAuthWrapper->MakeSignedUrl(m_AccessToken, m_AccessTokenSecret,
            QUrl(url), "POST");
    auto reply = m_NAM->post(PreparePostRequest(pair.first), pair.second);
    connect (reply, &QNetworkReply::finished,
             this, &GoodReadsApi::handleUpdateUserStatus);
}

void GoodReadsApi::DeleteUserStatus(quint64 userStatusId)
{
    const auto& pair = m_OAuthWrapper->MakeSignedUrl(m_AccessToken, m_AccessTokenSecret,
            QUrl(QString("https://www.goodreads.com/user_status/destroy/%1?format=xml")
                    .arg(userStatusId)), "POST");
    auto reply = m_NAM->post(PreparePostRequest(pair.first), pair.second);
    connect (reply, &QNetworkReply::finished,
             this, &GoodReadsApi::handleDeleteUserStatus);
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
            reply->error() != QNetworkReply::OperationCanceledError) {
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

    ok = true;
    return reply->readAll();
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

void GoodReadsApi::handleGetUserFollowers()
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

void GoodReadsApi::handleGetUserFollowings()
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
    //TODO
    qDebug() << doc.toByteArray();
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
    //TODO
    qDebug() << doc.toByteArray();
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
    //TODO
    qDebug() << doc.toByteArray();
}

void GoodReadsApi::handleGetAuthorBooks()
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

void GoodReadsApi::handleFollowAuthor()
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

void GoodReadsApi::handleUnfollowAuthor()
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

void GoodReadsApi::handleShowAuthorFollowingInformation()
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

void GoodReadsApi::handleJoinGroup()
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

void GoodReadsApi::handleGetGroupFolderTopics(quint64 groupFolderId, quint64 groupId)
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

void GoodReadsApi::handleConfirmFriendRequest()
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

void GoodReadsApi::handleDeclineFriendRequest()
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

void GoodReadsApi::handleAddFriend()
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

void GoodReadsApi::handleFollowUser()
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

void GoodReadsApi::handleUnfollowUser()
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
