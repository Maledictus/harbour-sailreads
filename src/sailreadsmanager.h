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

#include <QObject>
#include <QVariantMap>

#include "objects/bookshelf.h"
#include "objects/friend.h"
#include "objects/group.h"

namespace Sailreads
{
class GoodReadsApi;
class UserProfile;

class SailreadsManager : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(SailreadsManager)

    GoodReadsApi *m_Api;

    bool m_IsBusy;
    bool m_IsLogged;

    QString m_AccessToken;
    QString m_AccessSecretToken;

    UserProfile *m_Profile;

    Q_PROPERTY(bool busy READ GetBusy NOTIFY busyChanged)
    Q_PROPERTY(bool logged READ GetLogged NOTIFY loggedChanged)
    Q_PROPERTY(UserProfile* userProfile READ GetProfile NOTIFY profileChanged)

    explicit SailreadsManager(QObject *parent = 0);

public:
    static SailreadsManager* Instance(QObject *parent = 0);

    bool GetBusy() const;
    bool GetLogged() const;

    UserProfile* GetProfile() const;

private:
    void MakeConnections();
    void SetBusy(bool busy);
    void SetLogged(bool logged);

public slots:
    void obtainRequestToken();
    void requestAccessToken();

    void authUser();
    void getUserInfo(quint64 id);
    void getUpdates();

    void loadBookShelves(quint64 id);
    void addBookShelf(const QString& name, bool exclusive);
    void editBookShelf(quint64 id, const QString& name, bool exclusive);

    void loadFriends(quint64 userId);

    void loadGroups(quint64 userId);

signals:
    void busyChanged();
    void loggedChanged();
    void profileChanged();

    void gotUserProfile();

    void gotUserBookShelves(quint64 userId, const BookShelves_t& shelves);
    void bookShelfAdded(const BookShelf& shelf);
    void bookShelfEdited(const BookShelf& shelf);

    void gotUserFriends(quint64 userId, const Friends_t& friends);

    void gotUserGroups(quint64 userId, const Groups_t& groups);

    void requestTokenChanged(const QString& requestToken);

    void authProgressChanged(const QString& progressMessage);
};
}

