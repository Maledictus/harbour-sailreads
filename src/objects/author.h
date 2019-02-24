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

#include <memory>

#include <QDateTime>
#include <QObject>
#include <QString>
#include <QUrl>

#include "../types.h"

namespace Sailreads
{
class Author: public QObject
{
    Q_OBJECT

    QString m_Id;
    QString m_Name;
    QUrl m_ImageUrl;
    QUrl m_SmallImageUrl;
    QUrl m_Link;
    qreal m_AverageRating;
    quint64 m_RatingsCount;
    quint64 m_TextReviewsCount;

    quint64 m_FansCount;
    quint64 m_FollowersCount;
    QUrl m_LargeImageUrl;
    QString m_About;
    QString m_Influences;
    quint64 m_WorksCount;
    QString m_Gender;
    QString m_HomeTown;
    QDateTime m_BirthDate;
    QDateTime m_DeathDate;
    bool m_IsGoodreadsAuthor;
    QString m_UserId;
    Books_t m_Books;
    quint64 m_FollowingId;


    Q_PROPERTY(QString id READ GetId NOTIFY idChanged)
    Q_PROPERTY(QString name READ GetName NOTIFY nameChanged)
    Q_PROPERTY(QUrl imageUrl READ GetImageUrl NOTIFY imageUrlChanged)
    Q_PROPERTY(QUrl smallImageUrl READ GetSmallImageUrl NOTIFY smallImageUrlChanged)
    Q_PROPERTY(QUrl link READ GetLink NOTIFY linkChanged)
    Q_PROPERTY(qreal averageRating READ GetAverageRating NOTIFY averageRatingChanged)
    Q_PROPERTY(quint64 ratingsCount READ GetRatingsCount NOTIFY ratingsCountChanged)
    Q_PROPERTY(quint64 textReviewsCount READ GetTextReviewsCount NOTIFY textReviewsCountChanged)
    Q_PROPERTY(quint64 fansCount READ GetFansCount NOTIFY fansCountChanged)
    Q_PROPERTY(quint64 followersCount READ GetFollowersCount NOTIFY followersCountChanged)
    Q_PROPERTY(QUrl largeImageUrl READ GetLargeImageUrl NOTIFY largeImageUrlChanged)
    Q_PROPERTY(QString about READ GetAbout NOTIFY aboutChanged)
    Q_PROPERTY(QString influences READ GetInfluences NOTIFY influencesChanged)
    Q_PROPERTY(quint64 worksCount READ GetWorksCount NOTIFY worksCountChanged)
    Q_PROPERTY(QString gender READ GetGender NOTIFY genderChanged)
    Q_PROPERTY(QString homeTown READ GetHomeTown NOTIFY homeTownChanged)
    Q_PROPERTY(QDateTime birthDate READ GetBirthDate NOTIFY birthDateChanged)
    Q_PROPERTY(QDateTime deathDate READ GetDeathDate NOTIFY deathDateChanged)
    Q_PROPERTY(bool isGoodreadsAuthor READ GetIsGoodreadsAuthor NOTIFY isGoodreadsAuthorChanged)
    Q_PROPERTY(QString userId READ GetUserId NOTIFY userIdChanged)
    Q_PROPERTY(QObjectList books READ GetBooks NOTIFY booksChanged)
    Q_PROPERTY(quint64 followingId READ GetFollowingId NOTIFY followingIdChanged)

public:
    Author(QObject *parent = nullptr);
    ~Author();

    QString GetId() const;
    void SetId(const QString& id);
    QString GetName() const;
    void SetName(const QString& name);
    QUrl GetImageUrl() const;
    void SetImageUrl(const QUrl& imageUrl);
    QUrl GetSmallImageUrl() const;
    void SetSmallImageUrl(const QUrl& smallImageUrl);
    QUrl GetLink() const;
    void SetLink(const QUrl& link);
    qreal GetAverageRating() const;
    void SetAverageRating(qreal averageRating);
    quint64 GetRatingsCount() const;
    void SetRatingsCount(quint64 ratingsCount);
    quint64 GetTextReviewsCount() const;
    void SetTextReviewsCount(quint64 textReviewsCount);
    quint64 GetFansCount() const;
    void SetFansCount(quint64 fansCount);
    quint64 GetFollowersCount() const;
    void SetFollowersCount(quint64 followersCount);
    QUrl GetLargeImageUrl() const;
    void SetLargeImageUrl(const QUrl& largeImageUrl);
    QString GetAbout() const;
    void SetAbout(const QString& about);
    QString GetInfluences() const;
    void SetInfluences(const QString& influences);
    quint64 GetWorksCount() const;
    void SetWorksCount(quint64 worksCount);
    QString GetGender() const;
    void SetGender(const QString& gender);
    QString GetHomeTown() const;
    void SetHomeTown(const QString& homeTown);
    QDateTime GetBirthDate() const;
    void SetBirthDate(const QDateTime& dt);
    QDateTime GetDeathDate() const;
    void SetDeathDate(const QDateTime& dt);
    bool GetIsGoodreadsAuthor() const;
    void SetIsGoodreadsAuthor(bool isGoodreadsAuthor);
    QString GetUserId() const;
    void SetUserId(const QString& userId);
    QObjectList GetBooks() const;
    Books_t GetBooksPtr() const;
    void SetBooks(const Books_t& books);
    quint64 GetFollowingId() const;
    void SetFollowingId(quint64 id);

    void Update(Author *newAuthor);
    void Update(const AuthorPtr& newAuthor);

signals:
    void idChanged();
    void nameChanged();
    void imageUrlChanged();
    void smallImageUrlChanged();
    void linkChanged();
    void averageRatingChanged();
    void ratingsCountChanged();
    void textReviewsCountChanged();
    void fansCountChanged();
    void followersCountChanged();
    void largeImageUrlChanged();
    void aboutChanged();
    void worksCountChanged();
    void genderChanged();
    void homeTownChanged();
    void birthDateChanged();
    void deathDateChanged();
    void isGoodreadsAuthorChanged();
    void userIdChanged();
    void booksChanged();
    void influencesChanged(QString influences);
    void followingIdChanged(quint64 followingId);
};
} // namespace Sailreads
