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

#include "group.h"

namespace Sailreads
{
Group::Group()
: m_Id(0)
{
}

quint64 Group::GetId() const
{
    return m_Id;
}

void Group::SetId(quint64 id)
{
    m_Id = id;
}

QString Group::GetName() const
{
    return m_Name;
}

void Group::SetName(const QString& name)
{
    m_Name = name;
}

bool Group::GetIsPublic() const
{
    return m_IsPublic;
}

void Group::SetIsPublic(bool isPublic)
{
    m_IsPublic = isPublic;
}

quint64 Group::GetUsersCount() const
{
    return m_UsersCount;
}

void Group::SetUsersCount(quint64 count)
{
    m_UsersCount = count;
}

QUrl Group::GetImageUrl() const
{
    return m_ImageUrl;
}

void Group::SetImageUrl(const QUrl& url)
{
    m_ImageUrl = url;
}

QDateTime Group::GetLastActivity() const
{
    return m_LastActivity;
}

void Group::SetLastActivity(const QDateTime& dt)
{
    m_LastActivity = dt;
}

QUrl Group::GetUrl() const
{
    return m_Url;
}

void Group::SetUrl(const QUrl& url)
{
    m_Url = url;
}

} // namespace Sailreads
