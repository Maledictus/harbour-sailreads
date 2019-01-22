/*
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

#include <QObject>

#include "../itemrequestcanceler.h"
#include "../types.h"

namespace Sailreads
{
class UserStatusItem: public ItemRequestCanceler
{
    Q_OBJECT

    QString m_UserStatusId;
    UserStatusPtr m_UserStatus;

    Q_PROPERTY(QString userStatusId READ GetUserStatusId WRITE SetUserStatusId NOTIFY userStatusIdChanged)
    Q_PROPERTY(UserStatus* userStatus READ GetUserStatus NOTIFY userStatusChanged)
public:
    UserStatusItem(QObject *parent = nullptr);

    QString GetUserStatusId() const;
    void SetUserStatusId(const QString& readStatusId);
    UserStatus* GetUserStatus() const;
    void SetUserStatus(const UserStatusPtr& readStatus);

private slots:
    void handleGotUserStatus(const UserStatusPtr& userStatus);
public slots:
    void loadUserStatus();

signals:
    void userStatusIdChanged();
    void userStatusChanged();
};

} // namespace Sailreads
