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

void SailreadsManager::AboutToQuit()
{
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
            [=](const QString& id, const QString&, const QString&) {
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
                if (!profile->GetBookShelves().isEmpty()) {
                    CountedItems<BookShelf> items;
                    items.m_Items = profile->GetBookShelves();
                    items.m_BeginIndex = 1;
                    items.m_EndIndex = items.m_Items.size();
                    items.m_Count = items.m_Items.size();

                    emit gotUserBookShelves(profile->GetId(), items);
                }
                if (!profile->GetRecentUpdates().isEmpty()) {
                    emit gotUserUpdates(profile->GetId(), profile->GetRecentUpdates());
                }
            });
    connect(m_Api, &GoodReadsApi::gotMessages,
            this, &SailreadsManager::gotMessages);
    connect(m_Api, &GoodReadsApi::gotMessage,
            this, &SailreadsManager::gotMessage);
    connect(m_Api, &GoodReadsApi::gotNotifications,
            this, &SailreadsManager::gotNotifications);
    connect(m_Api, &GoodReadsApi::gotFriendsRequests,
            this, &SailreadsManager::gotFriendsRequests);
    connect(m_Api, &GoodReadsApi::gotFriendsRecommendations,
            this, &SailreadsManager::gotFriendsRecommendations);

    connect(m_Api, &GoodReadsApi::gotUpdates,
            this, &SailreadsManager::gotUpdates);

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
    connect(m_Api, &GoodReadsApi::gotFoundReviews,
            this, &SailreadsManager::gotFoundReviews);
    connect(m_Api, &GoodReadsApi::gotReviewInfo,
            this, &SailreadsManager::gotReviewInfo);

    connect(m_Api, &GoodReadsApi::gotUserFriends,
            this, &SailreadsManager::gotUserFriends);
    connect(m_Api, &GoodReadsApi::gotUserFollowers,
            this, &SailreadsManager::gotUserFollowers);
    connect(m_Api, &GoodReadsApi::gotUserFollowings,
            this, &SailreadsManager::gotUserFollowings);
    connect(m_Api, &GoodReadsApi::friendRequestConfirmed,
            this, &SailreadsManager::friendRequestConfirmed);
    connect(m_Api, &GoodReadsApi::friendRecommendationConfirmed,
            this, &SailreadsManager::friendRecommendationConfirmed);
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
    connect(m_Api, &GoodReadsApi::gotFoundBooks,
            this, &SailreadsManager::gotFoundBooks);
    connect(m_Api, &GoodReadsApi::bookEditionSwitched,
            this, &SailreadsManager::bookEditionSwitched);

    connect(m_Api, &GoodReadsApi::gotSeries,
            this, &SailreadsManager::gotSeries);

    connect(m_Api, &GoodReadsApi::gotNewGroupFolderTopic,
            this, &SailreadsManager::gotNewGroupFolderTopic);

    connect(m_Api, &GoodReadsApi::gotComments,
            this, &SailreadsManager::gotComments);
    connect(m_Api, &GoodReadsApi::newCommentAdded,
            this, &SailreadsManager::newCommentAdded);

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

    connect(m_Api, &GoodReadsApi::gotReadStatus,
            this, &SailreadsManager::gotReadStatus);

    connect(m_Api, &GoodReadsApi::gotUserStatus,
            this, &SailreadsManager::gotUserStatus);

    connect(m_Api, &GoodReadsApi::gotRecommendation,
            this, &SailreadsManager::gotRecommendation);

    connect(m_Api, &GoodReadsApi::likeAdded,
            this, &SailreadsManager::likeAdded);
    connect(m_Api, &GoodReadsApi::likeRemoved,
            this, &SailreadsManager::likeRemoved);

    connect(m_Api, &GoodReadsApi::bookAddedToShelves,
            this, &SailreadsManager::bookAddedToShelves);
    connect(m_Api, &GoodReadsApi::bookAddedToShelves,
            this, [this](const QString& bookId, const QStringList& /*shelves*/,
                    const QStringList& /*oldShelves*/, const ReviewPtr& review)
            {
                emit reviewUpdated(bookId, review.get());
            });
    connect(m_Api, &GoodReadsApi::bookAddedToShelf,
            this, &SailreadsManager::bookAddedToShelf);
    connect(m_Api, &GoodReadsApi::bookRemovedFromShelf,
            this, &SailreadsManager::bookRemovedFromShelf);

    connect(m_Api, &GoodReadsApi::gotUserQuotes,
            this, &SailreadsManager::gotUserQuotes);
    connect(m_Api, &GoodReadsApi::gotBookQuotes,
            this, &SailreadsManager::gotBookQuotes);
    connect(m_Api, &GoodReadsApi::gotAuthorQuotes,
            this, &SailreadsManager::gotAuthorQuotes);
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

