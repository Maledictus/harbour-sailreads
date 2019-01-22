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
#include "userstatus.h"

#include <QtDebug>

#include "book.h"

namespace Sailreads
{
UserStatus::UserStatus(QObject *parent)
: QObject(parent)
, m_LikesCount(0)
, m_CommentsCount(0)
, m_IsLiked(false)
, m_Page(0)
, m_Percent(0)
, m_WorkId(0)
{
#ifdef QT_DEBUG
    qDebug() << this << "CONSTRUCTED";
#endif
}

UserStatus::~UserStatus()
{
#ifdef QT_DEBUG
    qDebug() << this << "DESTRUCTED";
#endif
}

QString UserStatus::GetId() const
{
    return m_Id;
}

void UserStatus::SetId(const QString& id)
{
    if (m_Id != id) {
        m_Id = id;
        emit idChanged();
    }
}

QString UserStatus::GetHeader() const
{
    return m_Header;
}

void UserStatus::SetHeader(const QString& header)
{
    if (m_Header != header) {
        m_Header = header;
        emit headerChanged();
    }
}

QString UserStatus::GetBody() const
{
    return m_Body;
}

void UserStatus::SetBody(const QString& body)
{
    if (m_Body != body) {
        m_Body = body;
        emit bodyChanged();
    }
}

QDateTime UserStatus::GetCreateDate() const
{
    return m_CreateDate;
}

void UserStatus::SetCreateDate(const QDateTime& dt)
{
    if (m_CreateDate != dt) {
        m_CreateDate = dt;
        emit createDateChanged();
    }
}

QDateTime UserStatus::GetUpdateDate() const
{
    return m_UpdateDate;
}

void UserStatus::SetUpdateDate(const QDateTime& dt)
{
    if (m_UpdateDate != dt) {
        m_UpdateDate = dt;
        emit updateDateChanged();
    }
}

quint64 UserStatus::GetLikesCount() const
{
    return m_LikesCount;
}

void UserStatus::SetLikesCount(quint64 count)
{
    if (m_LikesCount != count) {
        m_LikesCount = count;
        emit likesCountChanged();
    }
}

quint64 UserStatus::GetCommentsCount() const
{
    return m_CommentsCount;
}

void UserStatus::SetCommentsCount(quint64 count)
{
    if (m_CommentsCount != count) {
        m_CommentsCount = count;
        emit commentsCountChanged();
    }
}

bool UserStatus::GetIsLiked() const
{
    return m_IsLiked;
}

void UserStatus::SetIsLiked(bool isLiked)
{
    if (m_IsLiked != isLiked) {
        m_IsLiked = isLiked;
        emit isLikedChanged();
    }
}

int UserStatus::GetPage() const
{
    return m_Page;
}

void UserStatus::SetPage(int page)
{
    if (m_Page != page) {
        m_Page = page;
        emit pageChanged();
    }
}

int UserStatus::GetPercent() const
{
    return m_Percent;
}

void UserStatus::SetPercent(int percent)
{
    if (m_Percent != percent) {
        m_Percent = percent;
        emit percentChanged();
    }
}

quint64 UserStatus::GetWorkId() const
{
    return m_WorkId;
}

void UserStatus::SetWorkId(quint64 workId)
{
    if (m_WorkId != workId) {
        m_WorkId = workId;
        emit workIdChanged();
    }
}

User* UserStatus::GetUser() const
{
    return m_User.get();
}

UserPtr UserStatus::GetUserPtr() const
{
    return m_User;
}

void UserStatus::SetUser(const UserPtr& user)
{
    m_User = user;
    emit userChanged();
}

Book* UserStatus::GetBook() const
{
    return m_Book.get();
}

BookPtr UserStatus::GetBookPtr() const
{
    return m_Book;
}

void UserStatus::SetBook(const BookPtr& book)
{
    m_Book = book;
    emit bookChanged();
}

CountedItems<Comment> UserStatus::GetComments() const
{
    return m_Comments;
}

void UserStatus::SetComments(const CountedItems<Comment>& comments)
{
    m_Comments = comments;
}

UserStatuses_t UserStatus::GetPreviousStatuses() const
{
    return m_PreviousStatuses;
}

void UserStatus::SetPreviousStatuses(const UserStatuses_t& userStatuses)
{
    m_PreviousStatuses = userStatuses;
}

} // namespace Sailreads
