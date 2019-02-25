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

#include <QQmlParserStatus>

#include "basebooksmodel.h"

namespace Sailreads
{
class AuthorBooksModel : public BaseBooksModel
{
    Q_OBJECT

    QString m_AuthorId;
    Q_PROPERTY(QString authorId READ GetAuthorId WRITE SetAuthorId NOTIFY authorIdChanged)

public:
    AuthorBooksModel(QObject *parent = nullptr);

    virtual void classBegin() override;
    virtual void componentComplete() override;

    QString GetAuthorId() const;
    void SetAuthorId(const QString& authorId);

public slots:
    virtual void fetchMoreContent() override;
    void loadAuthorBooks();
    void handleGotAuthorBooks(const QString& authorId, const CountedItems<BookPtr>& books);
    void handleBookAddedToShelves(const QString& bookId, const QStringList& shelves,
            const QStringList& oldShelves, const ReviewPtr& review);

signals:
    void authorIdChanged();
};

} // namespace Sailreads
