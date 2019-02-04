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

#include "recommendation.h"

#include <QtDebug>

#include "book.h"
#include "user.h"

namespace Sailreads
{
Recommendation::Recommendation(QObject *parent)
: QObject(parent)
, m_LikesCount(0)
, m_RatingId(0)
{
#ifdef QT_DEBUG
    qDebug() << this << "CONSTRUCTED";
#endif
}

Recommendation::~Recommendation()
{
#ifdef QT_DEBUG
    qDebug() << this << "DESTRUCTED";
#endif
}

QString Recommendation::GetId() const
{
    return m_Id;
}

void Recommendation::SetId(const QString& id)
{
    if (m_Id != id) {
        m_Id = id;
        emit idChanged();
    }
}

QDateTime Recommendation::GetCreateDate() const
{
    return m_CreateDate;
}

void Recommendation::SetCreateDate(const QDateTime& dt)
{
    if (m_CreateDate != dt) {
        m_CreateDate = dt;
        emit createDateChanged();
    }
}

Book* Recommendation::GetBook() const
{
    return m_Book.get();
}

BookPtr Recommendation::GetBookPtr() const
{
    return m_Book;
}

void Recommendation::SetBook(const BookPtr& book)
{
    if (m_Book != book) {
        m_Book = book;
        emit bookChanged();
    }
}

User* Recommendation::GetFromUser() const
{
    return m_FromUser.get();
}

UserPtr Recommendation::GetFromUserPtr() const
{
    return m_FromUser;
}

void Recommendation::SetFromUser(const UserPtr& user)
{
    if (m_FromUser != user) {
        m_FromUser = user;
        emit fromUserChanged();
    }
}

User* Recommendation::GetToUser() const
{
    return m_ToUser.get();
}

UserPtr Recommendation::GetToUserPtr() const
{
    return m_ToUser;
}

void Recommendation::SetToUser(const UserPtr& user)
{
    if (m_ToUser != user) {
        m_ToUser = user;
        emit toUserChanged();
    }
}

quint64 Recommendation::GetLikesCount() const
{
    return m_LikesCount;
}

void Recommendation::SetLikesCount(quint64 count)
{
    if (m_LikesCount != count) {
        m_LikesCount = count;
        if (m_LikesCount < 0) {
            m_LikesCount = 0;
        }
        emit likesCountChanged();
    }
}

bool Recommendation::GetIsLiked() const
{
    return m_RatingId > 0;
}

quint64 Recommendation::GetCommentsCount() const
{
    return m_Comments.m_Count;
}

quint64 Recommendation::GetRatingId() const
{
    return m_RatingId;
}

void Recommendation::SetRatingId(quint64 ratingId)
{
    if (m_RatingId != ratingId) {
        m_RatingId = ratingId;
        emit ratingIdChanged();
        emit isLikedChanged();
    }
}

CountedItems<Comment> Recommendation::GetComments() const
{
    return m_Comments;
}

void Recommendation::SetComments(const CountedItems<Comment>& comments)
{
    m_Comments = comments;
    emit commentsCountChanged();
}

} // namespace Sailreads
