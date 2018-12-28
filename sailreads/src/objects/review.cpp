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

#include "review.h"

#include <QtDebug>

#include "book.h"

namespace Sailreads
{
Review::Review(QObject *parent)
: QObject(parent)
, m_Id(0)
, m_Rating(0)
, m_Votes(0)
, m_ReadCount(0)
, m_CommentsCount(0)
, m_Owned(0)
{
#ifdef QT_DEBUG
    qDebug() << this << "CONSTRUCTED";
#endif
}

Review::~Review()
{
#ifdef QT_DEBUG
    qDebug() << this << "DESTRUCTED";
#endif
}

quint64 Review::GetId() const
{
    return m_Id;
}

void Review::SetId(quint64 id)
{
    m_Id = id;
}

Book* Review::GetBook() const
{
    return m_Book.get();
}

BookPtr Review::GetBookPtr() const
{
    return m_Book;
}

void Review::SetBook(const BookPtr& book)
{
    m_Book = book;
}

int Review::GetRating() const
{
    return m_Rating;
}

void Review::SetRating(int rating)
{
    m_Rating = rating;
}

int Review::GetVotes() const
{
    return m_Votes;
}

void Review::SetVotes(int votes)
{
    m_Votes = votes;
}

BookShelves_t Review::GetShelves() const
{
    return m_Shelves;
}

void Review::SetShelves(const BookShelves_t& shelves)
{
    m_Shelves = shelves;
}

QStringList Review::GetShelvesList() const
{
    QStringList list;
    std::transform(m_Shelves.begin(), m_Shelves.end(),
        std::back_inserter(list),
        [](decltype(m_Shelves.front()) shelf){ return shelf.GetName(); });
    return list;
}

QDateTime Review::GetAddedDate() const
{
    return m_AddedDate;
}

void Review::SetAddedDate(const QDateTime& addedDate)
{
    m_AddedDate = addedDate;
}

QDateTime Review::GetUpdatedDate() const
{
    return m_UpdatedDate;
}

void Review::SetUpdatedDate(const QDateTime& updatedDate)
{
    m_UpdatedDate = updatedDate;
}

QDateTime Review::GetReadDate() const
{
    return m_ReadDate;
}

void Review::SetReadDate(const QDateTime& readDate)
{
    m_ReadDate = readDate;
}

QDateTime Review::GetStartedDate() const
{
    return m_StartedDate;
}

void Review::SetStartedDate(const QDateTime& startedDate)
{
    m_StartedDate = startedDate;
}

int Review::GetReadCount() const
{
    return m_ReadCount;
}

void Review::SetReadCount(int readCount)
{
    m_ReadCount = readCount;
}

QString Review::GetBody() const
{
    return m_Body;
}

void Review::SetBody(const QString& body)
{
    m_Body = body;
}

quint64 Review::GetCommentsCount() const
{
    return m_CommentsCount;
}

void Review::SetCommentsCount(const quint64& commentsCount)
{
    m_CommentsCount = commentsCount;
}

quint64 Review::GetOwned() const
{
    return m_Owned;
}

void Review::SetOwned(const quint64& owned)
{
    m_Owned = owned;
}

QUrl Review::GetUrl() const
{
    return m_Url;
}

void Review::SetUrl(const QUrl& url)
{
    m_Url = url;
}

User *Review::GetUser() const
{
    return m_User.get();
}

UserPtr Review::GetUserPtr() const
{
    return m_User;
}

void Review::SetUser(const UserPtr& user)
{
    m_User = user;
    emit userChanged();
}

CountedItems<Comment> Review::GetComments() const
{
    return m_Comments;
}

void Review::SetComments(const CountedItems<Comment>& comments)
{
    m_Comments = comments;
}

void Review::Update(Review *newReview)
{
    if (!newReview) {
        return;
    }

    SetId(newReview->GetId());
    SetBook(newReview->GetBookPtr());
    SetRating(newReview->GetRating());
    SetVotes(newReview->GetVotes());
    SetShelves(newReview->GetShelves());
    SetAddedDate(newReview->GetAddedDate());
    SetUpdatedDate(newReview->GetUpdatedDate());
    SetReadDate(newReview->GetReadDate());
    SetStartedDate(newReview->GetStartedDate());
    SetReadCount(newReview->GetReadCount());
    SetBody(newReview->GetBody());
    SetCommentsCount(newReview->GetCommentsCount());
    SetOwned(newReview->GetOwned());
    SetUrl(newReview->GetUrl());
    SetUser(newReview->GetUserPtr());
}

void Review::Update(const ReviewPtr& newReview)
{
    if (!newReview) {
        return;
    }

    Update(newReview.get());
    SetComments(newReview->GetComments());
}
} // namespace Sailreads
