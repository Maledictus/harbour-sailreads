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

#pragma once

#include <functional>
#include <memory>

#include <QDomDocument>
#include <QMap>
#include <QObject>
#include <QUrl>
#include <QPointer>

#include "objects/bookshelf.h"
#include "objects/comment.h"
#include "objects/friend.h"
#include "objects/friendrequest.h"
#include "objects/friendrecommendation.h"
#include "objects/groupfolder.h"
#include "objects/groupmember.h"
#include "types.h"

class QNetworkAccessManager;
class QNetworkReply;

namespace Sailreads
{
class OAuth1;

class GoodReadsApi : public QObject
{
    Q_OBJECT

    static const QString UnreadTopicsFolderId;

    const QString m_ConsumerKey;
    const QString m_ConsumerSecretKey;
    const QString m_BaseUrl;

    QString m_RequestToken;
    QString m_RequestTokenSecret;

    QString m_AccessToken;
    QString m_AccessTokenSecret;

    QNetworkAccessManager *m_NAM;
    OAuth1 *m_OAuth1;

    QMap<QObject*, QNetworkReply*> m_Requester2Reply;

public:
    explicit GoodReadsApi(QObject *parent = 0);

    void AbortRequest(QObject *object);

    void RequestRedirectedUrl(const QUrl& url, const std::function<void(QObject*)>& func);

    void UpdateCredentials(const QString& accessToken, const QString& accessTokenSecret);

    void ObtainRequestToken() const;
    void RequestAccessToken() const;

    void AuthUser();
    void GetUserInfo(QObject *requester, const QString& id);
    void CompareBooks(const QString& userId);

    void GetUpdates(QObject *requester, const QString& scope, const QString& items,
            const QDateTime& updateDt);
    void GetNotifications(QObject *requester, const QString& pageToken);

    void GetMessages(QObject *requester, const QString& folder, int page);
    void GetMessage(QObject *requester, quint64 messageId);
    void MarkMessageAsRead(quint64 messageId);

    void GetBookShelves(QObject *requester, const QString& userId, int page = 1);
    void GetAllBookShelves(QObject *requester, const QString& userId, int page = 1);
    void AddBookShelf(const QString& name, bool exclusive, bool sortable, bool featured,
            bool recommendFor);
    void EditBookShelf(quint64 id, const QString& name, bool exclusive, bool sortable, bool featured,
            bool recommendFor);

    void GetReviews(QObject *requester, const QString& userId, const QString& bookShelf,
            const QString& sortField = "date_added", Qt::SortOrder order = Qt::DescendingOrder,
            int page = 1);
    void GetReview(QObject *requester, const QString& reviewId, int commentsPage);
    void SearchReviews(QObject *requester, const QString& userId, const QString& searchText, int page = 1);

    void AddReview(const QString& bookId, int rating, const QString& review);
    void EditReview(const QString& reviewId, int rating, const QString& review);
    void DeleteReview(const QString& bookId, const QString& reviewId);

    void GetBook(QObject *requester, const QString& bookId);
    void GetBookEditions(QObject *requester, quint64 workId, int page);
    void SearchBooks(QObject *requester, const QString& query, const QString& key, int page);

    void GetSeries(QObject *requester, quint64 seriesId);
    void GetAuthorSeries(QObject *requester, const QString& authorId);
    void GetWorkSeries(QObject *requester, quint64 workId);

    void AddBookToShelf(const QString& bookId, const QString& shelfName,
            const QStringList& oldShelves);
    void AddBooksToShelves(const QStringList& bookIds, const QStringList& shelvesName,
            const QStringList& oldShelves);
    void RemoveBookFromShelf(const QString& bookId, const QString& shelfName);

    void GetAuthor(QObject *requester, const QString& authorId);
    void GetAuthorBooks(QObject *requester, const QString& authorId, int page = 1);
    void FollowAuthor(const QString& authorId);
    void UnfollowAuthor(const QString& authorId, quint64 authorFollowingId);

    void GetGroups(QObject *requester, const QString& userId, int page = 1);
    void GetGroup(QObject *requester, quint64 groupId);
    void JoinGroup(quint64 groupId);
    void SearchGroup(QObject *requester, const QString& text, int page);
    void GetGroupMembers(QObject *requester, quint64 groupId, int page);
    void GetGroupFolderTopics(QObject *requester, const QString& groupFolderId,
            quint64 groupId, int page);
    void GetGroupFolderTopic(QObject *requester, const QString& topicId, int page);
    void AddNewTopic(const QString& topic, const QString& subject, quint64 subjectId,
            const QString& folderId, bool question, bool updateFeed, bool digest,
            const QString& comment);

