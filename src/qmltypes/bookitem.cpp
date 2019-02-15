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

#include "bookitem.h"

#include "../objects/book.h"
#include "../objects/review.h"
#include "../sailreadsmanager.h"

namespace Sailreads
{
BookItem::BookItem(QObject *parent)
: ItemRequestCanceler(parent)
, m_BookId("")
{
    connect(SailreadsManager::Instance(), &SailreadsManager::gotBook,
            this, &BookItem::handleGotBook);
    connect(SailreadsManager::Instance(), &SailreadsManager::gotReviewInfo,
            this, &BookItem::handleGotReviewInfo);
    connect(SailreadsManager::Instance(), &SailreadsManager::bookAddedToShelves,
            this, &BookItem::handleBookAddedToShelves);
    connect(SailreadsManager::Instance(), &SailreadsManager::bookAddedToShelf,
            this, &BookItem::handleBookAddedToShelf);
    connect(SailreadsManager::Instance(), &SailreadsManager::bookRemovedFromShelf,
            this, &BookItem::handleBookRemovedFromShelf);
}

QString BookItem::GetBookId() const
{
    return m_BookId;
}

void BookItem::SetBookId(const QString& bookId)
{
    if (m_BookId == bookId) {
        return;
    }

    m_BookId = bookId;
    loadBook();
    emit bookIdChanged();
}

Book* BookItem::GetBook() const
{
    return m_Book.get();
}

void BookItem::SetBook(Book *book)
{
    if (!book) {
        return;
    }

    if (!m_Book) {
        m_Book = BookPtr(new Book());
    }
    m_Book->Update(book);
    emit bookChanged();
}

void BookItem::SetBook(const BookPtr& book)
{
    if (!book) {
        return;
    }

    if (!m_Book) {
        m_Book = BookPtr(new Book());
    }
    m_Book->Update(book);
    emit bookChanged();
}

void BookItem::handleGotBook(const BookPtr& book)
{
    if (!book || (m_BookId != book->GetId() && !m_BookId.startsWith(book->GetId()))) {
        return;
    }
    m_BookId = book->GetId();
    emit bookIdChanged();
    SetBook(book);
}

void BookItem::handleGotReviewInfo(const ReviewInfo& reviewInfo)
{
    if (!m_Book || m_BookId != reviewInfo.m_BookId || !m_Book->GetReviewPtr() ||
            m_Book->GetReviewPtr()->GetId() != reviewInfo.m_ReviewId) {
        return;
    }
    m_Book->GetReviewPtr()->SetRating(reviewInfo.m_Rating);
    m_Book->GetReviewPtr()->SetAddedDate(reviewInfo.m_CreateDate);
    m_Book->GetReviewPtr()->SetUpdatedDate(reviewInfo.m_UpdateDate);
    m_Book->GetReviewPtr()->SetReadDate(reviewInfo.m_ReadDate);
    m_Book->GetReviewPtr()->SetStartedDate(reviewInfo.m_StartedDate);
    m_Book->GetReviewPtr()->SetReadCount(reviewInfo.m_ReadCount);
    m_Book->GetReviewPtr()->SetBody(reviewInfo.m_Review);
    emit bookChanged();
}

void BookItem::handleBookAddedToShelf(const QString& bookId, const ReviewPtr& review)
{
    if (!m_Book || m_BookId != bookId || !m_Book->GetReview()) {
        return;
    }

    if (m_Book->GetReview()->GetId() == review->GetId())
    {
        if (!review->GetShelves()[0].GetExclusive())
        {
            m_Book->GetReview()->SetShelves(m_Book->GetReview()->GetShelves() + review->GetShelves());
        }
        else
        {
            auto shelves = m_Book->GetReview()->GetShelves();
            for(int i = 0; i < shelves.count(); ++i) {
                if(shelves.at(i).GetExclusive()) {
                    shelves.removeAt(i);
                }
            }
            m_Book->GetReview()->SetShelves(shelves + review->GetShelves());
        }

        emit bookChanged();
    }
}

void BookItem::handleBookRemovedFromShelf(const QString& bookId, const QString& shelfName)
{
    if (!m_Book || m_BookId != bookId || !m_Book->GetReview()) {
        return;
    }

    auto shelves = m_Book->GetReview()->GetShelves();
    for(int i = 0; i < shelves.count(); ++i) {
        if(shelves.at(i).GetName() == shelfName) {
            shelves.removeAt(i);
            break;
        }
    }
    m_Book->GetReview()->SetShelves(shelves);
    emit bookChanged();
}

void BookItem::handleBookAddedToShelves(const QString& bookId, const ReviewPtr& review)
{
    if (!m_Book || m_BookId != bookId) {
        return;
    }

    if (!m_Book->GetReview())
    {
        m_Book->SetReview(review);
    }
    else if (m_Book->GetReview()->GetId() == review->GetId())
    {
        m_Book->GetReview()->SetUpdatedDate(review->GetUpdatedDate());
        m_Book->GetReview()->SetAddedDate(review->GetAddedDate());
        m_Book->GetReview()->SetReadDate(review->GetReadDate());
        m_Book->GetReview()->SetStartedDate(review->GetStartedDate());
        m_Book->GetReview()->SetRating(review->GetRating());
        m_Book->GetReview()->SetShelves(review->GetShelves());
    }
    emit bookChanged();
}

void BookItem::loadBook()
{
    if (m_BookId.isEmpty()) {
        return;
    }
    SailreadsManager::Instance()->loadBook(this, m_BookId);
}
} // namespace Sailreads
