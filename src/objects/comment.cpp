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

#include "comment.h"

#include <QtDebug>

#include "user.h"

namespace Sailreads
{
Comment::Comment()
: m_Id(0)
, m_CanDelete(false)
{
#ifdef QT_DEBUG
    qDebug() << this << "CONSTRUCTED";
#endif
}

Comment::~Comment()
{
#ifdef QT_DEBUG
    qDebug() << this << "DESTRUCTED";
#endif
}

quint64 Comment::GetId() const
{
    return m_Id;
}

void Comment::SetId(quint64 id)
{
    m_Id = id;
}

QString Comment::GetBody() const
{
    return m_Body;
}

void Comment::SetBody(const QString& body)
{
    m_Body = body;
}

QDateTime Comment::GetUpdateAtDate() const
{
    return m_UpdatedAt;
}

void Comment::SetUpdateAtDate(const QDateTime& dt)
{
    m_UpdatedAt = dt;
}

User* Comment::GetAuthor() const
{
    return m_Author.get();
}

void Comment::SetAuthor(const UserPtr& user)
{
    m_Author = user;
}

bool Comment::GetCanDelete() const
{
    return m_CanDelete;
}

void Comment::SetCanDelete(bool can)
{
    m_CanDelete = can;
}

} // namespace Sailreads
