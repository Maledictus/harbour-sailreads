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

#include <memory>

#include <QObject>
#include <QVariantMap>

#include "objects/bookshelf.h"
#include "objects/comment.h"
#include "objects/friend.h"
#include "objects/friendrequest.h"
#include "objects/friendrecommendation.h"
#include "objects/groupmember.h"
#include "types.h"

namespace Sailreads
{
class GoodReadsApi;

class SailreadsManager : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(SailreadsManager)

    GoodReadsApi *m_Api;

    bool m_IsBusy;
    bool m_IsLogged;

    QString m_AccessToken;
    QString m_AccessSecretToken;

    UserPtr m_AuthUser;

    Q_PROPERTY(bool busy READ GetBusy NOTIFY busyChanged)
    Q_PROPERTY(bool logged READ GetLogged NOTIFY loggedChanged)
    Q_PROPERTY(User* authUser READ GetAuthUser NOTIFY authUserChanged)

    explicit SailreadsManager(QObject *parent = 0);

public:
    static SailreadsManager* Instance(QObject *parent = 0);

    bool GetBusy() const;
    bool GetLogged() const;

    User* GetAuthUser() const;

private:
    void MakeConnections();
    void SetBusy(bool busy);
    void SetLogged(bool logged);

public slots:
    void handleGotAuthAnswer(const QString& data);

    void obtainRequestToken();
    void requestAccessToken();

    void abortRequest(QObject *requester);

    void authenticateUser();
    void logout();

    void getUserInfo(QObject *requester, const QString& id);
    void loadUpdates(QObject *requester, const QString& scope, const QString& items,
            const QDateTime& dt = QDateTime());
    void loadUserFollowings(QObject *requester, const QString& userId, int page = 1);
    void loadUserFollowers(QObject *requester, const QString& userId, int page = 1);

    void loadMessages(QObject *requester, const QString& folder, int page = 1);
    void loadMessage(QObject *requester, quint64 messageId);
    void markMessageAsRead(quint64 messageId);

    void loadNotifications(QObject *requester, const QString& pageToken);

    void loadFriendsRequests(QObject *requester, int page = 1);
    void loadFriendsRecommendations(QObject *requester, int page = 1);

    void loadBookShelves(QObject *requester, const QString& id, int page = 1);
    void loadAllBookShelves(QObject *requester, const QString& id, int page = 1);
    void addBookShelf(const QString& name, bool exclusive, bool sortable, bool featured,
            bool recommendFor);
    void editBookShelf(quint64 id, const QString& name, bool exclusive, bool sortable, bool featured,
            bool recommendFor);

    void loadReviews(QObject *requester, const QString& userId, const QString& bookShelf, int page = 1,
            Qt::SortOrder order = Qt::DescendingOrder, const QString& sortField = "date_added");
    void loadReview(QObject *requester, const QString& reviewId, int commentsPage = 1);
    void searchReviews(QObject *requester, const QString& userId,
            const QString& searchText, int page = 1);
    void addReview(const QString& bookId, int rating, const QString& reviewText = QString());
    void editReview(const QString& reviewId, int rating, const QString& reviewText);
    void removeReview(const QString& bookId, const QString& reviewId);

    void loadBook(QObject *requester, const QString& bookId);
    void loadBookEditions(QObject *requester, quint64 workId, int page = 1);

    void loadSeries(QObject *requester, quint64 seriesId);

    void loadFriends(QObject *requester, const QString& userId, int page = 1);
    void confirmFriendRequest(quint64 friendRequestId, bool confirm);
    void confirmFriendRecommendationRequest(quint64 friendRecommendationId, bool confirm);
    void followUser(const QString& userId);
    void unfollowUser(const QString& userId);
    void addAsFriend(const QString& userId);
    void removeFriend(const QString& userId);

    void loadGroups(QObject *requester, const QString& userId, int page = 1);
    void loadGroup(QObject *requester, quint64 groupId);
    void joinGroup(quint64 groupId);
    void searchGroup(QObject *requester, const QString& text, int page = 1);
    void loadGroupMembers(QObject *requester, quint64 groupId, int page = 1);
    void loadGroupFolderTopics(QObject *requester, const QString& groupFolderId,
            quint64 groupId, int page = 1);
    void loadGroupFolderTopic(QObject *requester, const QString& topicId, int page = 1);

