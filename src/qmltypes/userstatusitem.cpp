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

#include "userstatusitem.h"

#include "../objects/userstatus.h"
#include "../sailreadsmanager.h"

namespace Sailreads
{
UserStatusItem::UserStatusItem(QObject *parent)
: ItemRequestCanceler(parent)
{
    connect(SailreadsManager::Instance(this), &SailreadsManager::gotUserStatus,
            this, &UserStatusItem::handleGotUserStatus);
    connect(SailreadsManager::Instance(this), &SailreadsManager::likeAdded,
            this, &UserStatusItem::handleLikeAdded);
    connect(SailreadsManager::Instance(this), &SailreadsManager::likeRemoved,
            this, &UserStatusItem::handleLikeRemoved);
}

QString UserStatusItem::GetUserStatusId() const
{
    return m_UserStatusId;
}

void UserStatusItem::SetUserStatusId(const QString& userStatusId)
{
    if(m_UserStatusId != userStatusId) {
        m_UserStatusId = userStatusId;
        loadUserStatus();
        emit userStatusIdChanged();
    }
}

UserStatus *UserStatusItem::GetUserStatus() const
{
    return m_UserStatus.get();
}

void UserStatusItem::SetUserStatus(const UserStatusPtr &userStatus)
{
    m_UserStatus = userStatus;
    emit userStatusChanged();
}

void UserStatusItem::handleGotUserStatus(const UserStatusPtr &userStatus)
{
    if (!userStatus || m_UserStatusId != userStatus->GetId()) {
        return;
    }
    SetUserStatus(userStatus);
}

void UserStatusItem::handleLikeAdded(const QString& resourceId, quint64 ratingId)
{
    if (m_UserStatus && resourceId != m_UserStatusId) {
        return;
    }

    m_UserStatus->SetRatingId(ratingId);
    m_UserStatus->SetLikesCount(m_UserStatus->GetLikesCount() + 1);
    emit userStatusChanged();
}

void UserStatusItem::handleLikeRemoved(const QString& resourceId)
{
    if (m_UserStatus && resourceId != m_UserStatusId) {
        return;
    }

    m_UserStatus->SetRatingId(0);
    m_UserStatus->SetLikesCount(m_UserStatus->GetLikesCount() - 1);
    emit userStatusChanged();
}

void UserStatusItem::loadUserStatus()
{
    SailreadsManager::Instance(this)->loadUserStatus(this, m_UserStatusId);
}

} // namespace Sailusers
