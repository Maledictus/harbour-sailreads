/*
Copyright (c) 2018 Oleg Linkin <maledictusdemagog@gmail.com>

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

namespace Sailreads
{
class Work
{
    quint64 m_Id;
    int m_BooksCount;
    quint64 m_BestBooksId;
    quint64 m_ReviewsCount;
    quint64 m_RatingSum;
    quint64 m_RatingCount;
    quint64 m_TextReviewsCount;
    int m_OriginalPublicationYear;
    int m_OriginalPublicationMonth;
    int m_OriginalPublicationDay;
    QString m_OriginalTitle;
    QString m_RatingDist;

public:
    Work();
    ~Work();
    Work(const Work&) = default;

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
    quint64 GetRatingCount() const;
    void SetRatingCount(quint64 ratingCount);
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
};

} // namespace Sailreads
