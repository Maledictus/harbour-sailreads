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

#pragma once

#include <functional>
#include <memory>

#include <QDomDocument>
#include <QObject>
#include <QUrl>
#include <QPointer>

#include "userprofile.h"
#include "objects/book.h"
#include "objects/comment.h"
#include "objects/group.h"
#include "objects/friend.h"
#include "objects/review.h"
#include "objects/topic.h"

class QNetworkAccessManager;
class QNetworkReply;

namespace Sailreads
{
class OAuthWrapper;

class GoodReadsApi : public QObject
{
    Q_OBJECT

    const QString m_ConsumerKey;
    const QString m_ConsumerSecretKey;
    const QUrl m_BaseUrl;

    QString m_RequestToken;
    QString m_RequestTokenSecret;

    QString m_AccessToken;
    QString m_AccessTokenSecret;

    QNetworkAccessManager *m_NAM;
    OAuthWrapper *m_OAuthWrapper;

    QPointer<QNetworkReply> m_CurrentReply;

public:
    explicit GoodReadsApi(QObject *parent = 0);

    void AbortRequest();

    void RequestRedirectedUrl(const QUrl& url, const std::function<void(QObject*)>& func);

    void UpdateCredentials(const QString& accessToken, const QString& accessTokenSecret);

    void ObtainRequestToken() const;
    void RequestAccessToken() const;

    void AuthUser();
    void GetUserInfo(quint64 id);
    void CompareBooks(quint64 userId);
    void GetUserFollowers(quint64 userId, int page);
    void GetUserFollowings(quint64 userId, int page);

    void GetUpdates();
    void GetNotifications(int page);

    void GetBookShelves(quint64 userId);
    void AddBookShelf(const QString& name, bool exclusive);
    void EditBookShelf(quint64 id, const QString& name, bool exclusive);

    void GetReviews(quint64 userId, const QString& bookShelf, const QString& sortField = "date_added",
            Qt::SortOrder order = Qt::DescendingOrder, int page = 1);
    void GetReview(quint64 reviewId, int page);
    void AddReview(quint64 bookId, const QString& review, int rating, const QDateTime& readAt,
            const QString& shelf);
    void EditReview(quint64 reviewId, const QString& review, int rating, const QDateTime& readAt,
            bool finished, const QString& shelf);
    void DeleteReview(quint64 reviewId);

    void GetBook(quint64 bookId);
    void SearchBooks(const QString& query, const QString& key, int page);

    void GetSeries(quint64 seriesId);
    void GetAuthorSeries(quint64 authorId);
    void GetWorkSeries(quint64 workId);

    void AddBookToShelf(quint64 bookId, const QString& shelfName);
    void AddBooksToShelves(QList<quint64>& bookIds, const QStringList& shelvesName);
    void RemoveBookFromShelf(quint64 bookId, const QString& shelfName);

    void GetAuthor(quint64 authorId);
    void GetAuthorBooks(quint64 authorId, int page);
    void FollowAuthor(quint64 authorId);
    void UnfollowAuthor(quint64 authorFollowingId);
    void ShowAuthorFollowingInformation(quint64 authorFollowingId);

    void GetGroups(quint64 userId);
    void GetGroup(quint64 groupId, const QString& groupName);
    void JoinGroup(quint64 groupId);
    void SearchGroup(const QString& text, int page);
    void GetGroupMembers(quint64 groupId, int page);
    void GetGroupFolderTopics(quint64 groupFolderId, quint64 groupId, int page);
    void GetGroupFolderTopic(quint64 topicId, int page);
    void AddNewTopic(const QString& topic, const QString& subject, quint64 subjectId,
            quint64 folderId, bool question, bool updateFeed, bool digest, const QString& comment);

    void AddNewComment(const QString& type, quint64 resourceId, const QString& comment);

    void GetFriends(quint64 userId);
    void GetFriendRequests(int page);
    void ConfirmFriendRequest(quint64 friendRequestId);
    void DeclineFriendRequest(quint64 friendRequestId);
    void ConfirmFriendRecommendation(quint64 friendRecommendationId);
    void DeclineFriendRecommendation(quint64 friendRecommendationId);
    void AddFriend(quint64 userId);
    void FollowUser(quint64 userId);
    void UnfollowUser(quint64 userId);

