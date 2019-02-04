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
#include "readstatus.h"

#include <QtDebug>

#include "book.h"

namespace Sailreads
{
ReadStatus::ReadStatus(QObject *parent)
: QObject(parent)
, m_LikesCount(0)
, m_CommentsCount(0)
, m_IsLiked(false)
, m_RatingId(0)
, m_ReviewId(0)
{
#ifdef QT_DEBUG
    qDebug() << this << "CONSTRUCTED";
#endif
}

ReadStatus::~ReadStatus()
{
#ifdef QT_DEBUG
    qDebug() << this << "DESTRUCTED";
#endif
}

QString ReadStatus::GetId() const
{
    return m_Id;
}

void ReadStatus::SetId(const QString& id)
{
    if (m_Id != id) {
        m_Id = id;
        emit idChanged();
    }
}

QString ReadStatus::GetHeader() const
{
    return m_Header;
}

void ReadStatus::SetHeader(const QString& header)
{
    if (m_Header != header) {
        m_Header = header;
        emit headerChanged();
    }
}

quint64 ReadStatus::GetLikesCount() const
{
    return m_LikesCount;
}

void ReadStatus::SetLikesCount(quint64 count)
{
    if (m_LikesCount != count) {
        m_LikesCount = count;
        emit likesCountChanged();
    }
}

quint64 ReadStatus::GetCommentsCount() const
{
    return m_CommentsCount;
}

void ReadStatus::SetCommentsCount(quint64 count)
{
    if (m_CommentsCount != count) {
        m_CommentsCount = count;
        emit commentsCountChanged();
    }
}

bool ReadStatus::GetIsLiked() const
{
    return m_RatingId > 0;
}

void ReadStatus::SetIsLiked(bool isLiked)
{
}

void ReadStatus::SetRatingId(quint64 ratingId)
{
    if (m_RatingId != ratingId) {
        m_RatingId = ratingId;
        emit ratingIdChanged();
        emit isLikedChanged();
    }
}

quint64 ReadStatus::GetRatingId() const
{
    return m_RatingId;
}

QDateTime ReadStatus::GetCreateDate() const
{
    return m_CreateDate;
}

void ReadStatus::SetCreateDate(const QDateTime& dt)
{
    if (m_CreateDate != dt) {
        m_CreateDate = dt;
        emit createDateChanged();
    }
}

QDateTime ReadStatus::GetUpdateDate() const
{
    return m_UpdateDate;
}

void ReadStatus::SetUpdateDate(const QDateTime& dt)
{
    if (m_UpdateDate != dt) {
        m_UpdateDate = dt;
        emit updateDateChanged();
    }
}

QString ReadStatus::GetStatus() const
{
    return m_Status;
}

void ReadStatus::SetStatus(const QString& status)
{
    if (m_Status != status) {
        m_Status = status;
        emit statusChanged();
    }
}

quint64 ReadStatus::GetReviewId() const
{
    return m_ReviewId;
}

void ReadStatus::SetReviewId(quint64 reviewId)
{
    if (m_ReviewId != reviewId) {
        m_ReviewId = reviewId;
        emit reviewIdChanged();
    }
}

Book* ReadStatus::GetBook() const
{
    return m_Book.get();
}

BookPtr ReadStatus::GetBookPtr() const
{
    return m_Book;
}

void ReadStatus::SetBook(const BookPtr& book)
{
    m_Book = book;
    emit bookChanged();
}

User* ReadStatus::GetUser() const
{
    return m_User.get();
}

UserPtr ReadStatus::GetUserPtr() const
{
    return m_User;
}

void ReadStatus::SetUser(const UserPtr& user)
{
    m_User = user;
    emit userChanged();
}

CountedItems<Comment> ReadStatus::GetComments() const
{
    return m_Comments;
}

void ReadStatus::SetComments(const CountedItems<Comment>& comments)
{
    m_Comments = comments;
}

} // namespace Sailreads