    void GetComments(QObject *requester, const QString& resourceId, const QString& resourceType,
            int page = 1);
    void AddNewComment(const QString& type, const QString& resourceId, const QString& comment);

    void GetFriends(QObject *requester, const QString& userId, int page = 1);
    void GetUserFollowers(QObject *requester, const QString& userId, int page = 1);
    void GetUserFollowings(QObject *requester, const QString& userId, int page = 1);
    void GetFriendRequests(QObject *requester, int page);
    void ConfirmFriendRequest(quint64 friendRequestId, bool confirm);
    void ConfirmFriendRecommendation(quint64 friendRecommendationId, bool confirm);
    void AddFriend(const QString& userId);
    void RemoveFriend(const QString& userId);
    void FollowUser(const QString& userId);
    void UnfollowUser(const QString& userId);

    void AddQuote(const QString& authorName, quint64 authorId, quint64 bookId, const QString& quote,
        const QStringList& tags);

    void GetReadStatus(QObject *requester, const QString& readStatusId, int page = 1);

    void GetRecentUserStatuses(QObject *requester);
    void GetUserStatus(QObject *requester, const QString& userStatusId, int page = 1);
    void UpdateUserStatus(quint64 bookId, const QString& body, int percent, int page = -1);
    void DeleteUserStatus(quint64 userStatusId);

    void GetRecommendation(QObject *requester, const QString& id, int page = 1);

    void LikeResource(const QString& resourceId, const QString& resourceType);
    void UnlikeResource(const QString& resourceId, quint64 ratingId);

private:
    QDomDocument GetDocumentFromReply(QObject *sender, bool& ok);
    QByteArray GetReply(QObject *sender, bool& ok);

private slots:
    void handleObtainRequestToken();
    void handleRequestAccessToken();

    void handleAuthUser();
    void handleGetUserInfo();
    void handleCompareBooks();

    void handleGetUpdates();
    void handleGetNotifications();

    void handleGetMessages();
    void handleGetMessage();
    void handleMarkMessageAsRead();

    void handleGetBookShelves(const QString& userId);
    void handleGetAllBookShelves(QObject *requester, const QString& userId, int page);
    void handleAddBookShelf();
    void handleEditBookShelf();

    void handleGetReviews();
    void handleGetReview();
    void handleSearchReviews();
    void handleAddReview();
    void handleEditReview();
    void handleDeleteReview(const QString& reviewId);

    void handleGetBook();
    void handleGetBookEditions(quint64 workId);
    void handleSearchBook();

    void handleGetSeries();
    void handleGetAuthorSeries(const QString& authorId);
    void handleGetWorkSeries();

    void handleAddBookToShelf(const QString& bookId, const QString& shelfName,
            const QStringList& oldShelves);
    void handleAddBooksToShelves(const QStringList& bookIds, const QStringList& shelvesName,
            const QStringList& oldShelves);
    void handleRemoveBookFromShelf(const QString& bookId);

    void handleGetAuthor();
    void handleGetAuthorBooks(const QString& authorId);
    void handleFollowAuthor();
    void handleUnfollowAuthor(const QString& authorId);

    void handleGetGroups(const QString& userId);
    void handleGetGroup(quint64 groupId, QObject *senderObject = nullptr);
    void handleJoinGroup(quint64 groupId);
    void handleSearchGroup();
    void handleGetGroupMembers(quint64 groupId, QObject *senderObject = nullptr);
    void handleGetGroupFolderTopics(const QString& groupFolderId, quint64 groupId);
    void handleGetGroupFolderTopic();
    void handleTopicAdded(const QString &folderId);

    void handleGetComments(const QString& resourceId);
    void handleNewCommentAdded(const QString& resourceId);

    void handleGetFriends(const QString& userId);
    void handleGetUserFollowers(const QString& userId);
    void handleGetUserFollowings(const QString& userId);
    void handleGetFriendRequests();
    void handleConfirmFriendRequest(quint64 friendRequestId, bool confirm);
    void handleConfirmFriendRecommendation(quint64 friendRecommendationId, bool confirm);
    void handleAddFriend(const QString& userId);
    void handleRemoveFriend(const QString& userId);
    void handleFollowUser();
    void handleUnfollowUser(const QString& userId);

