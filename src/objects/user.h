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

#pragma once

#include <QObject>
#include <QString>
#include <QVariantMap>
#include <QUrl>

#include "bookshelf.h"
#include "friendrequest.h"
#include "../types.h"

namespace Sailreads
{
class User : public QObject
{
    Q_OBJECT

    QString m_Id;
    QUrl m_WebUrl;
    bool m_Private;

    QString m_UserName;
    QString m_FirstName;
    QString m_LastName;
    QString m_NickName;
    QUrl m_AvatarUrl;
    QUrl m_SmallAvatarUrl;
    QString m_About;
    quint32 m_Age;
    QString m_Gender;
    QString m_Location;
    QUrl m_WebSite;
    QString m_JoinDate;
    QString m_LastUpdateDate;
    QString m_Interests;
    QString m_FavoriteBooksDesc;
    FavoriteAuthors_t m_FavoriteAuthors;
    QUrl m_UpdateRSSUrl;
    QUrl m_ReviewRSSUrl;
    quint32 m_FriendsCount;
    quint32 m_GroupsCount;
    quint32 m_BooksCount;
    BookShelves_t m_BookShelves;
    bool m_IsFriend;
    bool m_IsFollwer;
    bool m_IsFollowing;
    FriendRequest m_FriendRequest;
    Updates_t m_RecentUpdates;

    Q_PROPERTY(QString id READ GetId NOTIFY idChanged)
    Q_PROPERTY(QString userName READ GetUserName NOTIFY userNameChanged)
    Q_PROPERTY(QString firstName READ GetFirstName NOTIFY firstNameChanged)
    Q_PROPERTY(QString lastName READ GetLastName NOTIFY lastNameChanged)
    Q_PROPERTY(QString nickName READ GetNickName NOTIFY nickNameChanged)
    Q_PROPERTY(QUrl webUrl READ GetWebUrl NOTIFY webUrlChanged)
    Q_PROPERTY(bool isPrivate READ GetPrivate NOTIFY isPrivateChanged)
    Q_PROPERTY(QUrl avatarUrl READ GetAvatarUrl NOTIFY avatarUrlChanged)
    Q_PROPERTY(QUrl smallAvatarUrl READ GetSmallAvatarUrl NOTIFY smallAvatarUrlChanged)
    Q_PROPERTY(QString about READ GetAbout NOTIFY aboutChanged)
    Q_PROPERTY(quint32 age READ GetAge NOTIFY ageChanged)
    Q_PROPERTY(QString gender READ GetGender NOTIFY genderChanged)
    Q_PROPERTY(QString location READ GetLocation NOTIFY locationChanged)
    Q_PROPERTY(QUrl webSite READ GetWebSite NOTIFY webSiteChanged)
    Q_PROPERTY(QString joinedDate READ GetJoinedDate NOTIFY joinedDateChanged)
    Q_PROPERTY(QString lastUpdateDate READ GetLastUpdateDate NOTIFY lastUpdateDateChanged)
    Q_PROPERTY(QString interests READ GetInterests NOTIFY interestsChanged)
    Q_PROPERTY(QString favoriteBooksDesc READ GetFavoriteBooksDesc NOTIFY favoriteBooksDescChanged)
    Q_PROPERTY(QUrl updateRSSUrl READ GetUpdateRSSUrl NOTIFY updateRSSUrlChanged)
    Q_PROPERTY(QUrl reviewRSSUrl READ GetReviewRSSUrl NOTIFY reviewRSSUrlChanged)
    Q_PROPERTY(quint32 friendsCount READ GetFriendsCount NOTIFY friendsCountChanged)
    Q_PROPERTY(quint32 groupsCount READ GetGroupsCount NOTIFY groupsCountChanged)
    Q_PROPERTY(quint32 booksCount READ GetBooksCount NOTIFY booksCountChanged)
    Q_PROPERTY(quint32 bookShelvesCount READ GetBookShelvesCount NOTIFY bookShelvesCountChanged)
    Q_PROPERTY(bool isFriend READ GetIsFriend NOTIFY isFriendChanged)
    Q_PROPERTY(bool isFollowing READ GetIsFollowing NOTIFY isFollowingChanged)
    Q_PROPERTY(bool isFollower READ GetIsFollower NOTIFY isFollowerChanged)
    Q_PROPERTY(quint64 friendRequestId READ GetFriendRequestId NOTIFY friendRequestIdChanged)
    Q_PROPERTY(int friendStatus READ GetFriendStatus NOTIFY friendStatusChanged)
    Q_PROPERTY(QString favoriteAutors READ GetFavoritesAuthorsString NOTIFY favoriteAutorsChanged)
    Q_PROPERTY(quint64 currentlyReadingShelfId READ GetCurrentlyReadingShelfId NOTIFY currentlyReadingShelfIdChanged)

public:
    enum FriendStatus
    {
        NotAFriend = 0,
        FriendRequestReceived,
        FriendRequestSent,
        Friend
    };
    Q_ENUMS(FriendStatus)

private:
    FriendStatus m_FriendStatus;

public:
    User(QObject *parent = nullptr);
    virtual ~User();

