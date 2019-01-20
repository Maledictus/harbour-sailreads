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

#include "userstatusupdate.h"

namespace Sailreads
{
UserStatusUpdate::UserStatusUpdate(QObject *parent)
: QObject(parent)
, m_Id(0)
, m_UserId("")
, m_BookId(0)
, m_Page(0)
, m_CommentsCount(0)
, m_Percent(0)
, m_WorkId(0)
, m_ReviewId("")
{
}

quint64 UserStatusUpdate::GetId() const
{
    return m_Id;
}

void UserStatusUpdate::SetId(quint64 id)
{
    if (m_Id != id) {
        m_Id = id;
        emit idChanged();
    }
}

QString UserStatusUpdate::GetUserId() const
{
    return m_UserId;
}

void UserStatusUpdate::SetUserId(const QString& id)
{
    if (m_UserId != id) {
        m_UserId = id;
        emit userIdChanged();
    }
}

quint64 UserStatusUpdate::GetBookId() const
{
    return m_BookId;
}

void UserStatusUpdate::SetBookId(quint64 id)
{
    if (m_BookId != id) {
        m_BookId = id;
        emit bookIdChanged();
    }
}

int UserStatusUpdate::GetPage() const
{
    return m_Page;
}

void UserStatusUpdate::SetPage(int page)
{
    if (m_Page != page) {
        m_Page = page;
        emit pageChanged();
    }
}

quint64 UserStatusUpdate::GetCommentsCount() const
{
    return m_CommentsCount;
}

void UserStatusUpdate::SetCommentsCount(quint64 count)
{
    if (m_CommentsCount != count) {
        m_CommentsCount = count;
        emit commentsCountChanged();
    }
}

QString UserStatusUpdate::GetBody() const
{
    return m_Body;
}

void UserStatusUpdate::SetBody(const QString& body)
{
    if (m_Body != body) {
        m_Body = body;
        emit bodyChanged();
    }
}

QDateTime UserStatusUpdate::GetCreateDate() const
{
    return m_CreateDate;
}

void UserStatusUpdate::SetCreateDate(const QDateTime& dt)
{
    if (m_CreateDate != dt) {
        m_CreateDate = dt;
        emit createDateChanged();
    }
}

QDateTime UserStatusUpdate::GetUpdateDate() const
{
    return m_UpdateDate;
}

void UserStatusUpdate::SetUpdateDate(const QDateTime& dt)
{
    if (m_UpdateDate != dt) {
        m_UpdateDate = dt;
        emit updateDateChanged();
    }
}

int UserStatusUpdate::GetPercent() const
{
    return m_Percent;
}

void UserStatusUpdate::SetPercent(int percent)
{
    if (m_Percent != percent) {
        m_Percent = percent;
        emit percentChanged();
    }
}

quint64 UserStatusUpdate::GetWorkId() const
{
    return m_WorkId;
}

void UserStatusUpdate::SetWorkId(quint64 id)
{
    if (m_WorkId != id) {
        m_WorkId = id;
        emit workIdChanged();
    }
}

QString UserStatusUpdate::GetReviewId() const
{
    return m_ReviewId;
}

void UserStatusUpdate::SetReviewId(const QString& id)
{
    if (m_ReviewId != id) {
        m_ReviewId = id;
        emit reviewIdChanged();
    }
}

Book* UserStatusUpdate::GetBook() const
{
    return m_Book.get();
}

BookPtr UserStatusUpdate::GetBookPtr() const
{
    return m_Book;
}

void UserStatusUpdate::SetBook(const BookPtr& book)
{
    if (m_Book != book) {
        m_Book = book;
        emit bookChanged();
    }
}


} // namespace Sailreads
