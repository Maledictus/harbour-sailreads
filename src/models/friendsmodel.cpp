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

#include "friendsmodel.h"

#include "../sailreadsmanager.h"

namespace Sailreads
{
FriendsModel::FriendsModel(QObject *parent)
: BaseModel<Friend>(parent)
, m_UserId(0)
, m_HasMore(true)
, m_CurrentPage(1)
{
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
    case Url:
        return friendItem.GetUrl();
    case AvatarImage:
        return friendItem.GetAvatarUrl();
    case SmallAvatarImage:
        return friendItem.GetSmallAvatarUrl();
    case FriendsCount:
        return friendItem.GetFriendsCount();
    case BooksCount:
        return friendItem.GetBooksCount();
    case CreatedAt:
        return friendItem.GetCreatedDate();
    default:
        return QVariant();
    }
}

QHash<int, QByteArray> FriendsModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[Id] = "friendId";
    roles[Name] = "friendName";
    roles[Url] = "friendUrl";
    roles[AvatarImage] = "friendAvatarUrl";
    roles[SmallAvatarImage] = "friendSmallAvatarUrl";
    roles[FriendsCount] = "friendFriendsCount";
    roles[BooksCount] = "friendBooksCount";
    roles[CreatedAt] = "friendCreatedAt";
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

bool FriendsModel::GetHasMore() const
{
    return m_HasMore;
}

void FriendsModel::SetHasMore(bool has)
{
    if (m_HasMore != has) {
        m_HasMore = has;
        emit hasMoreChanged();
    }
}

void FriendsModel::classBegin()
{
    auto sm = SailreadsManager::Instance();
    connect(sm, &SailreadsManager::gotUserFriends,
            this, &FriendsModel::handleGotUserFriends);
    connect(sm, &SailreadsManager::friendRemoved,
            this, &FriendsModel::handleFriendRemoved);
}

void FriendsModel::componentComplete()
{
}

void FriendsModel::fetchMoreContent()
{
    SailreadsManager::Instance()->loadFriends(this, m_UserId, m_CurrentPage);
}

void FriendsModel::handleGotUserFriends(quint64 userId, const CountedItems<Friend>& friends)
{
    if (userId != m_UserId) {
        return;
    }

    if (friends.m_BeginIndex == 1) {
        m_CurrentPage = 1;
        SetItems(friends.m_Items);
    }
    else {
        AddItems(friends.m_Items);
    }

    SetHasMore(friends.m_EndIndex != friends.m_Count);
    if (m_HasMore) {
        ++m_CurrentPage;
    }
}

void FriendsModel::handleFriendRemoved(quint64 friendId)
{
    auto it = std::find_if(m_Items.begin(), m_Items.end(),
            [friendId](decltype(m_Items.front()) fr) {
                return fr.GetId() == friendId;
            });
    if (it == m_Items.end()) {
        return;
    }

    const int pos = std::distance(m_Items.begin(), it);
    beginRemoveRows(QModelIndex(), pos, pos);
    m_Items.removeAt(pos);
    endRemoveRows();
}

} // namespace Sailreads
