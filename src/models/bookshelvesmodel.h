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

#include <QQmlParserStatus>

#include "basemodel.h"
#include "../bookshelf.h"

namespace Sailreads
{
class BookShelvesModel : public BaseModel<BookShelf>
{
    Q_OBJECT

    Q_ENUMS(BookShelfRoles)

    quint64 m_UserId;

    Q_PROPERTY(quint64 userId READ GetUserId WRITE SetUserId NOTIFY userIdChanged)
public:
    enum BookShelfRoles
    {
        Id = Qt::UserRole + 1,
        Name,
        BooksCount,
        Description,
        Exclusive,
        Featured,
        Sortable
    };

    explicit BookShelvesModel(QObject *parent = 0);
    virtual ~BookShelvesModel();

    virtual QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    virtual QHash<int, QByteArray> roleNames() const override;

    quint64 GetUserId() const;
    void SetUserId(quint64 id);

private slots:
    void handleGotUserBookShelves(quint64 userId, const BookShelves_t& bookshelves);
    void handleBookShelfAdded(const BookShelf& shelf);
    void handleBookShelfEdited(const BookShelf& shelf);

signals:
    void userIdChanged();
};
} // namespace Sailreads
