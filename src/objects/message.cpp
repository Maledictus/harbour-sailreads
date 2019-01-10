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

#include "message.h"

namespace Sailreads
{
Message::Message(QObject *parent)
: QObject(parent)
, m_Id(0)
, m_ParentMessageId(0)
, m_NextMessageId(0)
, m_PreviousMessageId(0)
{
}

quint64 Message::GetId() const
{
    return m_Id;
}

void Message::SetId(quint64 id)
{
    m_Id = id;
}

QDateTime Message::GetCreateDate() const
{
    return m_CreateDate;
}

void Message::SetCreateDate(const QDateTime& dt)
{
    m_CreateDate = dt;
}

QDateTime Message::GetUpdateDate() const
{
    return m_UpdateDate;
}

void Message::SetUpdateDate(const QDateTime& dt)
{
    m_UpdateDate = dt;
}

QDateTime Message::GetReadDate() const
{
    return m_ReadDate;
}

void Message::SetReadDate(const QDateTime& dt)
{
    m_ReadDate = dt;
}

QString Message::GetFolder() const
{
    return m_Folder;
}

void Message::SetFolder(const QString& folder)
{
    m_Folder = folder;
}

quint64 Message::GetParentMessageId() const
{
    return m_ParentMessageId;
}

void Message::SetParentMessageId(quint64 messageId)
{
    m_ParentMessageId = messageId;
}

QString Message::GetSubject() const
{
    return m_Subject;
}

void Message::SetSubject(const QString& subject)
{
    m_Subject = subject;
}

QString Message::GetBody() const
{
    return m_Body;
}

void Message::SetBody(const QString& body)
{
    m_Body = body;
}

UserPtr Message::GetFromUserPtr() const
{
    return m_FromUser;
}

User *Message::GetFromUser() const
{
    return m_FromUser.get();
}

void Message::SetFromUser(const UserPtr& user)
{
    m_FromUser = user;
}

UserPtr Message::GetToUserPtr() const
{
    return m_ToUser;
}

User *Message::GetToUser() const
{
    return m_ToUser.get();
}

void Message::SetToUser(const UserPtr& user)
{
    m_ToUser = user;
}

Messages_t Message::GetMessageHistory() const
{
    return m_MessageHistory;
}

void Message::SetMessageHistory(const Messages_t& messages)
{
    m_MessageHistory = messages;
}

quint64 Message::GetNextMessageId() const
{
    return m_NextMessageId;
}

void Message::SetNextMessageId(quint64 messageId)
{
    m_NextMessageId = messageId;
}

quint64 Message::GetPreviousMessageId() const
{
    return m_PreviousMessageId;
}

void Message::SetPreviousMessageId(quint64 messageId)
{
    m_PreviousMessageId = messageId;
}

} // namespace Sailreads
