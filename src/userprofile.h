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

#pragma once

#include <memory>

#include <QObject>
#include <QString>
#include <QVariantMap>
#include <QUrl>

#include "objects/bookshelf.h"
#include "types.h"
#include "userupdate.h"

namespace Sailreads
{
class BookShelvesModel;

class UserProfile : public QObject
{
    Q_OBJECT

    quint64 m_UserID;
    QString m_UserName;
    QUrl m_WebUrl;
    bool m_Private;

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

    Q_PROPERTY(quint64 userId READ GetUserID NOTIFY userIdChanged)
    Q_PROPERTY(QString userName READ GetUserName NOTIFY userNameChanged)
    Q_PROPERTY(QUrl webUrl READ GetWebUrl NOTIFY webUrlChanged)
    Q_PROPERTY(bool isPrivate READ GetPrivate NOTIFY isPrivateChanged)
    Q_PROPERTY(QString nickName READ GetNickName NOTIFY nickNameChanged)
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

public:
    explicit UserProfile(QObject *parent = 0);
    explicit UserProfile(qint64 id, const QString& name, const QUrl& webUrl, QObject *parent = 0);

    bool IsValid() const;

    quint64 GetUserID() const;
    void SetUserID(quint64 id);
    QString GetUserName() const;
    void SetUserName(const QString& name);
    QUrl GetWebUrl() const;
    void SetWebUrl(const QUrl& url);
    bool GetPrivate() const;
    void SetPrivate(bool priv);

    QString GetNickName() const;
    void SetNickName(const QString& nickName);
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

    void Update(std::shared_ptr<UserProfile> profile);

signals:
    void userIdChanged();
    void userNameChanged();
    void webUrlChanged();
    void isPrivateChanged();
    void nickNameChanged();
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
};
} // namespace Sailreads
