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

#include "groupmember.h"

#include <QtDebug>

#include "user.h"

namespace Sailreads
{
GroupMember::GroupMember()
{
#ifdef QT_DEBUG
    qDebug() << this << "CONSTRUCTED";
#endif
}

GroupMember::~GroupMember()
{
#ifdef QT_DEBUG
    qDebug() << this << "DESTRUCTED";
#endif
}

UserPtr GroupMember::GetUser() const
{
    return m_User;
}

void GroupMember::SetUser(const UserPtr& user)
{
    m_User = user;
}

QString GroupMember::GetTitle() const
{
    return m_Title;
}

void GroupMember::SetTitle(const QString& title)
{
    m_Title = title;
}

quint64 GroupMember::GetCommentsCount() const
{
    return m_CommentsCount;
}

void GroupMember::SetCommentsCount(quint64 count)
{
    m_CommentsCount = count;
}
} // namespace Sailreads
