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

#include "friendrequest.h"

#include <QtDebug>

namespace Sailreads
{
FriendRequest::FriendRequest()
: m_Id(0)
{
#ifdef QT_DEBUG
    qDebug() << this << "CONSTRUCTED";
#endif
}

FriendRequest::~FriendRequest()
{
#ifdef QT_DEBUG
    qDebug() << this << "DESTRUCTED";
#endif
}

quint64 FriendRequest::GetId() const
{
    return m_Id;
}

void FriendRequest::SetId(quint64 id)
{
    m_Id = id;
}

QDateTime FriendRequest::GetCreatedAt() const
{
    return m_CreatedAt;
}

void FriendRequest::SetCreatedAt(const QDateTime& createdAt)
{
    m_CreatedAt = createdAt;
}

QString FriendRequest::GetMessage() const
{
    return m_Message;
}

void FriendRequest::SetMessage(const QString& message)
{
    m_Message = message;
}

UserPtr FriendRequest::GetFromUser() const
{
    return m_FromUser;
}

void FriendRequest::SetFromUser(const UserPtr& fromUser)
{
    m_FromUser = fromUser;
}


} // namespace Sailreads
