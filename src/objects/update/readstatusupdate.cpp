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

#include "readstatusupdate.h"

namespace Sailreads
{
ReadStatusUpdate::ReadStatusUpdate(QObject *parent)
: QObject(parent)
, m_Id(0)
, m_ReviewId(0)
, m_UserId(0)
{
}

quint64 ReadStatusUpdate::GetId() const
{
    return m_Id;
}

void ReadStatusUpdate::SetId(quint64 id)
{
    if (m_Id != id) {
        m_Id = id;
        emit idChanged();
    }
}

quint64 ReadStatusUpdate::GetReviewId() const
{
    return m_ReviewId;
}

void ReadStatusUpdate::SetReviewId(quint64 id)
{
    if (m_ReviewId != id) {
        m_ReviewId = id;
        emit reviewIdChanged();
    }
}

quint64 ReadStatusUpdate::GetUserId() const
{
    return m_UserId;
}

void ReadStatusUpdate::SetUserId(quint64 id)
{
    if (m_UserId != id) {
        m_UserId = id;
        emit userIdChanged();
    }
}

QString ReadStatusUpdate::GetOldStatus() const
{
    return m_OldStatus;
}

void ReadStatusUpdate::SetOldStatus(const QString& status)
{
    if (m_OldStatus != status) {
        m_OldStatus = status;
        emit oldStatusChanged();
    }
}

QString ReadStatusUpdate::GetCurrentStatus() const
{
    return m_CurrentStatus;
}

void ReadStatusUpdate::SetCurrentStatus(const QString& status)
{
    if (m_CurrentStatus != status) {
        m_CurrentStatus = status;
        emit currentStatusChanged();
    }
}

QDateTime ReadStatusUpdate::GetUpdateDate() const
{
    return m_UpdatedDate;
}

void ReadStatusUpdate::SetUpdateDate(const QDateTime& dt)
{
    if (m_UpdatedDate != dt) {
        m_UpdatedDate = dt;
        emit updateDateChanged();
    }
}

Book* ReadStatusUpdate::GetBook() const
{
    return m_Book.get();
}

BookPtr ReadStatusUpdate::GetBookPtr() const
{
    return m_Book;
}

void ReadStatusUpdate::SetBook(const BookPtr& book)
{
    if (m_Book != book) {
        m_Book = book;
        emit bookChanged();
    }
}

} // namespace Sailreads
