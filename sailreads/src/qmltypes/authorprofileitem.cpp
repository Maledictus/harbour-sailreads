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

#include "authorprofileitem.h"

#include "../objects/author.h"
#include "../sailreadsmanager.h"

namespace Sailreads
{
AuthorProfileItem::AuthorProfileItem(QObject *parent)
: QObject(parent)
, m_AuthorId(0)
{
    connect(SailreadsManager::Instance(), &SailreadsManager::gotAuthorProfile,
            this, &AuthorProfileItem::handleGotAuthorProfile);
    connect(SailreadsManager::Instance(), &SailreadsManager::authorFollowed,
            this, &AuthorProfileItem::handleAuthorFollowed);
    connect(SailreadsManager::Instance(), &SailreadsManager::authorUnfollowed,
            this, &AuthorProfileItem::handleAuthorUnfollowed);
}

quint64 AuthorProfileItem::GetAuthorId() const
{
    return m_AuthorId;
}

void AuthorProfileItem::SetAuthorId(quint64 authorId)
{
    if (m_AuthorId == authorId) {
        return;
    }

    m_AuthorId = authorId;
    updateAuthorProfile();
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

void AuthorProfileItem::updateAuthorProfile()
{
    if (m_AuthorId <= 0) {
        return;
    }
    SailreadsManager::Instance()->loadAuthorProfile(m_AuthorId);
}

void AuthorProfileItem::handleGotAuthorProfile(const AuthorPtr& author)
{
    if (!author || m_AuthorId != author->GetId()) {
        return;
    }
    SetAuthor(author);
}

void AuthorProfileItem::handleAuthorFollowed(quint64 authorId, quint64 followingId)
{
    if (m_AuthorId != authorId) {
        return;
    }

    if (m_Author) {
        m_Author->SetFollowingId(followingId);
        emit authorChanged();
    }
}

void AuthorProfileItem::handleAuthorUnfollowed(quint64 authorId)
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
