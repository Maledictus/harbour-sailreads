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

#include "friendsmodel.h"

#include "../sailreadsmanager.h"

namespace Sailreads
{
FriendsModel::FriendsModel(QObject *parent)
: BaseModel<Friend>(parent)
, m_UserId(0)
{
    auto sm = SailreadsManager::Instance();
    connect(sm, &SailreadsManager::gotUserFriends,
            this, &FriendsModel::handleGotUserFriends);
}

QVariant FriendsModel::data(const QModelIndex& index, int role) const
{
    if (index.row() > m_Items.count() - 1 || index.row() < 0) {
        return QVariant();
    }

    const auto& friendItem = m_Items.at(index.row());
    switch (role) {
    case Id:
        return friendItem.GetId();
    case Name:
        return friendItem.GetName();
    default:
        return QVariant();
    }
}

QHash<int, QByteArray> FriendsModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[Id] = "friendId";
    roles[Name] = "friendName";
    return roles;
}

quint64 FriendsModel::GetUserId() const
{
    return m_UserId;
}

void FriendsModel::SetUserId(quint64 id)
{
    if (m_UserId != id) {
        m_UserId = id;
        emit userIdChanged();
    }
}

void FriendsModel::handleGotUserFriends(quint64 userId, const Friends_t& friends)
{
    if (userId != m_UserId) {
        return;
    }

    SetItems(friends);
}

} // namespace Sailreads
