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

#include <QString>
#include <QObject>

#include "../types.h"

namespace Sailreads
{
class Work: public QObject
{
    Q_OBJECT

    quint64 m_Id;
    int m_BooksCount;
    quint64 m_BestBooksId;
    BookPtr m_BestBook;
    quint64 m_ReviewsCount;
    quint64 m_RatingSum;
    quint64 m_RatingsCount;
    quint64 m_TextReviewsCount;
    int m_OriginalPublicationYear;
    int m_OriginalPublicationMonth;
    int m_OriginalPublicationDay;
    QString m_OriginalTitle;
    QString m_RatingDist;
    qreal m_AverageRating;

    Q_PROPERTY(quint64 id READ GetId NOTIFY idChanged)
    Q_PROPERTY(int booksCount READ GetBooksCount NOTIFY booksCountChanged)
    Q_PROPERTY(quint64 bestBooksId READ GetBestBooksId NOTIFY bestBookIdChanged)
    Q_PROPERTY(Book* bestBook READ GetBestBook NOTIFY bestBookChanged)
    Q_PROPERTY(quint64 reviewsCount READ GetReviewsCount NOTIFY reviewsCountChanged)
    Q_PROPERTY(quint64 ratingSum READ GetRatingSum NOTIFY ratingSumChanged)
    Q_PROPERTY(quint64 ratingsCount READ GetRatingsCount NOTIFY ratingCountChanged)
    Q_PROPERTY(quint64 textReviewsCount READ GetTextReviewsCount NOTIFY textReviewsCountChanged)
    Q_PROPERTY(int originalPublicationYear READ GetOriginalPublicationYear NOTIFY originalPublicationYearChanged)
    Q_PROPERTY(int originalPublicationMonth READ GetOriginalPublicationMonth NOTIFY originalPublicationMonthChanged)
    Q_PROPERTY(int originalPublicationDay READ GetOriginalPublicationDay NOTIFY originalPublicationDayChanged)
    Q_PROPERTY(QString originalTitle READ GetOriginalTitle NOTIFY originalTitleChanged)
    Q_PROPERTY(QString ratingDist READ GetRatingDist NOTIFY ratingDistChanged)
    Q_PROPERTY(qreal averageRating READ GetAverageRating NOTIFY averageRatingChanged)

public:
    Work(QObject *parent = nullptr);
    ~Work();

    quint64 GetId() const;
    void SetId(quint64 id);
    int GetBooksCount() const;
    void SetBooksCount(int booksCount);
    quint64 GetBestBooksId() const;
    void SetBestBooksId(quint64 bestBooksId);
    quint64 GetReviewsCount() const;
    void SetReviewsCount(quint64 reviewsCount);
    quint64 GetRatingSum() const;
    void SetRatingSum(quint64 ratingSum);
    quint64 GetRatingsCount() const;
    void SetRatingsCount(quint64 ratingsCount);
    quint64 GetTextReviewsCount() const;
    void SetTextReviewsCount(quint64 textReviewsCount);
    int GetOriginalPublicationYear() const;
    void SetOriginalPublicationYear(int originalPublicationYear);
    int GetOriginalPublicationMonth() const;
    void SetOriginalPublicationMonth(int originalPublicationMonth);
    int GetOriginalPublicationDay() const;
    void SetOriginalPublicationDay(int originalPublicationDay);
    QString GetOriginalTitle() const;
    void SetOriginalTitle(const QString& originalTitle);
    QString GetRatingDist() const;
    void SetRatingDist(const QString& ratingDist);
    qreal GetAverageRating() const;
    void SetAverageRating(const qreal& averageRating);
    Book* GetBestBook() const;
    void SetBestBook(const BookPtr& book);

signals:
    void idChanged();
    void booksCountChanged();
    void bestBookIdChanged();
    void reviewsCountChanged();
    void ratingSumChanged();
    void ratingCountChanged();
    void textReviewsCountChanged();
    void originalPublicationYearChanged();
    void originalPublicationMonthChanged();
    void originalPublicationDayChanged();
    void originalTitleChanged();
    void ratingDistChanged();
    void averageRatingChanged();
    void bestBookChanged();
};

} // namespace Sailreads
