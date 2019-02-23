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

#include "user.h"

#include <QtDebug>

namespace Sailreads
{
User::User(QObject *parent)
: QObject(parent)
, m_Id("")
, m_Private(false)
, m_Age(0)
, m_FriendsCount(0)
, m_GroupsCount(0)
, m_BooksCount(0)
, m_IsFriend(false)
, m_IsFollwer(false)
, m_IsFollowing(false)
, m_FriendStatus(User::NotAFriend)
{
#ifdef QT_DEBUG
    qDebug() << this << "CONSTRUCTED";
#endif
}

User::~User()
{
#ifdef QT_DEBUG
    qDebug() << this << "DESTRUCTED";
#endif
}

QString User::GetId() const
{
    return m_Id;
}

void User::SetId(const QString& id)
{
    m_Id = id;
}

QString User::GetUserName() const
{
    return m_UserName.isEmpty() ? m_FirstName : m_UserName;
}

void User::SetUserName(const QString& name)
{
    m_UserName = name;
}

QString User::GetFirstName() const
{
    return m_FirstName;
}

void User::SetFirstName(const QString& name)
{
    m_FirstName = name;
}

QString User::GetLastName() const
{
    return m_LastName;
}

void User::SetLastName(const QString& name)
{
    m_LastName = name;
}

QString User::GetNickName() const
{
    return m_NickName;
}

void User::SetNickName(const QString& name)
{
    m_NickName = name;
}

QUrl User::GetWebUrl() const
{
    return m_WebUrl;
}

void User::SetWebUrl(const QUrl& url)
{
    m_WebUrl = url;
}

bool User::GetPrivate() const
{
    return m_Private;
}

void User::SetPrivate(bool priv)
{
    m_Private = priv;
}

QUrl User::GetAvatarUrl() const
{
    return m_AvatarUrl.isEmpty() ? m_SmallAvatarUrl : m_AvatarUrl;
}

void User::SetAvatarUrl(const QUrl& url)
{
    m_AvatarUrl = url;
}

QUrl User::GetSmallAvatarUrl() const
{
    return m_SmallAvatarUrl;
}

void User::SetSmallAvatarUrl(const QUrl& url)
{
    m_SmallAvatarUrl = url;
}

QString User::GetAbout() const
{
    return m_About;
}

void User::SetAbout(const QString& about)
{
    m_About = about;
}

quint32 User::GetAge() const
{
    return m_Age;
}

void User::SetAge(quint32 age)
{
    m_Age = age;
}

QString User::GetGender() const
{
    return m_Gender;
}

void User::SetGender(const QString& gender)
{
    m_Gender = gender;
}

QString User::GetLocation() const
{
    return m_Location;
}

void User::SetLocation(const QString& location)
{
    m_Location = location;
}

QUrl User::GetWebSite() const
{
    return m_WebSite;
}

void User::SetWebSite(const QUrl& url)
{
    m_WebSite = url;
}

QString User::GetJoinedDate() const
{
    return m_JoinDate;
}

void User::SetJoinedDate(const QString& date)
{
    m_JoinDate = date;
}

QString User::GetLastUpdateDate() const
{
    return m_LastUpdateDate;
}

void User::SetLastUpdateDate(const QString& date)
{
    m_LastUpdateDate = date;
}

QString User::GetInterests() const
{
    return m_Interests;
}

void User::SetInterests(const QString& interests)
{
    m_Interests = interests;
}

QString User::GetFavoriteBooksDesc() const
{
    return m_FavoriteBooksDesc;
}

void User::SetFavoriteBooksDesc(const QString& booksDesc)
{
    m_FavoriteBooksDesc = booksDesc;
}

FavoriteAuthors_t User::GetFavoritesAuthors() const
{
    return m_FavoriteAuthors;
}

void User::SetFavoriteAuthors(const FavoriteAuthors_t& favAuthors)
{
    m_FavoriteAuthors = favAuthors;
}

QUrl User::GetUpdateRSSUrl() const
{
    return m_UpdateRSSUrl;
}

void User::SetUpdateRSSUrl(const QUrl& url)
{
    m_UpdateRSSUrl = url;
}

QUrl User::GetReviewRSSUrl() const
{
    return m_ReviewRSSUrl;
}

void User::SetReviewRSSUrl(const QUrl& url)
{
    m_ReviewRSSUrl = url;
}

quint32 User::GetFriendsCount() const
{
    return m_FriendsCount;
}

void User::SetFriendsCount(quint32 count)
{
    m_FriendsCount = count;
}

quint32 User::GetGroupsCount() const
{
    return m_GroupsCount;
}

void User::SetGroupsCount(quint32 count)
{
    m_GroupsCount = count;
}

quint32 User::GetBooksCount() const
{
    return m_BooksCount;
}

void User::SetBooksCount(quint32 count)
{
    m_BooksCount = count;
}

BookShelves_t User::GetBookShelves() const
{
    return m_BookShelves;
}

void User::SetBookShelves(const BookShelves_t& shelves)
{
    m_BookShelves = shelves;
    emit currentlyReadingShelfIdChanged();
}

quint32 User::GetBookShelvesCount() const
{
    return m_BookShelves.count();
}

quint64 User::GetCurrentlyReadingShelfId() const
{
    auto it = std::find_if(m_BookShelves.begin(), m_BookShelves.end(),
            [](decltype(m_BookShelves.front()) bookshelf)
            {
                return bookshelf.GetExclusive() &&
                        bookshelf.GetName() == "currently-reading";
            });
    return it->GetId();
}

void User::SetIsFriend(bool isFriend)
{
    m_IsFriend = isFriend;
}

bool User::GetIsFriend() const
{
    return m_IsFriend;
}

void User::SetIsFollowing(bool following)
{
    m_IsFollowing = following;
}

bool User::GetIsFollowing() const
{
    return m_IsFollowing;
}

void User::SetIsFollower(bool follower)
{
    m_IsFollwer = follower;
}

bool User::GetIsFollower() const
{
    return m_IsFollwer;
}

void User::SetFriendRequest(const FriendRequest& fr)
{
    m_FriendRequest = fr;
}

FriendRequest User::GetFriendRequest() const
{
    return m_FriendRequest;
}

quint64 User::GetFriendRequestId() const
{
    return m_FriendRequest.GetId();
}

int User::GetFriendStatus() const
{
    return m_FriendStatus;
}

void User::SetFriendStatus(const QString& status)
{
    if (status == "friend") {
        m_FriendStatus = User::Friend;
    }
    else if (status == "not_friend") {
        m_FriendStatus = User::NotAFriend;
    }
    else if (status == "request_pending_to") {
        m_FriendStatus = User::FriendRequestSent;
    }
    else if (status == "request_pending_from") {
        m_FriendStatus = User::FriendRequestReceived;
    }
}

QString User::GetFavoritesAuthorsString() const
{
    QString result;
    for (int i = 0; i < m_FavoriteAuthors.size(); ++i) {
        result += QString("<a href=\"%1\" style=\"text-decoration:none;\">%2</a>")
                .arg(m_FavoriteAuthors[i].m_Id)
                .arg(m_FavoriteAuthors[i].m_Name);
        if (i + 1 < m_FavoriteAuthors.size()) {
            result += ", ";
        }
    }
    return result;
}

Updates_t User::GetRecentUpdates() const
{
    return m_RecentUpdates;
}

void User::SetRecentUpdates(const Updates_t& updates)
{
    m_RecentUpdates = updates;
}

bool User::IsEqual(const UserPtr& user) const
{
    if (!user) {
        return false;
    }

    return m_Id == user->GetId() &&
        m_WebUrl == user->GetWebUrl() &&
        m_Private == user->GetPrivate() &&
        m_UserName == user->GetUserName() &&
        m_FirstName == user->GetFirstName() &&
        m_LastName == user->GetLastName() &&
        m_NickName == user->GetNickName() &&
        m_AvatarUrl == user->GetAvatarUrl() &&
        m_SmallAvatarUrl == user->GetSmallAvatarUrl() &&
        m_About == user->GetAbout() &&
        m_Age == user->GetAge() &&
        m_Gender == user->GetGender() &&
        m_Location == user->GetLocation() &&
        m_WebSite == user->GetWebSite() &&
        m_JoinDate == user->GetJoinedDate() &&
        m_LastUpdateDate == user->GetLastUpdateDate() &&
        m_Interests == user->GetInterests() &&
        m_FavoriteBooksDesc == user->GetFavoriteBooksDesc() &&
        m_FavoriteAuthors == user->GetFavoritesAuthors() &&
        m_UpdateRSSUrl == user->GetUpdateRSSUrl() &&
        m_ReviewRSSUrl == user->GetReviewRSSUrl() &&
        m_FriendsCount == user->GetFriendsCount() &&
        m_GroupsCount == user->GetGroupsCount() &&
        m_BooksCount == user->GetBooksCount() &&
        m_BookShelves == user->GetBookShelves() &&
        m_IsFriend == user->GetIsFriend() &&
        m_IsFollowing == user->GetIsFollowing() &&
        m_IsFollwer == user->GetIsFollower() &&
        m_FriendRequest.GetId() == user->GetFriendRequest().GetId() &&
        m_FriendStatus == user->GetFriendStatus();
}
} // namespace Sailreads
