#include "followingsmodel.h"

#include "../sailreadsmanager.h"

namespace Sailreads
{
FollowingsModel::FollowingsModel(QObject *parent)
: FriendsModel(parent)
{
}

void FollowingsModel::classBegin()
{
    auto sm = SailreadsManager::Instance();
    connect(sm, &SailreadsManager::gotUserFollowings,
            this, &FollowingsModel::handleGotUserFriends);
}

void FollowingsModel::fetchMoreContent()
{
    SailreadsManager::Instance()->loadUserFollowings(this, m_UserId, m_CurrentPage);
}

void FollowingsModel::loadUsers()
{
    if (m_UserId.isEmpty()) {
        return;
    }
    m_CurrentPage = 1;
    SailreadsManager::Instance()->loadUserFollowings(this, m_UserId, m_CurrentPage, false);
}
} // namespace Sailreads
