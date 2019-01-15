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
#include "objects/friend.h"
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

    void abortRequest();

    void authenticateUser();
    void logout();

    void getUserInfo(quint64 id);
    void loadUpdates(const QString& scope, const QString& items, const QDateTime& dt = QDateTime());
    void loadUserFollowings(quint64 userId, int page = 1);
    void loadUserFollowers(quint64 userId, int page = 1);

    void loadMessages(const QString& folder, int page = 1);
    void loadMessage(quint64 messageId);
    void markMessageAsRead(quint64 messageId);

    void loadNotifications(const QString& pageToken);

    void loadBookShelves(quint64 id, int page = 1);
    void addBookShelf(const QString& name, bool exclusive, bool sortable, bool featured,
            bool recommendFor);
    void editBookShelf(quint64 id, const QString& name, bool exclusive, bool sortable, bool featured,
            bool recommendFor);

    void loadReviews(quint64 userId, const QString& bookShelf, int page = 1,
            Qt::SortOrder order = Qt::DescendingOrder, const QString& sortField = "date_added");
    void loadReview(quint64 reviewId, int commentsPage = 1);
    void searchReviews(quint64 userId, const QString& searchText, int page = 1);

    void loadBook(quint64 bookId);
    void loadBookEditions(quint64 workId, int page = 1);

    void loadSeries(quint64 seriesId);

    void loadFriends(quint64 userId, int page = 1);
    void confirmFriendRequest(quint64 friendRequestId, bool confirm);
    void followUser(quint64 id);
    void unfollowUser(quint64 id);
    void addAsFriend(quint64 id);
    void removeFriend(quint64 id);

    void loadGroups(quint64 userId, int page = 1);
    void loadGroup(quint64 groupId);
    void joinGroup(quint64 groupId);
    void searchGroup(const QString& text, int page = 1);
    void loadGroupMembers(quint64 groupId, int page = 1);
    void loadGroupFolderTopics(const QString& groupFolderId, quint64 groupId, int page = 1);
    void loadGroupFolderTopic(quint64 topicId, int page = 1);

    void addNewTopic(const QString& topic, const QString& subject, quint64 subjectId,
            const QString& folderId, bool question, bool updateFeed, bool digest,
            const QString& comment);

    void addNewComment(const QString& type, quint64 resourceId, const QString& comment);

    void addBookToShelves(quint64 bookId, const QStringList& shelves);

    void loadAuthorProfile(quint64 authorId);
    void loadAuthorBooks(quint64 authorId, int page = 1);
    void loadAuthorSeries(quint64 authorId);
    void followAuthor(quint64 authorId);
    void unfollowAuthor(quint64 authorId, quint64 followingId);

signals:
    void busyChanged();
    void loggedChanged();
    void authUserChanged();

    void gotAuthUserId(quint64 authUserId);
    void gotUserProfile(const UserPtr& userProfile);

    void gotUpdates(const Updates_t& updates);

    void gotMessages(const QString& folder, const CountedItems<MessagePtr>& messages);
    void gotMessage(const MessagePtr& message);
    void gotNotifications(const QString& nextPageToken, const Notifications_t& notifications);

    void gotUserBookShelves(quint64 userId, const CountedItems<BookShelf>& shelves);
    void bookShelfAdded(const BookShelf& shelf);
    void bookShelfEdited(const BookShelf& shelf);

    void gotReviews(quint64 bookShelfId, const CountedItems<ReviewPtr>& reviews);
    void gotReview(const ReviewPtr& review);
    void gotFoundReviews(const CountedItems<ReviewPtr>& reviews);

    void gotBook(const BookPtr& book);
    void gotBookEditions(quint64 workId, const CountedItems<BookPtr>& books);

    void gotAuthorProfile(const AuthorPtr& author);
    void authorFollowed(quint64 authorId, quint64 followingId);
    void authorUnfollowed(quint64 authorId);
    void gotAuthorBooks(quint64 authorId, const CountedItems<BookPtr>& books);
    void gotAuthorSeries(quint64 authorId, const Series_t& series);

    void gotSeries(const SeriesPtr& series);

    void gotUserFriends(quint64 userId, const CountedItems<Friend>& friends);
    void gotUserFollowings(quint64 userId, const CountedItems<Friend>& friends);
    void gotUserFollowers(quint64 userId, const CountedItems<Friend>& friends);
    void friendRequestConfirmed(quint64 friendRequestId, bool confirmed);
    void userFollowed(quint64 userId, bool success);
    void userUnfollowed(quint64 userId, bool success);
    void friendAdded(quint64 userId);
    void friendRemoved(quint64 userId);

    void gotUserGroups(quint64 userId, const CountedItems<GroupPtr>& groups);
    void gotUserGroup(quint64 inGroupId, const GroupPtr& inGroup);
    void gotFoundGroups(const CountedItems<GroupPtr>& groups);
    void gotGroupMembers(quint64 groupId, const GroupMembers_t& members);
    void gotGroupFolderTopics(const QString& groupFolderId, quint64 groupId,
            const CountedItems<TopicPtr>& topics);
    void gotGroupFolderTopic(const TopicPtr& inTopic);
    void gotNewGroupFolderTopic(const TopicPtr& inTopic);

    void requestTokenChanged(const QString& requestToken);

    void authProgressChanged(const QString& progressMessage);
};
}

