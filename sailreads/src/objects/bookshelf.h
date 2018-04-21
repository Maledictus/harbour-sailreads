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
class BookShelf
{
    quint64 m_Id;
    QString m_Name;
    quint32 m_BooksCount;
    QString m_Description;
    bool m_Exclusive;
    bool m_Featured;
    bool m_Sortable;
public:
    explicit BookShelf();

    bool IsValid() const;

    quint64 GetId() const;
    void SetId(quint64 id);
    QString GetName() const;
    void SetName(const QString& name);
    quint32 GetBooksCount() const;
    void SetBooksCount(quint32 count);
    QString GetDescription() const;
    void SetDescription(const QString& description);
    bool GetExclusive() const;
    void SetExclusive(bool exclusive);
    bool GetFeatured() const;
    void SetFeatured(bool featured);
    bool GetSortable() const;
    void SetSortable(bool sortable);

    bool operator ==(const BookShelf& shelf) const;
};

typedef QList<BookShelf> BookShelves_t;
} // namespace Sailreads
