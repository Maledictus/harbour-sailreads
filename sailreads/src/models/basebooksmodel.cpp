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

#include "basebooksmodel.h"

#include "../objects/book.h"

namespace Sailreads
{
BaseBooksModel::BaseBooksModel(QObject *parent)
: BaseModel<BookPtr>(parent)
, m_HasMore(true)
, m_CurrentPage(1)
{
}

QVariant BaseBooksModel::data(const QModelIndex& index, int role) const
{
    if (index.row() > m_Items.count() - 1 || index.row() < 0) {
        return QVariant();
    }

    const auto& bookItem = m_Items.at(index.row());
    switch (role) {
    case Id:
        return bookItem->GetId();
    case Book:
        return QVariant::fromValue(bookItem.get());
    case ImageUrl:
        return bookItem->GetImageUrl();
    case Title:
        return bookItem->GetTitle();
    case Authors:
        return bookItem->GetAuthorsString();
    case AverageRating:
        return bookItem->GetAverageRating();
    case RatingsCount:
        return bookItem->GetRatingsCount();
    default:
        return QVariant();
    }
}

QHash<int, QByteArray> BaseBooksModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[Id] = "bookId";
    roles[Book] = "bookBook";
    roles[ImageUrl] = "bookImageUrl";
    roles[Title] = "bookTitle";
    roles[Authors] = "bookAuthors";
    roles[AverageRating] = "bookAverageRating";
    roles[RatingsCount] = "bookRatingsCount";
    return roles;
}

bool BaseBooksModel::GetHasMore() const
{
    return m_HasMore;
}

void BaseBooksModel::SetHasMore(bool has)
{
    if (m_HasMore != has) {
        m_HasMore = has;
        emit hasMoreChanged();
    }
}

void BaseBooksModel::handleGotBooks(const CountedItems<BookPtr>& books)
{
    SetHasMore(books.m_EndIndex != books.m_Count);
    if (m_HasMore) {
        ++m_CurrentPage;
    }

    if (books.m_BeginIndex == 1) {
        SetItems(books.m_Items);
    }
    else {
        AddItems(books.m_Items);
    }
}
} // namespace Sailreads