    void handleAddQuote();

    void handleGetReadStatus();

    void handleGetRecentUserStatuses();
    void handleGetUserStatus();
    void handleUpdateUserStatus();
    void handleDeleteUserStatus();

    void handleGetRecommendation();

    void handleLikeResource(const QString& resourceId);
    void handleUnlikeResource(const QString& resourceId);

signals:
    void requestFinished();

    void authenticationFailed();

    void requestTokenChanged(const QString& requestToken);
    void accessTokensChanged(const QString& accessToken, const QString& accessTokenSecret);

    void gotAuthUserInfo(const QString& id, const QString& name, const QString& link);
    void gotUserProfile(const UserPtr& profile);
    void gotUpdates(const Updates_t& updates);

    void gotNotifications(const QString& pageToken, const Notifications_t& notifications);

    void gotMessages(const QString& folder, const CountedItems<MessagePtr>& messages);
    void gotMessage(const MessagePtr& message);

    void gotFriendsRequests(const CountedItems<FriendRequest>& requests);
    void gotFriendsRecommendations(const CountedItems<FriendRecommendation>& recommendations);

    void gotUserBookShelves(const QString& userId, const CountedItems<BookShelf>& shelves);
    void bookShelfAdded(const BookShelf& shelf);
    void bookShelfEdited(const BookShelf& shelf);

    void gotReviews(quint64 bookShelfId, const CountedItems<ReviewPtr>& reviews);
    void gotReview(const ReviewPtr& review);
    void gotFoundReviews(const CountedItems<ReviewPtr>& reviews);
    void gotReviewInfo(const ReviewInfo& reviewInfo);

    void gotBook(const BookPtr& book);
    void gotBookEditions(quint64 workId, const CountedItems<BookPtr>& books);
    void gotFoundBooks(const CountedItems<BookPtr>& books);

    void gotAuthorProfile(const AuthorPtr& authorProfile);
    void gotAuthorBooks(const QString& authorId, const CountedItems<BookPtr>& books);
    void authorFollowed(const QString& authorId, quint64 followingId);
    void authorUnfollowed(const QString& authorId);

    void gotSeries(const SeriesPtr& series);
    void gotAuthorSeries(const QString& authorId, const Series_t& seriesList);
    void gotWorkSeries(const Series_t& seriesList);

    void gotUserGroups(const QString& userId, const CountedItems<GroupPtr>& groups);
    void gotUserGroup(quint64 groupId, const GroupPtr& group);
    void gotFoundGroups(const CountedItems<GroupPtr>& groups);
    void gotGroupMembers(quint64 groupId, const GroupMembers_t& members);
    void gotGroupFolderTopics(const QString& groupFolderId, quint64 groupId,
            const CountedItems<TopicPtr>& topics);
    void gotGroupFolderTopic(const TopicPtr& topic);
    void gotNewGroupFolderTopic(const QString& folderId, const TopicPtr& topic);

    void gotComments(const QString& resourceId, const CountedItems<Comment>& comments);
    void newCommentAdded(const QString& resourceId, const Comment& comment);

    void gotUserFriends(const QString& userId, const CountedItems<Friend>& friends);
    void gotUserFollowings(const QString& userId, const CountedItems<Friend>& followings);
    void gotUserFollowers(const QString& userId, const CountedItems<Friend>& followers);
    void friendRequestConfirmed(quint64 friendRequestId, bool confirmed);
    void friendRecommendationConfirmed(quint64 friendRecommendationId, bool confirmed);
    void userFollowed(const QString& userId, bool success);
    void userUnfollowed(const QString& userId, bool success);
    void friendAdded(const QString& userId);
    void friendRemoved(const QString& userId);

    void gotReadStatus(const ReadStatusPtr& readStatus);

    void gotUserStatus(const UserStatusPtr& userStatus);

    void gotRecommendation(const RecommendationPtr& recommendation);

    void likeAdded(const QString& resourceId, quint64 ratingId);
    void likeRemoved(const QString& resourceId);

    void bookAddedToShelf(const QString& bookId, const QString& shelf,
            const QStringList& oldShelves, const ReviewPtr& review);
    void bookAddedToShelves(const QString& bookId, const QStringList& shelves,
            const QStringList& oldShelves, const ReviewPtr& review);
    void bookRemovedFromShelf(const QString& bookId, const QString& shelfName);
};
}
