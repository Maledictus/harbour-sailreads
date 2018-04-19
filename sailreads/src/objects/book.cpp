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

#include "book.h"

namespace Sailreads
{
Book::Book()
: m_Id(0)
, m_TextReviewsCount(0)
, m_NumPages(0)
, m_PublicationYear(0)
, m_PublicationMonth(0)
, m_PublicationDay(0)
, m_AverageRating(0)
, m_RatingsCount(0)
, m_PublishedYear(0)
, m_WorkId(0)
{
}

quint64 Book::GetId() const
{
    return m_Id;
}

void Book::SetId(quint64 id)
{
    m_Id = id;
}

QString Book::GetISBN() const
{
    return m_ISBN;
}

void Book::SetISBN(const QString& isbn)
{
    m_ISBN = isbn;
}

QString Book::GetISBN13() const
{
    return m_ISBN13;
}

void Book::SetISBN13(const QString& isbn13)
{
    m_ISBN13 = isbn13;
}

quint64 Book::GetTextReviewsCount() const
{
    return m_TextReviewsCount;
}

void Book::SetTextReviewsCount(const quint64& textReviewsCount)
{
    m_TextReviewsCount = textReviewsCount;
}

QString Book::GetTitle() const
{
    return m_Title;
}

void Book::SetTitle(const QString& title)
{
    m_Title = title;
}

QString Book::GetTitleWithoutSeries() const
{
    return m_TitleWithoutSeries;
}

void Book::SetTitleWithoutSeries(const QString& titleWithoutSeries)
{
    m_TitleWithoutSeries = titleWithoutSeries;
}

QUrl Book::GetImageUrl() const
{
    return m_ImageUrl;
}

void Book::SetImageUrl(const QUrl& imageUrl)
{
    m_ImageUrl = imageUrl;
}

QUrl Book::GetSmallImageUrl() const
{
    return m_SmallImageUrl;
}

void Book::SetSmallImageUrl(const QUrl& smallImageUrl)
{
    m_SmallImageUrl = smallImageUrl;
}

QUrl Book::GetLargeImageUrl() const
{
    return m_LargeImageUrl;
}

void Book::SetLargeImageUrl(const QUrl& largeImageUrl)
{
    m_LargeImageUrl = largeImageUrl;
}

QUrl Book::GetLink() const
{
    return m_Link;
}

void Book::SetLink(const QUrl& link)
{
    m_Link = link;
}

int Book::GetNumPages() const
{
    return m_NumPages;
}

void Book::SetNumPages(int numPages)
{
    m_NumPages = numPages;
}

QString Book::GetFormat() const
{
    return m_Format;
}

void Book::SetFormat(const QString& format)
{
    m_Format = format;
}

QString Book::GetEditionInformation() const
{
    return m_EditionInformation;
}

void Book::SetEditionInformation(const QString& info)
{
    m_EditionInformation = info;
}

QString Book::GetPublisher() const
{
    return m_Publisher;
}

void Book::SetPublisher(const QString& publisher)
{
    m_Publisher = publisher;
}

int Book::GetPublicationYear() const
{
    return m_PublicationYear;
}

void Book::SetPublicationYear(int publicationYear)
{
    m_PublicationYear = publicationYear;
}

int Book::GetPublicationMonth() const
{
    return m_PublicationMonth;
}

void Book::SetPublicationMonth(int publicationMonth)
{
    m_PublicationMonth = publicationMonth;
}

int Book::GetPublicationDay() const
{
    return m_PublicationDay;
}

void Book::SetPublicationDay(int publicationDay)
{
    m_PublicationDay = publicationDay;
}

qreal Book::GetAverageRating() const
{
    return m_AverageRating;
}

void Book::SetAverageRating(const qreal& averageRating)
{
    m_AverageRating = averageRating;
}

quint64 Book::GetRatingsCount() const
{
    return m_RatingsCount;
}

void Book::SetRatingsCount(const quint64& ratingsCount)
{
    m_RatingsCount = ratingsCount;
}

QString Book::GetDescription() const
{
    return m_Description;
}

void Book::SetDescription(const QString& description)
{
    m_Description = description;
}

Authors_t Book::GetAuthors() const
{
    return m_Authors;
}

void Book::SetAuthors(const Authors_t& authors)
{
    m_Authors = authors;
}

QString Book::GetAuthorsString() const
{
    QString result;
    for (int i = 0, count = m_Authors.size(); i < count; ++i) {
        result += m_Authors[i].GetName();
        if (i + 1 < count) {
            result += ", ";
        }
    }
    return result;
}

int Book::GetPublishedYear() const
{
    return m_PublishedYear;
}

void Book::SetPublishedYear(int publishedYear)
{
    m_PublishedYear = publishedYear;
}

quint64 Book::GetWorkId() const
{
    return m_WorkId;
}

void Book::SetWorkId(const quint64& workId)
{
    m_WorkId = workId;
}

} // namespace Sailreads