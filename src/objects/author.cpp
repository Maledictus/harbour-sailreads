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

#include "author.h"

#include <QtDebug>

#include "book.h"

namespace Sailreads
{
Author::Author(QObject *parent)
: QObject(parent)
, m_Id("")
, m_AverageRating(0)
, m_RatingsCount(0)
, m_TextReviewsCount(0)
, m_FansCount(0)
, m_FollowersCount(0)
, m_WorksCount(0)
, m_IsGoodreadsAuthor(false)
, m_UserId("")
, m_FollowingId(0)
{
#ifdef QT_DEBUG
    qDebug() << this << "CONSTRUCTED";
#endif
}

Author::~Author()
{
#ifdef QT_DEBUG
    qDebug() << this << "DESTRUCTED";
#endif
}

QString Author::GetId() const
{
    return m_Id;
}

void Author::SetId(const QString& id)
{
    m_Id = id;
}

QString Author::GetName() const
{
    return m_Name;
}

void Author::SetName(const QString& name)
{
    m_Name = name;
}

QUrl Author::GetImageUrl() const
{
    return m_ImageUrl;
}

void Author::SetImageUrl(const QUrl& imageUrl)
{
    m_ImageUrl = imageUrl;
}

QUrl Author::GetSmallImageUrl() const
{
    return m_SmallImageUrl;
}

void Author::SetSmallImageUrl(const QUrl& smallImageUrl)
{
    m_SmallImageUrl = smallImageUrl;
}

QUrl Author::GetLink() const
{
    return m_Link;
}

void Author::SetLink(const QUrl& link)
{
    m_Link = link;
}

qreal Author::GetAverageRating() const
{
    return m_AverageRating;
}

void Author::SetAverageRating(qreal averageRating)
{
    m_AverageRating = averageRating;
}

quint64 Author::GetRatingsCount() const
{
    return m_RatingsCount;
}

void Author::SetRatingsCount(quint64 ratingsCount)
{
    m_RatingsCount = ratingsCount;
}

quint64 Author::GetTextReviewsCount() const
{
    return m_TextReviewsCount;
}

void Author::SetTextReviewsCount(quint64 textReviewsCount)
{
    m_TextReviewsCount = textReviewsCount;
}

quint64 Author::GetFansCount() const
{
    return m_FansCount;
}

void Author::SetFansCount(quint64 fansCount)
{
    m_FansCount = fansCount;
}

quint64 Author::GetFollowersCount() const
{
    return m_FollowersCount;
}

void Author::SetFollowersCount(quint64 followersCount)
{
    m_FollowersCount = followersCount;
}

QUrl Author::GetLargeImageUrl() const
{
    return m_LargeImageUrl;
}

void Author::SetLargeImageUrl(const QUrl& largeImageUrl)
{
    m_LargeImageUrl = largeImageUrl;
}

QString Author::GetAbout() const
{
    return m_About;
}

void Author::SetAbout(const QString& about)
{
    m_About = about;
}

QString Author::GetInfluences() const
{
    return m_Influences;
}

void Author::SetInfluences(const QString& influences)
{
    m_Influences = influences;
}

quint64 Author::GetWorksCount() const
{
    return m_WorksCount;
}

void Author::SetWorksCount(quint64 worksCount)
{
    m_WorksCount = worksCount;
}

QString Author::GetGender() const
{
    return m_Gender;
}

void Author::SetGender(const QString& gender)
{
    m_Gender = gender;
}

QString Author::GetHomeTown() const
{
    return m_HomeTown;
}

void Author::SetHomeTown(const QString& homeTown)
{
    m_HomeTown = homeTown;
}

QDateTime Author::GetBirthDate() const
{
    return m_BirthDate;
}

void Author::SetBirthDate(const QDateTime& dt)
{
    m_BirthDate = dt;
}

QDateTime Author::GetDeathDate() const
{
    return m_DeathDate;
}

void Author::SetDeathDate(const QDateTime& dt)
{
    m_DeathDate = dt;
}

bool Author::GetIsGoodreadsAuthor() const
{
    return m_IsGoodreadsAuthor;
}

void Author::SetIsGoodreadsAuthor(bool isGoodreadsAuthor)
{
    m_IsGoodreadsAuthor = isGoodreadsAuthor;
}

QString Author::GetUserId() const
{
    return m_UserId;
}

void Author::SetUserId(const QString& userId)
{
    m_UserId = userId;
}

QObjectList Author::GetBooks() const
{
    QObjectList objList;
    for (const auto& book : m_Books) {
        objList << book.get();
    }
    return objList;
}

Books_t Author::GetBooksPtr() const
{
    return m_Books;
}

void Author::SetBooks(const Books_t& books)
{
    m_Books = books;
    if (!m_Books.isEmpty()) {
        const auto& book = m_Books.front();
        const auto& authors = book->GetAuthorsPtr();
        auto it = std::find_if(authors.begin(), authors.end(),
                [this](decltype(authors.front()) author)
                { return m_Id == author->GetId(); });
        if (it != authors.end()) {
            SetAverageRating(std::max(m_AverageRating, (*it)->GetAverageRating()));
            SetRatingsCount(std::max(m_RatingsCount, (*it)->GetRatingsCount()));
            SetTextReviewsCount(std::max(m_TextReviewsCount, (*it)->GetTextReviewsCount()));
        }
    }
}

quint64 Author::GetFollowingId() const
{
    return m_FollowingId;
}

void Author::SetFollowingId(quint64 id)
{
    m_FollowingId = id;
}

void Author::Update(Author *newAuthor)
{
    if (!newAuthor) {
        return;
    }

    SetId(newAuthor->GetId());
    SetName(newAuthor->GetName());
    SetLink(newAuthor->GetLink());
    SetImageUrl(newAuthor->GetImageUrl());
    SetSmallImageUrl(newAuthor->GetSmallImageUrl());
    SetAverageRating(newAuthor->GetAverageRating());
    SetRatingsCount(newAuthor->GetRatingsCount());
    SetTextReviewsCount(newAuthor->GetTextReviewsCount());
}

void Author::Update(const AuthorPtr& newAuthor)
{
    if (!newAuthor) {
        return;
    }

    SetId(newAuthor->GetId());
    SetName(newAuthor->GetName());
    SetLink(newAuthor->GetLink());
    SetFansCount(newAuthor->GetFansCount());
    SetFollowersCount(newAuthor->GetFollowersCount());
    SetLargeImageUrl(newAuthor->GetLargeImageUrl());
    SetImageUrl(newAuthor->GetImageUrl());
    SetSmallImageUrl(newAuthor->GetSmallImageUrl());
    SetAbout(newAuthor->GetAbout());
    SetWorksCount(newAuthor->GetWorksCount());
    SetGender(newAuthor->GetGender());
    SetHomeTown(newAuthor->GetHomeTown());
    SetBirthDate(newAuthor->GetBirthDate());
    SetDeathDate(newAuthor->GetDeathDate());
    SetIsGoodreadsAuthor(newAuthor->GetIsGoodreadsAuthor());
    SetUserId(newAuthor->GetUserId());
    SetBooks(newAuthor->GetBooksPtr());
    SetFollowingId(newAuthor->GetFollowingId());
}


} // namespace Sailreads
