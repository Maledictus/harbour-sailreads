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

#include "foundbooksmodel.h"

#include <QtDebug>

#include "../objects/book.h"
#include "../objects/review.h"
#include "../objects/work.h"
#include "../sailreadsmanager.h"

namespace Sailreads
{
FoundBooksModel::FoundBooksModel(QObject *parent)
: BaseBooksModel(parent)
{
}

QVariant FoundBooksModel::data(const QModelIndex &index, int role) const
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
        return bookItem->GetWork()->GetAverageRating();
    case RatingsCount:
        return bookItem->GetWork()->GetRatingsCount();
    default:
        return QVariant();
    }
}

void FoundBooksModel::classBegin()
{
    auto sm = SailreadsManager::Instance();
    connect(sm, &SailreadsManager::gotFoundBooks,
            this, &FoundBooksModel::handleGotFoundBooks);
    connect(SailreadsManager::Instance(), &SailreadsManager::bookAddedToShelves,
            this, &FoundBooksModel::handleBookAddedToShelves);
}

void FoundBooksModel::componentComplete()
{
}

QString FoundBooksModel::GetSearch() const
{
    return m_Search;
}

void FoundBooksModel::SetSearch(const QString& search)
{
    if (m_Search != search) {
        m_Search = search;
        emit searchChanged();
    }
}

QString FoundBooksModel::GetField() const
{
    return m_Field;
}

void FoundBooksModel::SetField(const QString& field)
{
    if (m_Field != field) {
        m_Field = field;
        emit fieldChanged();
    }
}

void FoundBooksModel::searchBooks()
{
    if (m_Search.isEmpty()) {
        return;
    }
    SailreadsManager::Instance(this)->searchBooks(this, m_Search, m_Field);
}

void FoundBooksModel::fetchMoreContent()
{
    if (m_Search.isEmpty()) {
        return;
    }
    SailreadsManager::Instance(this)->searchBooks(this, m_Search, m_Field, m_CurrentPage);
}

void FoundBooksModel::handleGotFoundBooks(const CountedItems<BookPtr>& books)
{
    if (!books.m_BeginIndex && !books.m_EndIndex)
    {
        Clear();
    }
    else if (books.m_BeginIndex == 1) {
        m_CurrentPage = 1;
        SetItems(books.m_Items);
    }
    else {
        AddItems(books.m_Items);
    }

    SetHasMore(books.m_EndIndex != books.m_Count);
    if (m_HasMore) {
        ++m_CurrentPage;
    }
}

void FoundBooksModel::handleBookAddedToShelves(const QString& bookId, const QStringList&,
        const ReviewPtr& review)
{
    auto it = std::find_if(m_Items.begin(), m_Items.end(),
            [bookId](decltype(m_Items.front()) book)
            { return book->GetId() == bookId; });
    if (it != m_Items.end())
    {
        int pos = std::distance(m_Items.begin(), it);
        if (!m_Items[pos]->GetReview())
        {
            m_Items[pos]->SetReview(review);
        }
        else if (m_Items[pos]->GetReview()->GetId() == review->GetId())
        {
            m_Items[pos]->GetReview()->SetUpdatedDate(review->GetUpdatedDate());
            m_Items[pos]->GetReview()->SetAddedDate(review->GetAddedDate());
            m_Items[pos]->GetReview()->SetReadDate(review->GetReadDate());
            m_Items[pos]->GetReview()->SetStartedDate(review->GetStartedDate());
            m_Items[pos]->GetReview()->SetRating(review->GetRating());
            m_Items[pos]->GetReview()->SetShelves(review->GetShelves());
        }
        emit dataChanged(index(pos), index(pos));
    }
}
} // namespace Sailreads
