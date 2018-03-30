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

#include "groupmembersmodel.h"

#include "../sailreadsmanager.h"

namespace Sailreads
{
GroupMembersModel::GroupMembersModel(QObject *parent)
: BaseModel<GroupMember>(parent)
, m_GroupId(0)
, m_HasMore(true)
, m_CurrentPage(1)
{
    auto sm = SailreadsManager::Instance();
    connect(sm, &SailreadsManager::gotGroupMembers,
            this, &GroupMembersModel::handleGotGroupMembers);
}

QVariant GroupMembersModel::data(const QModelIndex& index, int role) const
{
    if (index.row() > m_Items.count() - 1 || index.row() < 0) {
        return QVariant();
    }

    const auto& groupMember = m_Items.at(index.row());
    switch (role) {
    case Id:
        return groupMember.GetUser().GetId();
    case Title:
        return groupMember.GetTitle();
    case FirstName:
        return groupMember.GetUser().GetFirstName();
    case LastName:
        return groupMember.GetUser().GetLastName();
    case NickName:
        return groupMember.GetUser().GetNickName();
    case AvatarImage:
        return groupMember.GetUser().GetAvatar();
    case CommentsCount:
        return groupMember.GetCommentsCount();
    default:
        return QVariant();
    }
}

QHash<int, QByteArray> GroupMembersModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[Id] = "groupMemberId";
    roles[Title] = "groupMemberTitle";
    roles[FirstName] = "groupMemberFirstName";
    roles[LastName] = "groupMemberLastName";
    roles[NickName] = "groupMemberNickName";
    roles[AvatarImage] = "groupMemberAvatar";
    roles[CommentsCount] = "groupMemberCommentsCount";
    return roles;
}

quint64 GroupMembersModel::GetGroupId() const
{
    return m_GroupId;
}

void GroupMembersModel::SetGroupId(quint64 id)
{
    if (m_GroupId != id) {
        m_GroupId = id;
        emit groupIdChanged();
    }
}

bool GroupMembersModel::GetHasMore() const
{
    return m_HasMore;
}

void GroupMembersModel::SetHasMore(bool has)
{
    if (m_HasMore != has) {
        m_HasMore = has;
        emit hasMoreChanged();
    }
}

void GroupMembersModel::handleGotGroupMembers(quint64 groupId, const GroupMembers_t& members)
{
    if (members.isEmpty()) {
        SetHasMore(false);
    }

    if (m_GroupId == groupId) {
        AddItems(members);
    }
}

void GroupMembersModel::clear()
{
    m_CurrentPage = 1;
    Clear();
}

void GroupMembersModel::fetchMoreContent()
{
    SailreadsManager::Instance()->loadGroupMembers(m_GroupId, ++m_CurrentPage);
}
} // namespace Sailreads

