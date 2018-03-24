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
#include "objects/group.h"
#include "objects/friend.h"

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

    void GetGroups(quint64 userId);
    void GetGroup(quint64 groupId, const QString& groupName);
    void SearchGroup(const QString& text, int page);
    void GetGroupMembers(quint64 groupId, int page);

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

    void handleGetGroups(quint64 userId);
    void handleGetGroup(quint64 groupId, QObject *senderObject = nullptr);
    void handleSearchGroup();
    void handleGetGroupMembers(quint64 groupId, QObject *senderObject = nullptr);

    void handleGetFriends(quint64 userId);

signals:
    void requestFinished();

    void requestTokenChanged(const QString& requestToken);
    void accessTokensChanged(const QString& accessToken, const QString& accessTokenSecret);

    void gotAuthUserInfo(quint64 id, const QString& name, const QString& link);
    void gotUserProfile(std::shared_ptr<UserProfile> profile);
//    void gotFriendsUpdates(const & updates);

    void gotUserBookShelves(quint64 userId, const BookShelves_t& shelves);
    void bookShelfAdded(const BookShelf& shelf);
    void bookShelfEdited(const BookShelf& shelf);

    void gotUserGroups(quint64 userId, const Groups& groups);
    void gotUserGroup(quint64 groupId, const Group& group);
    void gotFoundGroups(const Groups& groups);
    void gotGroupMembers(quint64 groupId, const GroupMembers_t& members);

    void gotUserFriends(quint64 userId, const Friends_t& friends);
};
}
