/*
The MIT License(MIT)

Copyright (c) 2018 Oleg Linkin <maledictusdemagog@gmail.com>

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

#include "userprofile.h"

#include <QtDebug>

#include "models/bookshelvesmodel.h"
#include "objects/user.h"
#include "sailreadsmanager.h"

namespace Sailreads
{
UserProfile::UserProfile(QObject *parent)
: QObject(parent)
, m_UserId(0)
{
    connect(SailreadsManager::Instance(), &SailreadsManager::gotUserProfile,
            this, &UserProfile::handleGotUser);
}

quint64 UserProfile::GetUserID() const
{
    return m_UserId;
}

void UserProfile::SetUserID(quint64 id)
{
    if (m_UserId != id) {
        m_UserId = id;
        updateProfile();
        emit userIdChanged();
    }
}

User* UserProfile::GetUser() const
{
    return m_User.get();
}

void UserProfile::SetUser(const UserPtr& user)
{
    if (m_User != user) {
        m_User = user;
        return userChanged();
    }
}

void UserProfile::handleGotUser(const UserPtr& user)
{
    if (!user || user->GetId() != m_UserId) {
        return;
    }
    SetUser(user);
}

void UserProfile::updateProfile()
{
    SailreadsManager::Instance()->getUserInfo(m_UserId);
}
} // namespace Sailreads
