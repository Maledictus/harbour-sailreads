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
#include "../sailreadsmanager.h"

namespace Sailreads
{
BookItem::BookItem(QObject *parent)
: ItemRequestCanceler(parent)
, m_BookId("")
{
    connect(SailreadsManager::Instance(), &SailreadsManager::gotBook,
            this, &BookItem::handleGotBook);
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

void BookItem::loadBook()
{
    if (m_BookId.isEmpty()) {
        return;
    }
    SailreadsManager::Instance()->loadBook(this, m_BookId);
}
} // namespace Sailreads
