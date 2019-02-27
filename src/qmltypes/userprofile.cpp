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

#include "userprofile.h"

#include <QtDebug>

#include "../models/bookshelvesmodel.h"
#include "../objects/user.h"
#include "../sailreadsmanager.h"

namespace Sailreads
{
UserProfile::UserProfile(QObject *parent)
: ItemRequestCanceler(parent)
, m_UserId("")
{
    connect(SailreadsManager::Instance(), &SailreadsManager::gotUserProfile,
            this, &UserProfile::handleGotUser);
    connect(SailreadsManager::Instance(), &SailreadsManager::friendRequestConfirmed,
            this, &UserProfile::handleFriendRequestCofirmed);
    connect(SailreadsManager::Instance(), &SailreadsManager::userFollowed,
            this, &UserProfile::handleUserFollowed);
    connect(SailreadsManager::Instance(), &SailreadsManager::userUnfollowed,
            this, &UserProfile::handleUserUnfollowed);
    connect(SailreadsManager::Instance(), &SailreadsManager::friendAdded,
            this, &UserProfile::handleFriendAdded);
    connect(SailreadsManager::Instance(), &SailreadsManager::friendRemoved,
            this, &UserProfile::handleFriendRemoved);
}

QString UserProfile::GetUserID() const
{
    return m_UserId;
}

void UserProfile::SetUserID(const QString& id)
{
    if (m_UserId != id) {
        m_UserId = id;
        loadProfile(true);
        emit userIdChanged();
    }
}

User* UserProfile::GetUser() const
{
    return m_User.get();
}

void UserProfile::SetUser(const UserPtr& user)
{
    if (!user) {
        return;
    }

    if (!m_User || !m_User->IsEqual(user)) {
        m_User = user;
        return userChanged();
    }
}

void UserProfile::handleGotUser(const UserPtr& user)
{
    if (!user || (user->GetId() != m_UserId && !m_UserId.startsWith(user->GetId()))) {
        return;
    }

    m_UserId = user->GetId();
    emit userIdChanged();
    SetUser(user);
}

void UserProfile::handleFriendRequestCofirmed(quint64 friendRequestId, bool)
{
    if (m_User && m_User->GetFriendRequestId() == friendRequestId) {
        handleFriendAdded(m_UserId);
    }

    if (m_User && SailreadsManager::Instance()->GetAuthUser() &&
            m_UserId == SailreadsManager::Instance()->GetAuthUser()->GetId()) {
        m_User->SetFriendsCount(m_User->GetFriendsCount() + 1);
        emit userChanged();
    }
}

void UserProfile::handleUserFollowed(const QString& userId, bool success)
{
    if (!m_User || m_UserId != userId) {
        return;
    }

    m_User->SetIsFollowing(success);
    emit userChanged();
}

void UserProfile::handleUserUnfollowed(const QString& userId, bool success)
{
    if (!m_User || m_UserId != userId) {
        return;
    }

    m_User->SetIsFollowing(!success);
    emit userChanged();
}

void UserProfile::handleFriendAdded(const QString& userId)
{
    if (!m_User || m_UserId != userId) {
        return;
    }

    m_User->SetIsFriend(true);
    emit userChanged();
}

void UserProfile::handleFriendRemoved(const QString&)
{
    if (!m_User || !SailreadsManager::Instance()->GetAuthUser() ||
            m_UserId != SailreadsManager::Instance()->GetAuthUser()->GetId()) {
        return;
    }

    m_User->SetFriendsCount(m_User->GetFriendsCount() - 1);
    emit userChanged();
}

void UserProfile::loadProfile(bool useCache)
{
    if (m_UserId.isEmpty()) {
        return;
    }
    SailreadsManager::Instance()->getUserInfo(this, m_UserId, useCache);
}
} // namespace Sailreads
