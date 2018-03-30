/*
Copyright (c) 2018 Oleg Linkin <maledictusdemagog@gmail.com>

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

#include "user.h"

namespace Sailreads
{
User::User()
: m_Id(0)
{
}

quint64 User::GetId() const
{
    return m_Id;
}

void User::SetId(quint64 id)
{
    m_Id = id;
}

QString User::GetFirstName() const
{
    return m_FirstName;
}

void User::SetFirstName(const QString& name)
{
    m_FirstName = name;
}

QString User::GetLastName() const
{
    return m_LastName;
}

void User::SetLastName(const QString& name)
{
    m_LastName = name;
}

QString User::GetNickName() const
{
    return m_NickName;
}

void User::SetNickName(const QString& name)
{
    m_NickName = name;
}

QUrl User::GetAvatar() const
{
    return m_Avatar;
}

void User::SetAvatar(const QUrl& url)
{
    m_Avatar = url;
}

} // namespace Sailreads
