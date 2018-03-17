/*
The MIT License(MIT)

Copyright (c) 2018 Oleg Linkin <maledictusdemagog@gmail.com>

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files(the "Software"), to deal
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

#include "userprofile.h"

#include <QtDebug>

#include "models/bookshelvesmodel.h"

namespace Sailreads
{
UserProfile::UserProfile(QObject *parent)
: QObject(parent)
, m_UserID(0)
, m_Private(false)
, m_Age(0)
, m_FriendsCount(0)
, m_GroupsCount(0)
, m_BooksCount(0)
{
}

UserProfile::UserProfile(qint64 id, const QString& name, const QUrl& webUrl, QObject *parent)
: QObject(parent)
, m_UserID(id)
, m_UserName(name)
, m_WebUrl(webUrl)
, m_Private(false)
, m_Age(0)
, m_FriendsCount(0)
, m_GroupsCount(0)
, m_BooksCount(0)
{
}

bool UserProfile::IsValid() const
{
    return m_UserID > 0;
}

quint64 UserProfile::GetUserID() const
{
    return m_UserID;
}

void UserProfile::SetUserID(quint64 id)
{
    if (m_UserID != id) {
        m_UserID = id;
        emit userIdChanged();
    }
}

QString UserProfile::GetUserName() const
{
    return m_UserName;
}

void UserProfile::SetUserName(const QString& name)
{
    if (m_UserName != name) {
        m_UserName = name;
        emit userNameChanged();
    }
}

QUrl UserProfile::GetWebUrl() const
{
    return m_WebUrl;
}

void UserProfile::SetWebUrl(const QUrl& url)
{
    if (m_WebUrl != url) {
        m_WebUrl = url;
        emit webUrlChanged();
    }
}

bool UserProfile::GetPrivate() const
{
    return m_Private;
}

void UserProfile::SetPrivate(bool priv)
{
    if (m_Private != priv) {
        m_Private = priv;
        emit isPrivateChanged();
    }
}

QString UserProfile::GetNickName() const
{
    return m_NickName;
}

void UserProfile::SetNickName(const QString& nickName)
{
    if (m_NickName != nickName) {
        m_NickName = nickName;
        emit nickNameChanged();
    }
}

QUrl UserProfile::GetAvatarUrl() const
{
    return m_AvatarUrl;
}

void UserProfile::SetAvatarUrl(const QUrl& url)
{
    if (m_AvatarUrl != url) {
        m_AvatarUrl = url;
        emit avatarUrlChanged();
    }
}

QUrl UserProfile::GetSmallAvatarUrl() const
{
    return m_SmallAvatarUrl;
}

void UserProfile::SetSmallAvatarUrl(const QUrl& url)
{
    if (m_SmallAvatarUrl != url) {
        m_SmallAvatarUrl = url;
        emit smallAvatarUrlChanged();
    }
}

QString UserProfile::GetAbout() const
{
    return m_About;
}

void UserProfile::SetAbout(const QString& about)
{
    if (m_About != about) {
        m_About = about;
        emit aboutChanged();
    }
}

quint32 UserProfile::GetAge() const
{
    return m_Age;
}

void UserProfile::SetAge(quint32 age)
{
    if (m_Age != age) {
        m_Age = age;
        emit ageChanged();
    }
}

QString UserProfile::GetGender() const
{
    return m_Gender;
}

void UserProfile::SetGender(const QString& gender)
{
    if (m_Gender != gender) {
        m_Gender = gender;
        emit genderChanged();
    }
}

QString UserProfile::GetLocation() const
{
    return m_Location;
}

void UserProfile::SetLocation(const QString& location)
{
    if (m_Location != location) {
        m_Location = location;
        emit locationChanged();
    }
}

QUrl UserProfile::GetWebSite() const
{
    return m_WebSite;
}

void UserProfile::SetWebSite(const QUrl& url)
{
    if (m_WebSite != url) {
        m_WebSite = url;
        emit webSiteChanged();
    }
}

QString UserProfile::GetJoinedDate() const
{
    return m_JoinDate;
}

void UserProfile::SetJoinedDate(const QString& date)
{
    if (m_JoinDate != date) {
        m_JoinDate = date;
        emit joinedDateChanged();
    }
}

QString UserProfile::GetLastUpdateDate() const
{
    return m_LastUpdateDate;
}

void UserProfile::SetLastUpdateDate(const QString& date)
{
    if (m_LastUpdateDate != date) {
        m_LastUpdateDate = date;
        emit lastUpdateDateChanged();
    }
}

QString UserProfile::GetInterests() const
{
    return m_Interests;
}

void UserProfile::SetInterests(const QString& interests)
{
    if (m_Interests != interests) {
        m_Interests = interests;
        emit interestsChanged();
    }
}

QString UserProfile::GetFavoriteBooksDesc() const
{
    return m_FavoriteBooksDesc;
}

void UserProfile::SetFavoriteBooksDesc(const QString& booksDesc)
{
    if (m_FavoriteBooksDesc != booksDesc) {
        m_FavoriteBooksDesc = booksDesc;
        emit favoriteBooksDescChanged();
    }
}

FavoriteAuthors_t UserProfile::GetFavoritesAuthors() const
{
    return m_FavoriteAuthors;
}

void UserProfile::SetFavoriteAuthors(const FavoriteAuthors_t& favAuthors)
{
    m_FavoriteAuthors = favAuthors;
}

QUrl UserProfile::GetUpdateRSSUrl() const
{
    return m_UpdateRSSUrl;
}

void UserProfile::SetUpdateRSSUrl(const QUrl& url)
{
    if (m_UpdateRSSUrl != url) {
        m_UpdateRSSUrl = url;
        emit updateRSSUrlChanged();
    }
}

QUrl UserProfile::GetReviewRSSUrl() const
{
    return m_ReviewRSSUrl;
}

void UserProfile::SetReviewRSSUrl(const QUrl& url)
{
    if (m_ReviewRSSUrl != url) {
        m_ReviewRSSUrl = url;
        emit reviewRSSUrlChanged();
    }
}

quint32 UserProfile::GetFriendsCount() const
{
    return m_FriendsCount;
}

void UserProfile::SetFriendsCount(quint32 count)
{
    if (m_FriendsCount != count) {
        m_FriendsCount = count;
        emit friendsCountChanged();
    }
}

quint32 UserProfile::GetGroupsCount() const
{
    return m_GroupsCount;
}

void UserProfile::SetGroupsCount(quint32 count)
{
    if (m_GroupsCount != count) {
        m_GroupsCount = count;
        emit groupsCountChanged();
    }
}

quint32 UserProfile::GetBooksCount() const
{
    return m_BooksCount;
}

void UserProfile::SetBooksCount(quint32 count)
{
    if (m_BooksCount != count) {
        m_BooksCount = count;
        emit booksCountChanged();
    }
}

BookShelves_t UserProfile::GetBookShelves() const
{
    return m_BookShelves;
}

void UserProfile::SetBookShelves(const BookShelves_t& shelves)
{
    m_BookShelves = shelves;
    emit bookShelvesCountChanged();
}

quint32 UserProfile::GetBookShelvesCount() const
{
    return m_BookShelves.count();
}

void UserProfile::Update(std::shared_ptr<UserProfile> profile)
{
    if (!profile) {
        return;
    }

    SetUserID(profile->GetUserID());
    SetUserName(profile->GetUserName());
    SetWebUrl(profile->GetWebUrl());
    SetPrivate(profile->GetPrivate());
    SetNickName(profile->GetNickName());
    SetAvatarUrl(profile->GetAvatarUrl());
    SetSmallAvatarUrl(profile->GetSmallAvatarUrl());
    SetAbout(profile->GetAbout());
    SetAge(profile->GetAge());
    SetGender(profile->GetGender());
    SetLocation(profile->GetLocation());
    SetWebSite(profile->GetWebSite());
    SetJoinedDate(profile->GetJoinedDate());
    SetLastUpdateDate(profile->GetLastUpdateDate());
    SetInterests(profile->GetInterests());
    SetFavoriteBooksDesc(profile->GetFavoriteBooksDesc());
    SetFavoriteAuthors(profile->GetFavoritesAuthors());
    SetUpdateRSSUrl(profile->GetUpdateRSSUrl());
    SetReviewRSSUrl(profile->GetReviewRSSUrl());
    SetFriendsCount(profile->GetFriendsCount());
    SetGroupsCount(profile->GetGroupsCount());
    SetBooksCount(profile->GetBooksCount());
    SetBookShelves(profile->GetBookShelves());
}
} // namespace Sailreads
