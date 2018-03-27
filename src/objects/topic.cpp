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

#include "topic.h"

namespace Sailreads
{
Topic::Topic()
: m_Id(0)
, m_CommentsCount(0)
, m_ContextId(0)
, m_NewCommentsCount(0)
, m_CommentsPerPage(0)
{
}

quint64 Topic::GetId() const
{
    return m_Id;
}

void Topic::SetId(quint64 id)
{
    m_Id = id;
}

QString Topic::GetTitle() const
{
    return m_Title;
}

void Topic::SetTitle(const QString& title)
{
    m_Title = title;
}

quint64 Topic::GetCommentsCount() const
{
    return m_CommentsCount;
}

void Topic::SetCommentsCount(quint64 count)
{
    m_CommentsCount = count;
}

QDateTime Topic::GetLastCommentDate() const
{
    return m_LastCommentDate;
}

void Topic::SetLastCommentDate(const QDateTime& dt)
{
    m_LastCommentDate = dt;
}

QString Topic::GetContextType() const
{
    return m_ContextType;
}

void Topic::SetContextType(const QString& ctxType)
{
    m_ContextType = ctxType;
}

quint64 Topic::GetContextId() const
{
    return m_ContextId;
}

void Topic::SetContextId(quint64 id)
{
    m_ContextId = id;
}

User Topic::GetAuthor() const
{
    return m_Author;
}

void Topic::SetAuthor(const User& user)
{
    m_Author = user;
}

GroupFolder Topic::GetGroupFolder() const
{
    return m_GroupFolder;
}

void Topic::SetGroupFolder(const GroupFolder& folder)
{
    m_GroupFolder = folder;
}

quint64 Topic::GetNewCommentsCount() const
{
    return m_NewCommentsCount;
}

void Topic::SetNewCommentsCount(quint64 count)
{
    m_NewCommentsCount = count;
}

quint64 Topic::GetCommentsPerPage() const
{
    return m_CommentsPerPage;
}

void Topic::SetCommentsPerPage(quint64 count)
{
    m_CommentsPerPage = count;
}

QDateTime Topic::GetUpdateAt() const
{
    return m_UpdatedAt;
}

void Topic::SetUpdatedAt(const QDateTime& dt)
{
    m_UpdatedAt = dt;
}

QString Topic::GetSubjectType() const
{
    return m_SubjectType;
}

void Topic::SetSubjectType(const QString& type)
{
    m_SubjectType = type;
}

Group Topic::GetGroup() const
{
    return m_Group;
}

void Topic::SetGroup(const Group& group)
{
    m_Group = group;
}

CountedItems<Comment> Topic::GetComments() const
{
    return m_Comments;
}

void Topic::SetComments(const CountedItems<Comment>& comments)
{
    m_Comments = comments;
}
} // namespace Sailreads
