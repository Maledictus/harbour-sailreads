/*
The MIT License (MIT)

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

#include "sailreadsmanager.h"

#include <QtDebug>
#include <QDir>

#include "application.h"
#include "objects/user.h"
#include "settings/accountsettings.h"
#include "goodreadsapi.h"

namespace Sailreads
{
SailreadsManager::SailreadsManager(QObject *parent)
: QObject(parent)
, m_Api(new GoodReadsApi(this))
, m_IsBusy(false)
, m_IsLogged(false)
{
    MakeConnections();

    m_AccessToken = AccountSettings::Instance(this)->value("access_token").toString();
    m_AccessSecretToken = AccountSettings::Instance(this)->value("access_token_secret").toString();
    SetLogged(!m_AccessToken.isEmpty() && !m_AccessSecretToken.isEmpty());
}

SailreadsManager *SailreadsManager::Instance(QObject *parent)
{
    static SailreadsManager *sailreadsManager = nullptr;
    if (!sailreadsManager)
    {
        sailreadsManager = new SailreadsManager(parent);
    }
    return sailreadsManager;
}

bool SailreadsManager::GetBusy() const
{
    return m_IsBusy;
}

bool SailreadsManager::GetLogged() const
{
    return m_IsLogged;
}

User* SailreadsManager::GetAuthUser() const
{
    return m_AuthUser.get();
}

void SailreadsManager::MakeConnections()
{
    connect(m_Api, &GoodReadsApi::requestFinished,
            this, [=](){ SetBusy(false); });

    connect(m_Api, &GoodReadsApi::authenticationFailed,
            this, [=](){ SetLogged(false); });

    connect(m_Api, &GoodReadsApi::requestTokenChanged,
            this, &SailreadsManager::requestTokenChanged);
    connect(m_Api, &GoodReadsApi::accessTokensChanged,
            this,
            [=](const QString& accessToken, const QString& accessTokenSecret) {
                AccountSettings::Instance(this)->setValue("access_token", accessToken);
                AccountSettings::Instance(this)->setValue("access_token_secret", accessTokenSecret);
                m_AccessToken = accessToken;
                m_AccessSecretToken = accessTokenSecret;
                if (!m_AccessToken.isEmpty() && !m_AccessSecretToken.isEmpty()) {
                    m_Api->UpdateCredentials(m_AccessToken, m_AccessSecretToken);
                }
                SetLogged(!accessToken.isEmpty() && !accessTokenSecret.isEmpty());
            });

    connect(m_Api, &GoodReadsApi::gotAuthUserInfo,
            this,
            [=](const quint64& id, const QString&, const QString&) {
                if (!m_AuthUser) {
                    m_AuthUser = std::make_shared<User>();
                }
                m_AuthUser->SetId(id);
                emit authUserChanged();
                emit gotAuthUserId(m_AuthUser->GetId());
            });
    connect(m_Api, &GoodReadsApi::gotUserProfile,
            this,
            [=](const UserPtr& profile) {
                if (!profile) {
                    return;
                }
                if (m_AuthUser && profile->GetId() == m_AuthUser->GetId()) {
                    qDebug() << "Authenticated profile updated";
                    m_AuthUser = profile;
                    emit authUserChanged();
                }
                emit gotUserProfile(profile);
                CountedItems<BookShelf> items;
                items.m_Items = profile->GetBookShelves();
                items.m_BeginIndex = 1;
                items.m_EndIndex = items.m_Items.size();
                items.m_Count = items.m_Items.size();
                emit gotUserBookShelves(profile->GetId(), items);
            });
    connect(m_Api, &GoodReadsApi::gotMessages,
            this, &SailreadsManager::gotMessages);
    connect(m_Api, &GoodReadsApi::gotMessage,
            this, &SailreadsManager::gotMessage);
    connect(m_Api, &GoodReadsApi::gotNotifications,
            this, &SailreadsManager::gotNotifications);

    connect(m_Api, &GoodReadsApi::gotUserBookShelves,
            this, &SailreadsManager::gotUserBookShelves);
    connect(m_Api, &GoodReadsApi::bookShelfAdded,
            this, &SailreadsManager::bookShelfAdded);
    connect(m_Api, &GoodReadsApi::bookShelfEdited,
            this, &SailreadsManager::bookShelfEdited);

    connect(m_Api, &GoodReadsApi::gotReviews,
            this, &SailreadsManager::gotReviews);
    connect(m_Api, &GoodReadsApi::gotReview,
            this, &SailreadsManager::gotReview);

    connect(m_Api, &GoodReadsApi::gotUserFriends,
            this, &SailreadsManager::gotUserFriends);
    connect(m_Api, &GoodReadsApi::gotUserFollowers,
            this, &SailreadsManager::gotUserFollowers);
    connect(m_Api, &GoodReadsApi::gotUserFollowings,
            this, &SailreadsManager::gotUserFollowings);
    connect(m_Api, &GoodReadsApi::friendRequestConfirmed,
            this, &SailreadsManager::friendRequestConfirmed);
    connect(m_Api, &GoodReadsApi::userFollowed,
            this, &SailreadsManager::userFollowed);
    connect(m_Api, &GoodReadsApi::userUnfollowed,
            this, &SailreadsManager::userUnfollowed);
    connect(m_Api, &GoodReadsApi::friendAdded,
            this, &SailreadsManager::friendAdded);
    connect(m_Api, &GoodReadsApi::friendRemoved,
            this, &SailreadsManager::friendRemoved);

    connect(m_Api, &GoodReadsApi::gotUserGroups,
            this, &SailreadsManager::gotUserGroups);
    connect(m_Api, &GoodReadsApi::gotUserGroup,
            this, &SailreadsManager::gotUserGroup);
    connect(m_Api, &GoodReadsApi::gotFoundGroups,
            this, &SailreadsManager::gotFoundGroups);
    connect(m_Api, &GoodReadsApi::gotGroupMembers,
            this, &SailreadsManager::gotGroupMembers);
    connect(m_Api, &GoodReadsApi::gotGroupFolderTopics,
            this, &SailreadsManager::gotGroupFolderTopics);
    connect(m_Api, &GoodReadsApi::gotGroupFolderTopic,
            this, &SailreadsManager::gotGroupFolderTopic);
    connect(m_Api, &GoodReadsApi::gotBook,
            this, &SailreadsManager::gotBook);
    connect(m_Api, &GoodReadsApi::gotBookEditions,
            this, &SailreadsManager::gotBookEditions);
    connect(m_Api, &GoodReadsApi::gotSeries,
            this, &SailreadsManager::gotSeries);

    connect(m_Api, &GoodReadsApi::gotAuthorProfile,
            this, &SailreadsManager::gotAuthorProfile);
    connect(m_Api, &GoodReadsApi::authorFollowed,
            this, &SailreadsManager::authorFollowed);
    connect(m_Api, &GoodReadsApi::authorUnfollowed,
            this, &SailreadsManager::authorUnfollowed);
    connect(m_Api, &GoodReadsApi::gotAuthorBooks,
            this, &SailreadsManager::gotAuthorBooks);
    connect(m_Api, &GoodReadsApi::gotAuthorSeries,
            this, &SailreadsManager::gotAuthorSeries);
}

void SailreadsManager::SetBusy(bool busy)
{
    m_IsBusy = busy;
    emit busyChanged();
}

void SailreadsManager::SetLogged(bool logged)
{
    m_IsLogged = logged;
    if (m_IsLogged) {
        m_Api->UpdateCredentials(m_AccessToken, m_AccessSecretToken);
    }
    else {
        m_Api->UpdateCredentials("", "");
        m_AccessToken.clear();
        m_AccessSecretToken.clear();
        AccountSettings::Instance(this)->removeAll();
    }
    emit loggedChanged();
}

void SailreadsManager::handleGotAuthAnswer(const QString& data)
{
    if (data.contains("authorize=1")) {
        m_Api->RequestAccessToken();
    }
    else if (data.contains("authorize=0")){
        SetLogged(false);
    }
}

void SailreadsManager::obtainRequestToken()
{
    SetBusy(true);
    emit authProgressChanged(tr("Authorization user..."));
    m_Api->ObtainRequestToken();
}

void SailreadsManager::requestAccessToken()
{
    SetBusy(true);
    m_Api->RequestAccessToken();
}

void SailreadsManager::abortRequest()
{
    m_Api->AbortRequest();
}

void SailreadsManager::authenticateUser()
{
    SetBusy(true);
    emit authProgressChanged(tr("Authentication user..."));
    m_Api->AuthUser();
}

void SailreadsManager::logout()
{
    //TODO reset application settings
    //TODO clear all caches
    QDir webkitCache(Application::GetPath(Application::ApplicationDirectory::CacheDirectory) +
            "/.QtWebKit");
    webkitCache.removeRecursively();
    webkitCache = QDir(Application::GetPath(Application::ApplicationDirectory::AppDataDirectory) +
            "/.QtWebKit");
    webkitCache.removeRecursively();

    SetLogged(false);
}

void SailreadsManager::getUserInfo(quint64 id)
{
    SetBusy(true);
    emit authProgressChanged(tr("Getting user profile..."));
    m_Api->GetUserInfo(id);
}

void SailreadsManager::loadUpdates()
{
    SetBusy(true);
    m_Api->GetUpdates();
}

void SailreadsManager::loadUserFollowings(quint64 userId, int page)
{
    SetBusy(true);
    m_Api->GetUserFollowings(userId, page);
}

void SailreadsManager::loadUserFollowers(quint64 userId, int page)
{
    SetBusy(true);
    m_Api->GetUserFollowers(userId, page);
}

void SailreadsManager::loadMessages(const QString& folder, int page)
{
    SetBusy(true);
    m_Api->GetMessages(folder, page);
}

void SailreadsManager::loadMessage(quint64 messageId)
{
    SetBusy(true);
    m_Api->GetMessage(messageId);
}

void SailreadsManager::markMessageAsRead(quint64 messageId)
{
    SetBusy(true);
    m_Api->MarkMessageAsRead(messageId);
}

void SailreadsManager::loadNotifications(const QString& pageToken)
{
    SetBusy(true);
    m_Api->GetNotifications(pageToken);
}

void SailreadsManager::loadBookShelves(quint64 id, int page)
{
    SetBusy(true);
    m_Api->GetBookShelves(id, page);
}

void SailreadsManager::addBookShelf(const QString& name, bool exclusive, bool sortable,
        bool featured, bool recommendFor)
{
    SetBusy(true);
    m_Api->AddBookShelf(name, exclusive, sortable, featured, recommendFor);
}

void SailreadsManager::editBookShelf(quint64 id, const QString& name, bool exclusive,
        bool sortable, bool featured, bool recommendFor)
{
    SetBusy(true);
    m_Api->EditBookShelf(id, name, exclusive, sortable, featured, recommendFor);
}

void SailreadsManager::loadReviews(quint64 userId, const QString& bookShelf, int page,
        Qt::SortOrder order, const QString& sortField)
{
    SetBusy(true);
    m_Api->GetReviews(userId, bookShelf, sortField, order, page);
}

void SailreadsManager::loadReview(quint64 reviewId, int commentsPage)
{
    SetBusy(true);
    m_Api->GetReview(reviewId, commentsPage);
}

void SailreadsManager::loadBook(quint64 bookId)
{
    SetBusy(true);
    m_Api->GetBook(bookId);
}

void SailreadsManager::loadBookEditions(quint64 workId, int page)
{
    SetBusy(true);
    m_Api->GetBookEditions(workId, page);
}

void SailreadsManager::loadSeries(quint64 seriesId)
{
    SetBusy(true);
    m_Api->GetSeries(seriesId);
}

void SailreadsManager::loadFriends(quint64 userId, int page)
{
    SetBusy(true);
    m_Api->GetFriends(userId, page);
}

void SailreadsManager::confirmFriendRequest(quint64 friendRequestId, bool confirm)
{
    SetBusy(true);
    m_Api->ConfirmFriendRequest(friendRequestId, confirm);
}

void SailreadsManager::followUser(quint64 id)
{
    SetBusy(true);
    m_Api->FollowUser(id);
}

void SailreadsManager::unfollowUser(quint64 id)
{
    SetBusy(true);
    m_Api->UnfollowUser(id);
}

void SailreadsManager::addAsFriend(quint64 id)
{
    SetBusy(true);
    m_Api->AddFriend(id);
}

void SailreadsManager::removeFriend(quint64 id)
{
    SetBusy(true);
    m_Api->RemoveFriend(id);
}

void SailreadsManager::loadGroups(quint64 userId, int page)
{
    SetBusy(true);
    m_Api->GetGroups(userId, page);
}

void SailreadsManager::loadGroup(quint64 groupId)
{
    SetBusy(true);
    m_Api->GetGroup(groupId);
}

void SailreadsManager::joinGroup(quint64 groupId)
{
    SetBusy(true);
    m_Api->JoinGroup(groupId);
}

void SailreadsManager::searchGroup(const QString& text, int page)
{
    SetBusy(true);
    m_Api->SearchGroup(text, page);
}

void SailreadsManager::loadGroupMembers(quint64 groupId, int page)
{
    SetBusy(true);
    m_Api->GetGroupMembers(groupId, page);
}

void SailreadsManager::loadGroupFolderTopics(const QString& groupFolderId, quint64 groupId, int page)
{
    SetBusy(true);
    m_Api->GetGroupFolderTopics(groupFolderId, groupId, page);
}

void SailreadsManager::loadGroupFolderTopic(quint64 topicId, int page)
{
    SetBusy(true);
    m_Api->GetGroupFolderTopic(topicId, page);
}

void SailreadsManager::addNewTopic(const QString& topic, const QString& subject, quint64 subjectId,
        const QString& folderId, bool question, bool updateFeed, bool digest, const QString& comment)
{
    SetBusy(true);
    m_Api->AddNewTopic(topic, subject, subjectId, folderId, question, updateFeed, digest, comment);
}

void SailreadsManager::addNewComment(const QString& type, quint64 resourceId, const QString& comment)
{
    SetBusy(true);
    m_Api->AddNewComment(type, resourceId, comment);
}

void SailreadsManager::addBookToShelves(quint64 bookId, const QStringList& shelves)
{
    SetBusy(true);
    m_Api->AddBooksToShelves({ bookId }, shelves);
}

void SailreadsManager::loadAuthorProfile(quint64 authorId)
{
    SetBusy(true);
    m_Api->GetAuthor(authorId);
}

void SailreadsManager::loadAuthorBooks(quint64 authorId, int page)
{
    SetBusy(true);
    m_Api->GetAuthorBooks(authorId, page);
}

void SailreadsManager::loadAuthorSeries(quint64 authorId)
{
    SetBusy(true);
    m_Api->GetAuthorSeries(authorId);
}

void SailreadsManager::followAuthor(quint64 authorId)
{
    SetBusy(true);
    m_Api->FollowAuthor(authorId);
}

void SailreadsManager::unfollowAuthor(quint64 authorId, quint64 followingId)
{
    SetBusy(true);
    m_Api->UnfollowAuthor(authorId, followingId);
}
}
