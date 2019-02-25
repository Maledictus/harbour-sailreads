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

#include "authorprofileitem.h"

#include <QtDebug>

#include "../models/authorbooksmodel.h"
#include "../objects/author.h"
#include "../sailreadsmanager.h"

namespace Sailreads
{
AuthorProfileItem::AuthorProfileItem(QObject *parent)
: ItemRequestCanceler(parent)
, m_AuthorId("")
, m_ShortBooksModel(new AuthorBooksModel(this))
{
    connect(SailreadsManager::Instance(), &SailreadsManager::gotAuthorProfile,
            this, &AuthorProfileItem::handleGotAuthorProfile);
    connect(SailreadsManager::Instance(), &SailreadsManager::authorFollowed,
            this, &AuthorProfileItem::handleAuthorFollowed);
    connect(SailreadsManager::Instance(), &SailreadsManager::authorUnfollowed,
            this, &AuthorProfileItem::handleAuthorUnfollowed);
    connect(SailreadsManager::Instance(), &SailreadsManager::bookAddedToShelves,
            m_ShortBooksModel, &AuthorBooksModel::handleBookAddedToShelves);
}

QString AuthorProfileItem::GetAuthorId() const
{
    return m_AuthorId;
}

void AuthorProfileItem::SetAuthorId(const QString& authorId)
{
    if (m_AuthorId == authorId) {
        return;
    }

    m_AuthorId = authorId;
    m_ShortBooksModel->SetAuthorId(m_AuthorId);
    loadAuthorProfile();
    emit authorIdChanged();
}

Author* AuthorProfileItem::GetAuthor() const
{
    return m_Author.get();
}

void AuthorProfileItem::SetAuthor(Author *author)
{
    if (!author) {
        return;
    }

    if (!m_Author) {
        m_Author = AuthorPtr(new Author());
    }
    m_Author->Update(author);
    emit authorChanged();
}

AuthorBooksModel* AuthorProfileItem::GetShortBooksModel() const
{
    return m_ShortBooksModel;
}

void AuthorProfileItem::SetAuthor(const AuthorPtr& author)
{
    if (!author) {
        return;
    }

    if (!m_Author) {
        m_Author = AuthorPtr(new Author());
    }
    m_Author->Update(author);
    emit authorChanged();
}

void AuthorProfileItem::loadAuthorProfile()
{
    if (m_AuthorId.isEmpty()) {
        return;
    }
    SailreadsManager::Instance()->loadAuthorProfile(this, m_AuthorId);
}

void AuthorProfileItem::handleGotAuthorProfile(const AuthorPtr& author)
{
    if (!author || (m_AuthorId != author->GetId() && !m_AuthorId.startsWith(author->GetId()))) {
        return;
    }
    m_AuthorId = author->GetId();
    emit authorIdChanged();
    SetAuthor(author);
    CountedItems<BookPtr> books;
    books.m_Items = author->GetBooksPtr();
    books.m_BeginIndex = 1;
    books.m_EndIndex = books.m_Items.size();
    books.m_Count = books.m_EndIndex;
    m_ShortBooksModel->handleGotAuthorBooks(m_AuthorId, books);
    emit shortBooksModelChanged();
}

void AuthorProfileItem::handleAuthorFollowed(const QString& authorId, quint64 followingId)
{
    if (m_AuthorId != authorId) {
        return;
    }

    if (m_Author) {
        m_Author->SetFollowingId(followingId);
        emit authorChanged();
    }
}

void AuthorProfileItem::handleAuthorUnfollowed(const QString& authorId)
{
    if (m_AuthorId != authorId) {
        return;
    }

    if (m_Author) {
        m_Author->SetFollowingId(0);
        emit authorChanged();
    }
}

} // namespace Sailreads
