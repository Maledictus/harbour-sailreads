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

#include <QObject>
#include <QString>
#include <QUrl>

namespace Sailreads
{
class Author
{
    Q_GADGET

    quint64 m_Id;
    QString m_Name;
    QUrl m_ImageUrl;
    QUrl m_SmallImageUrl;
    QUrl m_Link;
    qreal m_AverageRating;
    quint64 m_RatingsCount;
    quint64 m_TextReviewsCount;

    Q_PROPERTY(quint64 id READ GetId)
    Q_PROPERTY(QString name READ GetName)
    Q_PROPERTY(QUrl imageUrl READ GetImageUrl)
    Q_PROPERTY(QUrl smallImageUrl READ GetSmallImageUrl)
    Q_PROPERTY(QUrl link READ GetLink)
    Q_PROPERTY(qreal averageRating READ GetAverageRating)
    Q_PROPERTY(quint64 ratingsCount READ GetRatingsCount)
    Q_PROPERTY(quint64 textReviewsCount READ GetTextReviewsCount)

public:
    Author();

    quint64 GetId() const;
    void SetId(const quint64& id);
    QString GetName() const;
    void SetName(const QString& name);
    QUrl GetImageUrl() const;
    void SetImageUrl(const QUrl& imageUrl);
    QUrl GetSmallImageUrl() const;
    void SetSmallImageUrl(const QUrl& smallImageUrl);
    QUrl GetLink() const;
    void SetLink(const QUrl& link);
    qreal GetAverageRating() const;
    void SetAverageRating(const qreal& averageRating);
    quint64 GetRatingsCount() const;
    void SetRatingsCount(const quint64& ratingsCount);
    quint64 GetTextReviewsCount() const;
    void SetTextReviewsCount(const quint64& textReviewsCount);
};
typedef QList<Author> Authors_t;
} // namespace Sailreads
Q_DECLARE_METATYPE(Sailreads::Author)
