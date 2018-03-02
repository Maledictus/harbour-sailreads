/*
The MIT License(MIT)

Copyright (c) 2018 Oleg Linkin <maledictusdemagog@gmail.com>

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files(the "Software"), to deal
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

#include "userprofile.h"

#include <QDataStream>
#include <QtDebug>

namespace Sailreads
{
UserProfile::UserProfile(qint64 id, const QString& name, const QUrl& webUrl)
: m_UserID(id)
, m_UserName(name)
, m_WebUrl(webUrl)
{
}

quint64 UserProfile::GetUserID() const
{
    return m_UserID;
}

void UserProfile::SetUserID(quint64 id)
{
    m_UserID = id;
}

QString UserProfile::GetUserName() const
{
    return m_UserName;
}

void UserProfile::SetUserName(const QString& name)
{
    m_UserName = name;
}

QString UserProfile::GetWebUrl() const
{
    return m_WebUrl;
}

void UserProfile::SetWebUrlUrl(const QUrl& url)
{
    m_WebUrl = url;
}
} // namespace Sailreads
