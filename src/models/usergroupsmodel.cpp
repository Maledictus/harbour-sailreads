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

#include "usergroupsmodel.h"

#include "../sailreadsmanager.h"

namespace Sailreads
{

UserGroupsModel::UserGroupsModel(QObject *parent)
: GroupsModel(parent)
, m_UserId("")
, m_HasMore(true)
, m_CurrentPage(1)
{
    auto sm = SailreadsManager::Instance();
    connect(sm, &SailreadsManager::gotUserGroups,
            this, &UserGroupsModel::handleGotUserGroups);
}

QString UserGroupsModel::GetUserId() const
{
    return m_UserId;
}

void UserGroupsModel::SetUserId(const QString& id)
{
    if (m_UserId != id) {
        m_UserId = id;
        emit userIdChanged();
    }
}

bool UserGroupsModel::GetHasMore() const
{
    return m_HasMore;
}

void UserGroupsModel::SetHasMore(bool has)
{
    if (m_HasMore != has) {
        m_HasMore = has;
        emit hasMoreChanged();
    }
}

void UserGroupsModel::fetchMoreContent()
{
    SailreadsManager::Instance()->loadGroups(this, m_UserId, m_CurrentPage);
}

void UserGroupsModel::loadGroups()
{
    if (m_UserId.isEmpty()) {
        return;
    }
    SailreadsManager::Instance()->loadGroups(this, m_UserId);
}

void UserGroupsModel::handleGotUserGroups(const QString& userId, const CountedItems<GroupPtr>& groups)
{
    if (userId != m_UserId) {
        return;
    }

    if (!groups.m_BeginIndex && !groups.m_EndIndex)
    {
        Clear();
    }
    else if (groups.m_BeginIndex == 1) {
        m_CurrentPage = 1;
        SetItems(groups.m_Items);
    }
    else {
        AddItems(groups.m_Items);
    }
    SetHasMore(groups.m_EndIndex != groups.m_Count);
    if (m_HasMore) {
        ++m_CurrentPage;
    }
}

} // namespace Sailreads
