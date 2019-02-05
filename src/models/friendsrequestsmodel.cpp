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

#include "friendsrequestsmodel.h"

#include "../sailreadsmanager.h"
#include "../objects/friendrequest.h"
#include "../objects/user.h"

namespace Sailreads
{
FriendsRequestsModel::FriendsRequestsModel(QObject *parent)
: BaseModel<FriendRequest>(parent)
, m_HasMore(true)
, m_CurrentPage(1)
{
    connect(SailreadsManager::Instance(), &SailreadsManager::gotFriendsRequests,
            this, &FriendsRequestsModel::handleGotFriendsRequests);
}

bool FriendsRequestsModel::GetHasMore() const
{
    return m_HasMore;
}

void FriendsRequestsModel::SetHasMore(bool has)
{
    if (m_HasMore != has) {
        m_HasMore = has;
        emit hasMoreChanged();
    }
}

QVariant FriendsRequestsModel::data(const QModelIndex &index, int role) const
{
    if (index.row() > m_Items.count() - 1 || index.row() < 0) {
        return QVariant();
    }

    const auto& friendRequest = m_Items.at(index.row());
    switch (role) {
    case Id:
        return friendRequest.GetId();
    case CreateDate:
        return friendRequest.GetCreatedAt();
    case FromUser:
        return QVariant::fromValue(friendRequest.GetFromUser().get());
    case Message:
        return friendRequest.GetMessage();
    default:
        return QVariant();
    }
}

QHash<int, QByteArray> FriendsRequestsModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[Id] = "friendRequestId";
    roles[CreateDate] = "friendRequestCreateDate";
    roles[FromUser] = "friendRequestFromUser";
    roles[Message] = "friendRequestMessage";
    return roles;
}

void FriendsRequestsModel::fetchMoreContent()
{
    SailreadsManager::Instance()->loadFriendsRequests(this, m_CurrentPage);
}

void FriendsRequestsModel::loadFriendsRequests()
{
    SailreadsManager::Instance()->loadFriendsRequests(this);
}

void FriendsRequestsModel::handleGotFriendsRequests(const CountedItems<FriendRequest>& friendsRequests)
{
    if (!friendsRequests.m_BeginIndex && !friendsRequests.m_EndIndex)
    {
        Clear();
    }
    else if (friendsRequests.m_BeginIndex == 1) {
        m_CurrentPage = 1;
        SetItems(friendsRequests.m_Items);
    }
    else {
        AddItems(friendsRequests.m_Items);
    }

    SetHasMore(friendsRequests.m_EndIndex != friendsRequests.m_Count);
    if (m_HasMore) {
        ++m_CurrentPage;
    }
}

} // namespace Sailreads
