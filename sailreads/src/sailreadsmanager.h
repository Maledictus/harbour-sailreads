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

#include <memory>

#include <QObject>
#include <QVariantMap>

#include "objects/book.h"
#include "objects/bookshelf.h"
#include "objects/friend.h"
#include "objects/group.h"
#include "objects/review.h"
#include "objects/topic.h"

namespace Sailreads
{
class GoodReadsApi;
class UserProfile;
class User;

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
    void obtainRequestToken();
    void requestAccessToken();

    void abortRequest();

    void authenticateUser();
    void getUserInfo(quint64 id);
    void getUpdates();

    void loadBookShelves(quint64 id);
    void addBookShelf(const QString& name, bool exclusive);
    void editBookShelf(quint64 id, const QString& name, bool exclusive);

    void loadReviews(quint64 userId, const QString& bookShelf, int page = 1,
            Qt::SortOrder order = Qt::DescendingOrder, const QString& sortField = "date_added");

    void loadBook(quint64 bookId);

    void loadFriends(quint64 userId);

    void loadGroups(quint64 userId);
    void loadGroup(quint64 groupId, const QString& groupName);
    void searchGroup(const QString& text, int page = 1);
    void loadGroupMembers(quint64 groupId, int page = 1);
    void loadGroupFolderTopics(quint64 groupFolderId, quint64 groupId, int page = 1);
    void loadGroupFolderTopic(quint64 topicId, int page = 1);

    void addNewTopic(const QString& topic, const QString& subject, quint64 subjectId, quint64 folderId,
            bool question, bool updateFeed, bool digest, const QString& comment);

    void addNewComment(const QString& type, quint64 resourceId, const QString& comment);
signals:
    void busyChanged();
    void loggedChanged();
    void authUserChanged();

    void gotAuthUserId(quint64 authUserId);
    void gotUserProfile(const UserPtr& userProfile);

    void gotUserBookShelves(quint64 userId, const BookShelves_t& shelves);
    void bookShelfAdded(const BookShelf& shelf);
    void bookShelfEdited(const BookShelf& shelf);

    void gotReviews(quint64 bookShelfId, const CountedItems<Review>& reviews);

    void gotBook(const BookPtr& book);

    void gotUserFriends(quint64 userId, const Friends_t& friends);

    void gotUserGroups(quint64 userId, const CountedItems<Group>& groups);
    void gotUserGroup(quint64 inGroupId, const Group& inGroup);
    void gotFoundGroups(const CountedItems<Group>& groups);
    void gotGroupMembers(quint64 groupId, const GroupMembers_t& members);
    void gotGroupFolderTopics(quint64 groupFolderId, quint64 groupId,
            const CountedItems<Topic>& topics);
    void gotGroupFolderTopic(const Topic& inTopic);
    void gotNewGroupFolderTopic(const Topic& inTopic);

    void requestTokenChanged(const QString& requestToken);

    void authProgressChanged(const QString& progressMessage);
};
}

