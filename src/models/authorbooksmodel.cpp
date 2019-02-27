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

#include "authorbooksmodel.h"

#include "../sailreadsmanager.h"
#include "../objects/book.h"
#include "../objects/review.h"

namespace Sailreads
{
AuthorBooksModel::AuthorBooksModel(QObject *parent)
: BaseBooksModel(parent)
, m_AuthorId("")
{
}

void AuthorBooksModel::classBegin()
{
    auto sm = SailreadsManager::Instance();
    connect(sm, &SailreadsManager::gotAuthorBooks,
            this, &AuthorBooksModel::handleGotAuthorBooks);
    connect(SailreadsManager::Instance(), &SailreadsManager::bookAddedToShelves,
            this, &AuthorBooksModel::handleBookAddedToShelves);
}

void AuthorBooksModel::componentComplete()
{
}

QString AuthorBooksModel::GetAuthorId() const
{
    return m_AuthorId;
}

void AuthorBooksModel::SetAuthorId(const QString& authorId)
{
    if (m_AuthorId == authorId) {
        return;
    }

    m_AuthorId = authorId;
    m_CurrentPage = 1;
    emit authorIdChanged();
}

void AuthorBooksModel::fetchMoreContent()
{
    SailreadsManager::Instance()->loadAuthorBooks(this, m_AuthorId, m_CurrentPage);
}

void AuthorBooksModel::loadAuthorBooks()
{
    if (m_AuthorId.isEmpty()) {
        return;
    }
    m_CurrentPage = 1;
    SailreadsManager::Instance()->loadAuthorBooks(this, m_AuthorId, m_CurrentPage, false);
}

void AuthorBooksModel::handleGotAuthorBooks(const QString& authorId, const CountedItems<BookPtr>& books)
{
    if (m_AuthorId != authorId) {
        return;
    }
    handleGotBooks(books);
}

void AuthorBooksModel::handleBookAddedToShelves(const QString& bookId, const QStringList& /*shelves*/,
        const QStringList& /*oldShelves*/, const ReviewPtr& review)
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
