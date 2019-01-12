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

#include "friend.h"

#include <QtDebug>

namespace Sailreads
{
Friend::Friend()
: m_Id(0)
, m_IsMutualFriend(false)
{
#ifdef QT_DEBUG
    qDebug() << this << "CONSTRUCTED";
#endif
}

Friend::~Friend()
{
#ifdef QT_DEBUG
    qDebug() << this << "DESTRUCTED";
#endif
}

quint64 Friend::GetId() const
{
    return m_Id;
}

void Friend::SetId(quint64 id)
{
    m_Id = id;
}

QString Friend::GetName() const
{
    return m_Name;
}

void Friend::SetName(const QString& name)
{
    m_Name = name;
}

QUrl Friend::GetUrl() const
{
    return m_Url;
}

void Friend::SetUrl(const QUrl& url)
{
    m_Url = url;
}

QUrl Friend::GetAvatarUrl() const
{
    return m_AvatarUrl;
}

void Friend::SetAvatarUrl(const QUrl& url)
{
    m_AvatarUrl = url;
}

QUrl Friend::GetSmallAvatarUrl() const
{
    return m_SmallAvatarUrl;
}

void Friend::SetSmallAvatarUrl(const QUrl& url)
{
    m_SmallAvatarUrl = url;
}

quint64 Friend::GetFriendsCount() const
{
    return m_FriendsCount;
}

void Friend::SetFriendsCount(quint64 count)
{
    m_FriendsCount = count;
}

quint64 Friend::GetBooksCount() const
{
    return m_BooksCount;
}

void Friend::SetBooksCount(quint64 count)
{
    m_BooksCount = count;
}

QDateTime Friend::GetCreatedDate() const
{
    return m_CreatedAt;
}

void Friend::SetCreatedDate(const QDateTime& dt)
{
    m_CreatedAt = dt;
}

bool Friend::GetIsMutualFriend() const
{
    return m_IsMutualFriend;
}

void Friend::SetIsMutualFriend(bool isMutualFriend)
{
    m_IsMutualFriend = isMutualFriend;
}
} // namespace Sailreads
