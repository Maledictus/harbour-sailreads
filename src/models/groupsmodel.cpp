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

#include "groupsmodel.h"

#include "../sailreadsmanager.h"

namespace Sailreads
{
GroupsModel::GroupsModel(QObject *parent)
: BaseModel<Group>(parent)
, m_UserId(0)
{
    auto sm = SailreadsManager::Instance();
    connect(sm, &SailreadsManager::gotUserGroups,
            this, &GroupsModel::handleGotUserGroups);
}

QVariant GroupsModel::data(const QModelIndex& index, int role) const
{
    if (index.row() > m_Items.count() - 1 || index.row() < 0) {
        return QVariant();
    }

    const auto& group = m_Items.at(index.row());
    switch (role) {
    case Id:
        return group.GetId();
    case Name:
        return group.GetName();
    case Public:
        return group.GetIsPublic();
    case UsersCount:
        return group.GetUsersCount();
    case ImageUrl:
        return group.GetImageUrl();
    case LastActivity:
        return group.GetLastActivity();
    case Url:
        return group.GetUrl();
    default:
        return QVariant();
    }
}

QHash<int, QByteArray> GroupsModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[Id] = "groupId";
    roles[Name] = "groupName";
    roles[Public] = "groupPublic";
    roles[UsersCount] = "groupUsersCount";
    roles[ImageUrl] = "groupImageUrl";
    roles[LastActivity] = "groupLastActivity";
    roles[Url] = "groupUrl";
    return roles;
}

bool GroupsModel::canFetchMore(const QModelIndex& parent) const
{
    return !parent.isValid() ? m_CanFetchMore : false;
}

void GroupsModel::fetchMore(const QModelIndex& parent)
{
    if (parent.isValid()) {
        return;
    }

    m_CanFetchMore = false;
    SailreadsManager::Instance()->loadGroups(m_UserId);
}

quint64 GroupsModel::GetUserId() const
{
    return m_UserId;
}

void GroupsModel::SetUserId(quint64 id)
{
    if (m_UserId != id) {
        m_UserId = id;
        emit userIdChanged();
    }
}

void GroupsModel::handleGotUserGroups(quint64 userId, const Groups_t& groups)
{
    if (userId != m_UserId) {
        return;
    }

    SetItems(groups);
}

} // namespace Sailreads
