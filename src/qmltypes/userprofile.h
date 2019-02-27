/*
The MIT License(MIT)

Copyright (c) 2018-2019 Oleg Linkin <maledictusdemagog@gmail.com>

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files(the "Software"), to deal
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
#include <QString>
#include <QVariantMap>
#include <QUrl>

#include "../itemrequestcanceler.h"
#include "../types.h"

namespace Sailreads
{
class BookShelvesModel;
class User;

class UserProfile : public ItemRequestCanceler
{
    Q_OBJECT

    QString m_UserId;
    UserPtr m_User;

    Q_PROPERTY(QString userId READ GetUserID WRITE SetUserID NOTIFY userIdChanged)
    Q_PROPERTY(User* user READ GetUser NOTIFY userChanged)

public:
    explicit UserProfile(QObject *parent = nullptr);

    QString GetUserID() const;
    void SetUserID(const QString& id);
    User* GetUser() const;
    void SetUser(const UserPtr& user);

private slots:
    void handleGotUser(const UserPtr& user);
    void handleFriendRequestCofirmed(quint64 friendRequestId, bool confirmed);
    void handleUserFollowed(const QString& userId, bool success);
    void handleUserUnfollowed(const QString& userId, bool success);
    void handleFriendAdded(const QString& userId);
    void handleFriendRemoved(const QString& friendId);
public slots:
    void loadProfile(bool useCache = true);

signals:
    void userIdChanged();
    void userChanged();
};
} // namespace Sailreads
