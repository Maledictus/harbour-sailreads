/*
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

#include "usergroupsmodel.h"

#include "../sailreadsmanager.h"

namespace Sailreads
{

UserGroupsModel::UserGroupsModel(QObject *parent)
: GroupsModel(parent)
, m_UserId(0)
{
    auto sm = SailreadsManager::Instance();
    connect(sm, &SailreadsManager::gotUserGroups,
            this, &UserGroupsModel::handleGotUserGroups);
}

bool UserGroupsModel::canFetchMore(const QModelIndex& parent) const
{
    return !parent.isValid() ? m_CanFetchMore : false;
}

void UserGroupsModel::fetchMore(const QModelIndex& parent)
{
    if (parent.isValid()) {
        return;
    }

    SailreadsManager::Instance()->loadGroups(m_UserId);
}

quint64 UserGroupsModel::GetUserId() const
{
    return m_UserId;
}

void UserGroupsModel::SetUserId(quint64 id)
{
    if (m_UserId != id) {
        m_UserId = id;
        emit userIdChanged();
    }
}

void UserGroupsModel::handleGotUserGroups(quint64 userId, const CountedItems<GroupPtr>& groups)
{
    if (userId != m_UserId) {
        return;
    }

    m_CanFetchMore = groups.m_EndIndex != groups.m_Count;
    if (groups.m_BeginIndex == 1) {
        SetItems(groups.m_Items);
    }
    else {
        AddItems(groups.m_Items);
    }
}

} // namespace Sailreads
