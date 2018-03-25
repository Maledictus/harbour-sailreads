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

#pragma once

#include <QDateTime>
#include <QString>

#include "groupfolder.h"
#include "../types.h"

namespace Sailreads
{
class Topic
{
    quint64 m_Id;
    QString m_Title;
    quint64 m_CommentsCount;
    QDateTime m_LastCommentDate;
    QString m_ContextType;
    quint64 m_ContextId;
    User m_Author;
    GroupFolder m_GroupFolder;

public:
    Topic();

    quint64 GetId() const;
    void SetId(quint64 id);
    QString GetTitle() const;
    void SetTitle(const QString& title);
    quint64 GetCommentsCount() const;
    void SetCommentsCount(quint64 count);
    QDateTime GetLastCommentDate() const;
    void SetLastCommentDate(const QDateTime& dt);
    QString GetContextType() const;
    void SetContextType(const QString& ctxType);
    quint64 GetContextId() const;
    void SetContextId(quint64 id);
    User GetAuthor() const;
    void SetAuthor(const User& user);
    GroupFolder GetGroupFolder() const;
    void SetGroupFolder(const GroupFolder& folder);
};
typedef QList<Topic> Topics_t;
} // namespace Sailreads
