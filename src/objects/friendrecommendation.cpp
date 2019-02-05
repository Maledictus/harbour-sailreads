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

#include "friendrecommendation.h"

#include <QtDebug>

namespace Sailreads
{
FriendRecommendation::FriendRecommendation()
: m_Id(0)
{
#ifdef QT_DEBUG
    qDebug() << this << "CONSTRUCTED";
#endif
}

FriendRecommendation::~FriendRecommendation()
{
#ifdef QT_DEBUG
    qDebug() << this << "DESTRUCTED";
#endif
}

quint64 FriendRecommendation::GetId() const
{
    return m_Id;
}

void FriendRecommendation::SetId(quint64 id)
{
    m_Id = id;
}

QDateTime FriendRecommendation::GetCreateDate() const
{
    return m_CreateDate;
}

void FriendRecommendation::SetCreateDate(const QDateTime& dt)
{
    m_CreateDate = dt;
}

UserPtr FriendRecommendation::GetRecommenderUser() const
{
    return m_RecommenderUser;
}

void FriendRecommendation::SetRecommenderUser(const UserPtr& user)
{
    m_RecommenderUser = user;
}

UserPtr FriendRecommendation::GetRecommendedUser() const
{
    return m_RecommendedUser;
}

void FriendRecommendation::SetRecommendedUser(const UserPtr& user)
{
    m_RecommendedUser = user;
}

} // namespace Sailreads
