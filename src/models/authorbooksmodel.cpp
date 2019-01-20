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
    loadAuthorBooks();
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
    SailreadsManager::Instance()->loadAuthorBooks(this, m_AuthorId);
}

void AuthorBooksModel::handleGotAuthorBooks(const QString& authorId, const CountedItems<BookPtr>& books)
{
    if (m_AuthorId != authorId) {
        return;
    }
    handleGotBooks(books);
}

} // namespace Sailreads
