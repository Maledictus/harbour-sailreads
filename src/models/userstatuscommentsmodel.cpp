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
#include "userstatuscommentsmodel.h"

#include "../objects/userstatus.h"
#include "../sailreadsmanager.h"

namespace Sailreads
{
UserStatusCommentsModel::UserStatusCommentsModel(QObject *parent)
: BaseCommentsModel(parent)
{
}

void UserStatusCommentsModel::classBegin()
{
    auto sm = SailreadsManager::Instance();
    connect(sm, &SailreadsManager::gotUserStatus,
            this, &UserStatusCommentsModel::handleGotUserStatus);
    connect(sm, &SailreadsManager::newCommentAdded,
            this, &UserStatusCommentsModel::handleNewCommentAdded);
}

void UserStatusCommentsModel::componentComplete()
{
}

QString UserStatusCommentsModel::GetUserStatusId() const
{
    return m_UserStatusId;
}

void UserStatusCommentsModel::SetUserStatusId(const QString& id)
{
    if (m_UserStatusId != id) {
        m_UserStatusId = id;
        userStatusIdChanged();
    }
}

void UserStatusCommentsModel::fetchMoreContent()
{
    if (!rowCount()) {
        return;
    }

    SailreadsManager::Instance()->loadUserStatus(this, m_UserStatusId, m_CurrentPage);
    SetFetching(true);
}

void UserStatusCommentsModel::handleGotUserStatus(const UserStatusPtr& userStatus)
{
    if (!userStatus || userStatus->GetId() != m_UserStatusId) {
        return;
    }
    handleGotComments(userStatus->GetComments());
    SetFetching(false);
}

void UserStatusCommentsModel::handleNewCommentAdded(const QString& resourceId, const Comment& comment)
{
    if (resourceId != m_UserStatusId) {
        return;
    }
    if (!m_HasMore) {
        AddItems(comment);
    }
}

} // namespace Sailreads
