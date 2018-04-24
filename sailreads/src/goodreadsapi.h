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

#include <QObject>
#include <QUrl>

#include "userprofile.h"
#include "objects/book.h"
#include "objects/comment.h"
#include "objects/group.h"
#include "objects/friend.h"
#include "objects/review.h"
#include "objects/topic.h"

class QNetworkAccessManager;

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

public:
    explicit GoodReadsApi(QObject *parent = 0);

    void RequestRedirectedUrl(const QUrl& url, const std::function<void(QObject*)>& func);

    void UpdateCredentials(const QString& accessToken, const QString& accessTokenSecret);

    void ObtainRequestToken() const;
    void RequestAccessToken() const;

    void AuthUser();
    void GetUserInfo(quint64 id);

    void GetUpdates();

    void GetBookShelves(quint64 userId);
    void AddBookShelf(const QString& name, bool exclusive);
    void EditBookShelf(quint64 id, const QString& name, bool exclusive);

    void GetReviews(quint64 userId, const QString& bookShelf, const QString& sortField = "date_added",
            Qt::SortOrder order = Qt::DescendingOrder, int page = 1);

    void GetBook(quint64 bookId);

    void GetGroups(quint64 userId);
    void GetGroup(quint64 groupId, const QString& groupName);
    void SearchGroup(const QString& text, int page);
    void GetGroupMembers(quint64 groupId, int page);
    void GetGroupFolderTopics(quint64 groupFolderId, quint64 groupId, int page);
    void GetGroupFolderTopic(quint64 topicId, int page);
    void AddNewTopic(const QString& topic, const QString& subject, quint64 subjectId,
            quint64 folderId, bool question, bool updateFeed, bool digest, const QString& comment);

    void AddNewComment(const QString& type, quint64 resourceId, const QString& comment);

    void GetFriends(quint64 userId);

private slots:
    void handleObtainRequestToken();
    void handleRequestAccessToken();

    void handleAuthUser();
    void handleGetUserInfo();
    void handleGetUpdates();

    void handleGetBookShelves(quint64 userId);
    void handleAddBookShelf();
    void handleEditBookShelf();

    void handleGetReviews();

    void handleGetBook();

    void handleGetGroups(quint64 userId);
    void handleGetGroup(quint64 groupId, QObject *senderObject = nullptr);
    void handleSearchGroup();
    void handleGetGroupMembers(quint64 groupId, QObject *senderObject = nullptr);
    void handleGetGroupFolderTopics(quint64 groupFolderId, quint64 groupId);
    void handleGetGroupFolderTopic();

    void handleTopicAdded();

    void handleNewCommentAdded();

    void handleGetFriends(quint64 userId);

signals:
    void requestFinished();

    void requestTokenChanged(const QString& requestToken);
    void accessTokensChanged(const QString& accessToken, const QString& accessTokenSecret);

    void gotAuthUserInfo(quint64 id, const QString& name, const QString& link);
    void gotUserProfile(const User& profile);
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
