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

#include <QObject>

#include "../../types.h"

namespace Sailreads
{
class ReviewUpdate : public QObject
{
    Q_OBJECT

    QString m_Body;
    int m_Rating;
    BookPtr m_Book;

    Q_PROPERTY(QString body READ GetBody NOTIFY bodyChanged)
    Q_PROPERTY(Book* book READ GetBook NOTIFY bookChanged)
    Q_PROPERTY(int rating READ GetRating NOTIFY ratingChanged)

public:
    ReviewUpdate(QObject *parent = nullptr);

    QString GetBody() const;
    void SetBody(const QString& body);
    BookPtr GetBookPtr() const;
    Book* GetBook() const;
    void SetBook(const BookPtr& book);
    int GetRating() const;
    void SetRating(int rating);

signals:
    void bodyChanged();
    void bookChanged();
    void ratingChanged();
};

} // namespace Sailreads
