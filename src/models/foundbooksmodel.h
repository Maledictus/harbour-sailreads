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

#include "basebooksmodel.h"
#include "../types.h"

namespace Sailreads
{
class FoundBooksModel : public BaseBooksModel
{
    Q_OBJECT

    QString m_Search;
    QString m_Field;

    Q_PROPERTY(QString search READ GetSearch WRITE SetSearch NOTIFY searchChanged)
    Q_PROPERTY(QString field READ GetField WRITE SetField NOTIFY fieldChanged)

public:
    explicit FoundBooksModel(QObject *parent = nullptr);

    QVariant data(const QModelIndex& index, int role) const;

    virtual void classBegin() override;
    virtual void componentComplete() override;

    QString GetSearch() const;
    void SetSearch(const QString& search);
    QString GetField() const;
    void SetField(const QString& field);

public slots:
    void searchBooks();
    virtual void fetchMoreContent() override;
private slots:
    void handleGotFoundBooks(const CountedItems<BookPtr>& books);
    void handleBookAddedToShelves(const QString& bookId, const QStringList& shelves,
            const ReviewPtr& review);

signals:
    void searchChanged();
    void fieldChanged();
};

} // namespace Sailreads