    void AddQuote(const QString& authorName, quint64 authorId, quint64 bookId, const QString& quote,
        const QStringList& tags);

    void GetReadStatus(quint64 readStatusId);

    void GetRecentUserStatuses();
    void GetUserStatus(quint64 userStatusId);
    void UpdateUserStatus(quint64 bookId, const QString& body, int percent, int page = -1);
    void DeleteUserStatus(quint64 userStatusId);

private:
    QDomDocument GetDocumentFromReply(QObject *sender, bool& ok);
    QByteArray GetReply(QObject *sender, bool& ok);

private slots:
    void handleObtainRequestToken();
    void handleRequestAccessToken();

    void handleAuthUser();
    void handleGetUserInfo();
    void handleCompareBooks();
    void handleGetUserFollowers();
    void handleGetUserFollowings();

    void handleGetUpdates();
    void handleGetNotifications();

    void handleGetBookShelves(quint64 userId);
    void handleAddBookShelf();
    void handleEditBookShelf();

    void handleGetReviews();
    void handleGetReview();
    void handleAddReview();
    void handleEditReview();
    void handleDeleteReview();

    void handleGetBook();
    void handleSearchBook();

    void handleGetSeries();
    void handleGetAuthorSeries();
    void handleGetWorkSeries();

    void handleAddBookToShelf();
    void handleAddBooksToShelves();
    void handleRemoveBookFromShelf();

    void handleGetAuthor();
    void handleGetAuthorBooks();
    void handleFollowAuthor();
    void handleUnfollowAuthor();
    void handleShowAuthorFollowingInformation();

    void handleGetGroups(quint64 userId);
    void handleGetGroup(quint64 groupId, QObject *senderObject = nullptr);
    void handleJoinGroup();
    void handleSearchGroup();
    void handleGetGroupMembers(quint64 groupId, QObject *senderObject = nullptr);
    void handleGetGroupFolderTopics(quint64 groupFolderId, quint64 groupId);
    void handleGetGroupFolderTopic();
    void handleTopicAdded();

    void handleNewCommentAdded();

    void handleGetFriends(quint64 userId);
    void handleGetFriendRequests();
    void handleConfirmFriendRequest();
    void handleDeclineFriendRequest();
    void handleConfirmFriendRecommendation();
    void handleDeclineFriendRecommendation();
    void handleAddFriend();
    void handleFollowUser();
    void handleUnfollowUser();

    void handleAddQuote();

    void handleGetReadStatus();

    void handleGetRecentUserStatuses();
    void handleGetUserStatus();
    void handleUpdateUserStatus();
    void handleDeleteUserStatus();

signals:
    void requestFinished();

    void requestTokenChanged(const QString& requestToken);
    void accessTokensChanged(const QString& accessToken, const QString& accessTokenSecret);

    void gotAuthUserInfo(quint64 id, const QString& name, const QString& link);
    void gotUserProfile(const UserPtr& profile);
//    void gotFriendsUpdates(const & updates);

    void gotUserBookShelves(quint64 userId, const BookShelves_t& shelves);
    void bookShelfAdded(const BookShelf& shelf);
    void bookShelfEdited(const BookShelf& shelf);

    void gotReviews(quint64 bookShelfId, const CountedItems<Review>& reviews);

    void gotBook(const Book& book);

    void gotUserGroups(quint64 userId, const CountedItems<Group>& groups);
    void gotUserGroup(quint64 groupId, const Group& group);
    void gotFoundGroups(const CountedItems<Group>& groups);
    void gotGroupMembers(quint64 groupId, const GroupMembers_t& members);
    void gotGroupFolderTopics(quint64 groupFolderId, quint64 groupId,
            const CountedItems<Topic>& topics);
    void gotGroupFolderTopic(const Topic& topic);
    void gotNewGroupFolderTopic(const Topic& topic);

    void newCommentAdded(const Comment& comment);

    void gotUserFriends(quint64 userId, const Friends_t& friends);
};
}