void SailreadsManager::abortRequest(QObject *requester)
{
    m_Api->AbortRequest(requester);
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

    m_Api->ClearCache();

    SetLogged(false);
}

void SailreadsManager::getUserInfo(QObject *requester, const QString& id, bool useCache)
{
    SetBusy(true);
    emit authProgressChanged(tr("Getting user profile..."));
    m_Api->GetUserInfo(requester, id, useCache);
}

void SailreadsManager::loadUpdates(QObject *requester, const QString& scope, const QString& items,
        const QDateTime& dt)
{
    SetBusy(true);
    m_Api->GetUpdates(requester, scope, items, dt);
}

void SailreadsManager::loadUserFollowings(QObject *requester, const QString& userId, int page,
        bool useCache)
{
    SetBusy(true);
    m_Api->GetUserFollowings(requester, userId, page, useCache);
}

void SailreadsManager::loadUserFollowers(QObject *requester, const QString& userId, int page,
        bool useCache)
{
    SetBusy(true);
    m_Api->GetUserFollowers(requester, userId, page, useCache);
}

void SailreadsManager::loadMessages(QObject *requester, const QString& folder, int page, bool useCache)
{
    SetBusy(true);
    m_Api->GetMessages(requester, folder, page, useCache);
}

void SailreadsManager::loadMessage(QObject *requester, quint64 messageId, bool useCache)
{
    SetBusy(true);
    m_Api->GetMessage(requester, messageId, useCache);
}

void SailreadsManager::markMessageAsRead(quint64 messageId)
{
    SetBusy(true);
    m_Api->MarkMessageAsRead(messageId);
}

void SailreadsManager::loadNotifications(QObject *requester, const QString& pageToken)
{
    SetBusy(true);
    m_Api->GetNotifications(requester, pageToken);
}

void SailreadsManager::loadFriendsRequests(QObject *requester, int page)
{
    SetBusy(true);
    m_Api->GetFriendRequests(requester, page);
}

void SailreadsManager::loadFriendsRecommendations(QObject *requester, int page)
{
    SetBusy(true);
    m_Api->GetFriendRequests(requester, page);
}

void SailreadsManager::loadBookShelves(QObject *requester, const QString& id, int page, bool useCache)
{
    SetBusy(true);
    m_Api->GetBookShelves(requester, id, page, useCache);
}

