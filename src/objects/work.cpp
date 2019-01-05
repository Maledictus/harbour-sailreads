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
#include "work.h"

#ifdef QT_DEBUG
    #include <QtDebug>
#endif

namespace Sailreads
{
Work::Work(QObject *parent)
: QObject(parent)
, m_Id(0)
, m_BooksCount(0)
, m_BestBooksId(0)
, m_ReviewsCount(0)
, m_RatingSum(0)
, m_RatingsCount(0)
, m_OriginalPublicationYear(0)
, m_OriginalPublicationMonth(0)
, m_OriginalPublicationDay(0)
{
#ifdef QT_DEBUG
    qDebug() << this << "CONSTRUCTED";
#endif
}

Work::~Work()
{
#ifdef QT_DEBUG
    qDebug() << this << "DESTRUCTED";
#endif
}

quint64 Work::GetId() const
{
    return m_Id;
}

void Work::SetId(quint64 id)
{
    m_Id = id;
}

int Work::GetBooksCount() const
{
    return m_BooksCount;
}

void Work::SetBooksCount(int booksCount)
{
    m_BooksCount = booksCount;
}

quint64 Work::GetBestBooksId() const
{
    return m_BestBooksId;
}

void Work::SetBestBooksId(quint64 bestBooksId)
{
    m_BestBooksId = bestBooksId;
}

quint64 Work::GetReviewsCount() const
{
    return m_ReviewsCount;
}

void Work::SetReviewsCount(quint64 reviewsCount)
{
    m_ReviewsCount = reviewsCount;
}

quint64 Work::GetRatingSum() const
{
    return m_RatingSum;
}

void Work::SetRatingSum(quint64 ratingSum)
{
    m_RatingSum = ratingSum;
}

quint64 Work::GetRatingsCount() const
{
    return m_RatingsCount;
}

void Work::SetRatingsCount(quint64 ratingsCount)
{
    m_RatingsCount = ratingsCount;
}

quint64 Work::GetTextReviewsCount() const
{
    return m_TextReviewsCount;
}

void Work::SetTextReviewsCount(quint64 textReviewsCount)
{
    m_TextReviewsCount = textReviewsCount;
}

int Work::GetOriginalPublicationYear() const
{
    return m_OriginalPublicationYear;
}

void Work::SetOriginalPublicationYear(int originalPublicationYear)
{
    m_OriginalPublicationYear = originalPublicationYear;
}

int Work::GetOriginalPublicationMonth() const
{
    return m_OriginalPublicationMonth;
}

void Work::SetOriginalPublicationMonth(int originalPublicationMonth)
{
    m_OriginalPublicationMonth = originalPublicationMonth;
}

int Work::GetOriginalPublicationDay() const
{
    return m_OriginalPublicationDay;
}

void Work::SetOriginalPublicationDay(int originalPublicationDay)
{
    m_OriginalPublicationDay = originalPublicationDay;
}

QString Work::GetOriginalTitle() const
{
    return m_OriginalTitle;
}

void Work::SetOriginalTitle(const QString& originalTitle)
{
    m_OriginalTitle = originalTitle;
}

QString Work::GetRatingDist() const
{
    return m_RatingDist;
}

void Work::SetRatingDist(const QString& ratingDist)
{
    m_RatingDist = ratingDist;
}

qreal Work::GetAverageRating() const
{
    return m_RatingsCount > 0 ? static_cast<qreal>(m_RatingSum) / m_RatingsCount : 0;
}

Book *Work::GetBestBook() const
{
    return m_BestBook.get();
}

void Work::SetBestBook(const BookPtr& book)
{
    m_BestBook = book;
}
} // namespace Sailreads
