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

#include "groupfoldersmodel.h"

namespace Sailreads
{
GroupFoldersModel::GroupFoldersModel(QObject *parent)
: BaseModel<GroupFolder>(parent)
, m_GroupId(0)
{
}

QVariant GroupFoldersModel::data(const QModelIndex& index, int role) const
{
    if (index.row() > m_Items.count() - 1 || index.row() < 0) {
        return QVariant();
    }

    const auto& groupFolder = m_Items.at(index.row());
    switch (role) {
    case Id:
        return groupFolder.GetId();
    case Name:
        return groupFolder.GetName();
    case ItemsCount:
        return groupFolder.GetItemsCount();
    case SubItemsCount:
        return groupFolder.GetSubItemsCount();
    case UpdatedAt:
        return groupFolder.GetUpdatedAt();
    default:
        return QVariant();
    }
}

QHash<int, QByteArray> GroupFoldersModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[Id] = "groupFolderId";
    roles[Name] = "groupFolderName";
    roles[ItemsCount] = "groupFolderItemsCount";
    roles[SubItemsCount] = "groupFolderSubItemsCount";
    roles[UpdatedAt] = "groupFolderUpdatedAt";
    return roles;
}

quint64 GroupFoldersModel::GetGroupId() const
{
    return m_GroupId;
}

void GroupFoldersModel::SetGroupId(quint64 id)
{
    if (m_GroupId != id) {
        m_GroupId = id;
        emit groupIdChanged();
    }
}

void GroupFoldersModel::handleGotGroup(quint64 groupId, const Group& group)
{
    if (m_GroupId == groupId) {
        SetItems(group.GetGroupFolders());
    }
}
} // namespace Sailreads