void SailreadsManager::loadAllBookShelves(QObject *requester, const QString &id, int page, bool useCache)
{
    SetBusy(true);
    m_Api->GetAllBookShelves(requester, id, page, useCache);
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

void SailreadsManager::loadReviews(QObject *requester, const QString& userId,
        const QString& bookShelf, int page, Qt::SortOrder order, const QString& sortField,
        bool useCache)
{
    SetBusy(true);
    m_Api->GetReviews(requester, userId, bookShelf, sortField, order, page, useCache);
}

void SailreadsManager::loadReview(QObject *requester, const QString& reviewId, int commentsPage,
        bool useCache)
{
    SetBusy(true);
    m_Api->GetReview(requester, reviewId, commentsPage, useCache);
}

void SailreadsManager::searchReviews(QObject *requester, const QString& userId,
        const QString& searchText, int page)
{
    SetBusy(true);
    m_Api->SearchReviews(requester, userId, searchText, page);
}

void SailreadsManager::addReview(const QString& bookId, int rating, const QString& reviewText)
{
    SetBusy(true);
    m_Api->AddReview(bookId, rating, reviewText);
}

void SailreadsManager::editReview(const QString& reviewId, int rating, const QString& reviewText,
        bool finished)
{
    SetBusy(true);
    m_Api->EditReview(reviewId, rating, reviewText, finished);
}

void SailreadsManager::removeReview(const QString& bookId, const QString& reviewId)
{
    SetBusy(true);
    m_Api->DeleteReview(bookId, reviewId);
}

void SailreadsManager::loadBook(QObject *requester, const QString& bookId, bool useCache)
{
    SetBusy(true);
    m_Api->GetBook(requester, bookId, useCache);
}

void SailreadsManager::loadBookEditions(QObject *requester, quint64 workId, int page, bool useCache)
{
    SetBusy(true);
    m_Api->GetBookEditions(requester, workId, page, useCache);
}

void SailreadsManager::searchBooks(QObject *requester, const QString& searchText,
        const QString& searchField, int page)
{
    SetBusy(true);
    m_Api->SearchBooks(requester, searchText, searchField, page);
}

void SailreadsManager::switchToBookEdition(const QString& reviewId, const QString& bookId)
{
    SetBusy(true);
    m_Api->SwitchToBookEdition(reviewId, bookId);
}

void SailreadsManager::loadSeries(QObject *requester, quint64 seriesId, bool useCache)
{
    SetBusy(true);
    m_Api->GetSeries(requester, seriesId, useCache);
}

void SailreadsManager::loadFriends(QObject *requester, const QString& userId, int page, bool useCache)
{
    SetBusy(true);
    m_Api->GetFriends(requester, userId, page, useCache);
}

void SailreadsManager::confirmFriendRequest(quint64 friendRequestId, bool confirm)
{
    SetBusy(true);
    m_Api->ConfirmFriendRequest(friendRequestId, confirm);
}

void SailreadsManager::confirmFriendRecommendationRequest(quint64 friendRecommendationId, bool confirm)
{
    SetBusy(true);
    m_Api->ConfirmFriendRecommendation(friendRecommendationId, confirm);
}

void SailreadsManager::followUser(const QString& userId)
{
    SetBusy(true);
    m_Api->FollowUser(userId);
}

void SailreadsManager::unfollowUser(const QString& userId)
{
    SetBusy(true);
    m_Api->UnfollowUser(userId);
}

void SailreadsManager::addAsFriend(const QString& userId)
{
    SetBusy(true);
    m_Api->AddFriend(userId);
}

void SailreadsManager::removeFriend(const QString& userId)
{
    SetBusy(true);
    m_Api->RemoveFriend(userId);
}

void SailreadsManager::loadGroups(QObject *requester, const QString& userId, int page, bool useCache)
{
    SetBusy(true);
    m_Api->GetGroups(requester, userId, page, useCache);
}

void SailreadsManager::loadGroup(QObject *requester, quint64 groupId, bool useCache)
{
    SetBusy(true);
    m_Api->GetGroup(requester, groupId, useCache);
}

void SailreadsManager::joinGroup(quint64 groupId)
{
    SetBusy(true);
    m_Api->JoinGroup(groupId);
}

void SailreadsManager::searchGroup(QObject *requester, const QString& text, int page)
{
    SetBusy(true);
    m_Api->SearchGroup(requester, text, page);
}

void SailreadsManager::loadGroupMembers(QObject *requester, quint64 groupId, int page, bool useCache)
{
    SetBusy(true);
    m_Api->GetGroupMembers(requester, groupId, page, useCache);
}

void SailreadsManager::loadGroupFolderTopics(QObject *requester, const QString& groupFolderId,
        quint64 groupId, int page, bool useCache)
{
    SetBusy(true);
    m_Api->GetGroupFolderTopics(requester, groupFolderId, groupId, page, useCache);
}

void SailreadsManager::loadGroupFolderTopic(QObject *requester, const QString& topicId, int page)
{
    SetBusy(true);
    m_Api->GetGroupFolderTopic(requester, topicId, page);
}

void SailreadsManager::addNewTopic(const QString& topic, const QString& subject, quint64 subjectId,
        const QString& folderId, bool question, bool updateFeed, bool digest, const QString& comment)
{
    SetBusy(true);
    m_Api->AddNewTopic(topic, subject, subjectId, folderId, question, updateFeed, digest, comment);
}

void SailreadsManager::loadComments(QObject *requester, const QString& resourceId,
        const QString& resourceType, int page)
{
    SetBusy(true);
    m_Api->GetComments(requester, resourceId, resourceType, page);
}

void SailreadsManager::addNewComment(const QString& type, const QString& resourceId,
        const QString& comment)
{
    SetBusy(true);
    m_Api->AddNewComment(type, resourceId, comment);
}

void SailreadsManager::addBookToShelf(const QString& bookId, const QString& shelf,
        const QStringList& oldShelves)
{
    SetBusy(true);
    m_Api->AddBookToShelf(bookId, shelf, oldShelves);
}

void SailreadsManager::addBookToShelves(const QString& bookId, const QStringList& shelves,
        const QStringList& oldShelves)
{
    SetBusy(true);
    m_Api->AddBooksToShelves({ bookId }, shelves, oldShelves);
}

void SailreadsManager::removeBookFromShelf(const QString& bookId, const QString& shelf)
{
    SetBusy(true);
    m_Api->RemoveBookFromShelf(bookId, shelf);
}

void SailreadsManager::loadAuthorProfile(QObject *requester, const QString& authorId, bool useCache)
{
    SetBusy(true);
    m_Api->GetAuthor(requester, authorId, useCache);
}

void SailreadsManager::loadAuthorBooks(QObject *requester, const QString& authorId, int page,
        bool useCache)
{
    SetBusy(true);
    m_Api->GetAuthorBooks(requester, authorId, page, useCache);
}

void SailreadsManager::loadAuthorSeries(QObject *requester, const QString& authorId, bool useCache)
{
    SetBusy(true);
    m_Api->GetAuthorSeries(requester, authorId, useCache);
}

void SailreadsManager::followAuthor(const QString& authorId)
{
    SetBusy(true);
    m_Api->FollowAuthor(authorId);
}

void SailreadsManager::unfollowAuthor(const QString& authorId, quint64 followingId)
{
    SetBusy(true);
    m_Api->UnfollowAuthor(authorId, followingId);
}

void SailreadsManager::loadReadStatus(QObject *requester, const QString& id, int page, bool useCache)
{
    SetBusy(true);
    m_Api->GetReadStatus(requester, id, page, useCache);
}

void SailreadsManager::loadUserStatus(QObject *requester, const QString &id, int page, bool useCache)
{
    SetBusy(true);
    m_Api->GetUserStatus(requester, id, page, useCache);
}

void SailreadsManager::updateReadingProgress(const QString& bookId, const QString& key, int value,
        const QString& comment)
{
    SetBusy(true);
    m_Api->UpdateReadingProgress(bookId, key, value, comment);
}

void SailreadsManager::loadRecommendation(QObject *requester, const QString& id, int page, bool useCache)
{
    SetBusy(true);
    m_Api->GetRecommendation(requester, id, page, useCache);
}

void SailreadsManager::likeResource(const QString& resourceId, const QString& resourceType)
{
    SetBusy(true);
    m_Api->LikeResource(resourceId, resourceType);
}

void SailreadsManager::unlikeResource(const QString& resourceId, quint64 ratingId)
{
    SetBusy(true);
    m_Api->UnlikeResource(resourceId, ratingId);
}

void SailreadsManager::loadUserQuotes(QObject *requester, const QString& userId, int page,
    bool useCache)
{
    SetBusy(true);
    m_Api->LoadUserQuotes(requester, userId, page, useCache);
}

void SailreadsManager::loadBookQuotes(QObject *requester, quint64 workId, int page, bool useCache)
{
    SetBusy(true);
    m_Api->LoadBookQuotes(requester, workId, page, useCache);
}

void SailreadsManager::loadAuthorQuotes(QObject *requester, const QString& authorId, int page,
        bool useCache)
{
    SetBusy(true);
    m_Api->LoadAuthorQuotes(requester, authorId, page, useCache);
}

}