    QString GetId() const;
    void SetId(const QString& id);
    QString GetUserName() const;
    void SetUserName(const QString& name);
    QString GetFirstName() const;
    void SetFirstName(const QString& name);
    QString GetLastName() const;
    void SetLastName(const QString& name);
    QString GetNickName() const;
    void SetNickName(const QString& name);
    QUrl GetWebUrl() const;
    void SetWebUrl(const QUrl& url);
    bool GetPrivate() const;
    void SetPrivate(bool priv);
    QUrl GetAvatarUrl() const;
    void SetAvatarUrl(const QUrl& url);
    QUrl GetSmallAvatarUrl() const;
    void SetSmallAvatarUrl(const QUrl& url);
    QString GetAbout() const;
    void SetAbout(const QString& about);
    quint32 GetAge() const;
    void SetAge(quint32 age);
    QString GetGender() const;
    void SetGender(const QString& gender);
    QString GetLocation() const;
    void SetLocation(const QString& location);
    QUrl GetWebSite() const;
    void SetWebSite(const QUrl& url);
    QString GetJoinedDate() const;
    void SetJoinedDate(const QString& date);
    QString GetLastUpdateDate() const;
    void SetLastUpdateDate(const QString& date);
    QString GetInterests() const;
    void SetInterests(const QString& interests);
    QString GetFavoriteBooksDesc() const;
    void SetFavoriteBooksDesc(const QString& booksDesc);
    FavoriteAuthors_t GetFavoritesAuthors() const;
    void SetFavoriteAuthors(const FavoriteAuthors_t& favAuthors);
    QUrl GetUpdateRSSUrl() const;
    void SetUpdateRSSUrl(const QUrl& url);
    QUrl GetReviewRSSUrl() const;
    void SetReviewRSSUrl(const QUrl& url);
    quint32 GetFriendsCount() const;
    void SetFriendsCount(quint32 count);
    quint32 GetGroupsCount() const;
    void SetGroupsCount(quint32 count);
    quint32 GetBooksCount() const;
    void SetBooksCount(quint32 count);
    BookShelves_t GetBookShelves() const;
    void SetBookShelves(const BookShelves_t& shelves);
    quint32 GetBookShelvesCount() const;
    quint64 GetCurrentlyReadingShelfId() const;
    void SetIsFriend(bool isFriend);
    bool GetIsFriend() const;
    void SetIsFollowing(bool following);
    bool GetIsFollowing() const;
    void SetIsFollower(bool follower);
    bool GetIsFollower() const;
    void SetFriendRequest(const FriendRequest& fr);
    FriendRequest GetFriendRequest() const;
    quint64 GetFriendRequestId() const;
    int GetFriendStatus() const;
    void SetFriendStatus(const QString& status);
    QString GetFavoritesAuthorsString() const;
    Updates_t GetRecentUpdates() const;
    void SetRecentUpdates(const Updates_t& updates);

    bool IsEqual(const UserPtr& user) const;

signals:
    void idChanged();
    void userNameChanged();
    void firstNameChanged();
    void lastNameChanged();
    void nickNameChanged();
    void webUrlChanged();
    void isPrivateChanged();
    void avatarUrlChanged();
    void smallAvatarUrlChanged();
    void aboutChanged();
    void ageChanged();
    void genderChanged();
    void locationChanged();
    void webSiteChanged();
    void joinedDateChanged();
    void lastUpdateDateChanged();
    void interestsChanged();
    void favoriteBooksDescChanged();
    void updateRSSUrlChanged();
    void reviewRSSUrlChanged();
    void friendsCountChanged();
    void groupsCountChanged();
    void booksCountChanged();
    void bookShelvesCountChanged();
    void isFriendChanged();
    void isFollowingChanged();
    void isFollowerChanged();
    void friendRequestIdChanged();
    void friendStatusChanged();
    void favoriteAutorsChanged();
    void currentlyReadingShelfIdChanged();
};
} // namespace Sailreads
