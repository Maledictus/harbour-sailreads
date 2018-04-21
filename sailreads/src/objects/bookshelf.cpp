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

#include "bookshelf.h"

namespace Sailreads
{
BookShelf::BookShelf()
: m_Id(0)
, m_Exclusive(false)
, m_Featured(false)
, m_Sortable(false)
{
}

bool BookShelf::IsValid() const
{
    return m_Id > 0 && !m_Name.isEmpty();
}

quint64 BookShelf::GetId() const
{
    return m_Id;
}

void BookShelf::SetId(quint64 id)
{
    m_Id = id;
}

QString BookShelf::GetName() const
{
    return m_Name;
}

void BookShelf::SetName(const QString& name)
{
    m_Name = name;
}

quint32 BookShelf::GetBooksCount() const
{
    return m_BooksCount;
}

void BookShelf::SetBooksCount(quint32 count)
{
    m_BooksCount = count;
}

QString BookShelf::GetDescription() const
{
    return m_Description;
}

void BookShelf::SetDescription(const QString& description)
{
    m_Description = description;
}

bool BookShelf::GetExclusive() const
{
    return m_Exclusive;
}

void BookShelf::SetExclusive(bool exclusive)
{
    m_Exclusive = exclusive;
}

bool BookShelf::GetFeatured() const
{
    return m_Featured;
}

void BookShelf::SetFeatured(bool featured)
{
    m_Featured = featured;
}

bool BookShelf::GetSortable() const
{
    return m_Sortable;
}

void BookShelf::SetSortable(bool sortable)
{
    m_Sortable = sortable;
}

bool BookShelf::operator ==(const BookShelf& shelf) const
{
    return m_Id == shelf.GetId() &&
        m_Name == shelf.GetName() &&
        m_BooksCount == shelf.GetBooksCount() &&
        m_Description == shelf.GetDescription() &&
        m_Exclusive == shelf.GetExclusive() &&
        m_Featured == shelf.GetFeatured() &&
        m_Sortable == shelf.GetSortable();
}
} // namespace Sailreads
