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
{
}

quint64 ReadStatusUpdate::GetId() const
{

}

void ReadStatusUpdate::SetId(quint64 id)
{

}

quint64 ReadStatusUpdate::GetReviewId() const
{

}

void ReadStatusUpdate::SetReviewId(quint64 id)
{

}

quint64 ReadStatusUpdate::GetUserId() const
{

}

void ReadStatusUpdate::SetUserId(quint64 id)
{

}

QString ReadStatusUpdate::GetOldStatus() const
{

}

void ReadStatusUpdate::SetOldStatus(const QString &status)
{

}

QString ReadStatusUpdate::GetCurrentStatus() const
{

}

void ReadStatusUpdate::SetCurrentStatus(const QString &status)
{

}

QDateTime ReadStatusUpdate::GetUpdateDate() const
{

}

void ReadStatusUpdate::SetUpdateDate(const QDateTime &dt)
{

}

Book *ReadStatusUpdate::GetBook() const
{

}

BookPtr ReadStatusUpdate::GetBookPtr() const
{

}

void ReadStatusUpdate::SetBook(const BookPtr &book)
{

}

} // namespace Sailreads
