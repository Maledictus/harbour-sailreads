#include "followersmodel.h"

#include "../sailreadsmanager.h"

namespace Sailreads
{
FollowersModel::FollowersModel(QObject *parent)
: FriendsModel(parent)
{
}

void FollowersModel::classBegin()
{
    auto sm = SailreadsManager::Instance();
    connect(sm, &SailreadsManager::gotUserFollowers,
            this, &FollowersModel::handleGotUserFriends);
}

void FollowersModel::fetchMoreContent()
{
    SailreadsManager::Instance()->loadUserFollowers(this, m_UserId, m_CurrentPage);
}

void FollowersModel::loadUsers()
{
    if (m_UserId <= 0) {
        return;
    }
    SailreadsManager::Instance()->loadUserFollowers(this, m_UserId);
}
} // namespace Sailreads
