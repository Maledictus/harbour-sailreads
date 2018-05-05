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

#include "author.h"

#include <QtDebug>

namespace Sailreads
{
Author::Author(QObject *parent)
: QObject(parent)
, m_Id(0)
, m_AverageRating(0)
, m_RatingsCount(0)
, m_TextReviewsCount(0)
{
    qDebug() << this << "CONSTRUCTED";
}

Author::~Author()
{
    qDebug() << this << "DESTRUCTED";
}

quint64 Author::GetId() const
{
    return m_Id;
}

void Author::SetId(const quint64& id)
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

void Author::SetAverageRating(const qreal& averageRating)
{
    m_AverageRating = averageRating;
}

quint64 Author::GetRatingsCount() const
{
    return m_RatingsCount;
}

void Author::SetRatingsCount(const quint64& ratingsCount)
{
    m_RatingsCount = ratingsCount;
}

quint64 Author::GetTextReviewsCount() const
{
    return m_TextReviewsCount;
}

void Author::SetTextReviewsCount(const quint64& textReviewsCount)
{
    m_TextReviewsCount = textReviewsCount;
}

} // namespace Sailreads
