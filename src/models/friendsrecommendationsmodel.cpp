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

#include "friendsrecommendationsmodel.h"

#include "../sailreadsmanager.h"
#include "../objects/friendrecommendation.h"
#include "../objects/user.h"

namespace Sailreads
{
FriendsRecommendationsModel::FriendsRecommendationsModel(QObject *parent)
: BaseModel<FriendRecommendation>(parent)
, m_HasMore(true)
, m_CurrentPage(1)
{
    connect(SailreadsManager::Instance(), &SailreadsManager::gotFriendsRecommendations,
            this, &FriendsRecommendationsModel::handleGotFriendsRecommendations);
    connect(SailreadsManager::Instance(), &SailreadsManager::friendRecommendationConfirmed,
            this, &FriendsRecommendationsModel::handleFriendRecommendationConfirmed);
}

bool FriendsRecommendationsModel::GetHasMore() const
{
    return m_HasMore;
}

void FriendsRecommendationsModel::SetHasMore(bool has)
{
    if (m_HasMore != has) {
        m_HasMore = has;
        emit hasMoreChanged();
    }
}

QVariant FriendsRecommendationsModel::data(const QModelIndex &index, int role) const
{
    if (index.row() > m_Items.count() - 1 || index.row() < 0) {
        return QVariant();
    }

    const auto& friendRecommendation = m_Items.at(index.row());
    switch (role) {
    case Id:
        return friendRecommendation.GetId();
    case CreateDate:
        return friendRecommendation.GetCreateDate();
    case RecommenderUser:
        return QVariant::fromValue(friendRecommendation.GetRecommenderUser().get());
    case RecommendedUser:
        return QVariant::fromValue(friendRecommendation.GetRecommendedUser().get());
    default:
        return QVariant();
    }
}

QHash<int, QByteArray> FriendsRecommendationsModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[Id] = "friendRecommendationId";
    roles[CreateDate] = "friendRecommendationCreateDate";
    roles[RecommenderUser] = "friendRecommendationRecommenderUser";
    roles[RecommendedUser] = "friendRecommendationRecommendedUser";
    return roles;
}

void FriendsRecommendationsModel::fetchMoreContent()
{
    SailreadsManager::Instance()->loadFriendsRecommendations(this, m_CurrentPage);
}

void FriendsRecommendationsModel::loadFriendsRecommendations()
{
    SailreadsManager::Instance()->loadFriendsRecommendations(this);
}

void FriendsRecommendationsModel::handleGotFriendsRecommendations(const CountedItems<FriendRecommendation>& friendsRecommendations)
{
    if (!friendsRecommendations.m_BeginIndex && !friendsRecommendations.m_EndIndex)
    {
        Clear();
    }
    else if (friendsRecommendations.m_BeginIndex == 1) {
        m_CurrentPage = 1;
        SetItems(friendsRecommendations.m_Items);
    }
    else {
        AddItems(friendsRecommendations.m_Items);
    }

    SetHasMore(friendsRecommendations.m_EndIndex != friendsRecommendations.m_Count);
    if (m_HasMore) {
        ++m_CurrentPage;
    }
}

void FriendsRecommendationsModel::handleFriendRecommendationConfirmed(quint64 friendRecommendationId, bool confirm)
{
    auto it = std::find_if(m_Items.begin(), m_Items.end(),
            [friendRecommendationId](decltype(m_Items.front()) fr) {
                return fr.GetId() == friendRecommendationId;
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