    void addNewTopic(const QString& topic, const QString& subject, quint64 subjectId,
            const QString& folderId, bool question, bool updateFeed, bool digest,
            const QString& comment);

    void loadComments(QObject *requester, const QString& resourceId,
            const QString& resourceType, int page = 1);
    void addNewComment(const QString& type, const QString& resourceId, const QString& comment);

    void addBookToShelf(const QString& bookId, const QString& shelf);
    void addBookToShelves(const QString& bookId, const QStringList& shelves);
    void removeBookFromShelf(const QString& bookId, const QString& shelf);

    void loadAuthorProfile(QObject *requester, const QString& authorId);
    void loadAuthorBooks(QObject *requester, const QString& authorId, int page = 1);
    void loadAuthorSeries(QObject *requester, const QString& authorId);
    void followAuthor(const QString& authorId);
    void unfollowAuthor(const QString& authorId, quint64 followingId);

    void loadReadStatus(QObject *requester, const QString& id, int page = 1);

    void loadUserStatus(QObject *requester, const QString& id, int page = 1);

    void loadRecommendation(QObject *requester, const QString& id, int page = 1);

    void likeResource(const QString& resourceId, const QString& resourceType);
    void unlikeResource(const QString& resourceId, quint64 ratingId);
signals:
    void busyChanged();
    void loggedChanged();
    void authUserChanged();

    void gotAuthUserId(const QString& authUserId);
    void gotUserProfile(const UserPtr& userProfile);

    void gotUpdates(const Updates_t& updates);
    void gotUserUpdates(const QString& userId, const Updates_t& updates);

    void gotMessages(const QString& folder, const CountedItems<MessagePtr>& messages);
    void gotMessage(const MessagePtr& message);
    void gotNotifications(const QString& nextPageToken, const Notifications_t& notifications);
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

    void gotAuthorProfile(const AuthorPtr& author);
    void authorFollowed(const QString& authorId, quint64 followingId);
    void authorUnfollowed(const QString& authorId);
    void gotAuthorBooks(const QString& authorId, const CountedItems<BookPtr>& books);
    void gotAuthorSeries(const QString& authorId, const Series_t& series);

    void gotSeries(const SeriesPtr& series);

    void gotUserFriends(const QString& userId, const CountedItems<Friend>& friends);
    void gotUserFollowings(const QString& userId, const CountedItems<Friend>& friends);
    void gotUserFollowers(const QString& userId, const CountedItems<Friend>& friends);
    void friendRequestConfirmed(quint64 friendRequestId, bool confirmed);
    void friendRecommendationConfirmed(quint64 friendRecommendationId, bool confirmed);
    void userFollowed(const QString& userId, bool success);
    void userUnfollowed(const QString& userId, bool success);
    void friendAdded(const QString& userId);
    void friendRemoved(const QString& userId);

    void gotComments(const QString& resourceId, const CountedItems<Comment>& comments);
    void newCommentAdded(const QString& resourceId, const Comment& comment);

    void gotUserGroups(const QString& userId, const CountedItems<GroupPtr>& groups);
    void gotUserGroup(quint64 inGroupId, const GroupPtr& inGroup);
    void gotFoundGroups(const CountedItems<GroupPtr>& groups);
    void gotGroupMembers(quint64 groupId, const GroupMembers_t& members);
    void gotGroupFolderTopics(const QString& groupFolderId, quint64 groupId,
            const CountedItems<TopicPtr>& topics);
    void gotGroupFolderTopic(const TopicPtr& inTopic);
    void gotNewGroupFolderTopic(const QString& groupFolderId, const TopicPtr& inTopic);

    void requestTokenChanged(const QString& requestToken);

    void authProgressChanged(const QString& progressMessage);

    void gotReadStatus(const ReadStatusPtr& readStatus);

    void gotUserStatus(const UserStatusPtr& userStatus);

    void gotRecommendation(const RecommendationPtr& recommendation);

    void likeAdded(const QString& resourceId, quint64 ratingId);
    void likeRemoved(const QString& resourceId);

    void bookAddedToShelves(const QString& bookId, const ReviewPtr& review);
};
}

