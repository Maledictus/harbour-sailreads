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

#include "../itemrequestcanceler.h"
#include "../types.h"

namespace Sailreads
{
class BookItem: public ItemRequestCanceler
{
    Q_OBJECT

    QString m_BookId;
    BookPtr m_Book;

    Q_PROPERTY(QString bookId READ GetBookId WRITE SetBookId NOTIFY bookIdChanged)
    Q_PROPERTY(Book* book READ GetBook WRITE SetBook NOTIFY bookChanged)
public:
    BookItem(QObject *parent = nullptr);

    QString GetBookId() const;
    void SetBookId(const QString& bookId);
    Book* GetBook() const;
    void SetBook(Book *book);
    void SetBook(const BookPtr& book);

private slots:
    void handleGotBook(const BookPtr& book);
    void handleGotReviewInfo(const ReviewInfo& reviewInfo);
public slots:
    void loadBook();

signals:
    void bookIdChanged();
    void bookChanged();
};

} // namespace Sailreads
