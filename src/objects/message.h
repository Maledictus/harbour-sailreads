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
#include <QObject>
#include <QString>

#include "../types.h"

namespace Sailreads
{
class Message : public QObject
{
    Q_OBJECT

    quint64 m_Id;
    QDateTime m_CreateDate;
    QDateTime m_UpdateDate;
    QDateTime m_ReadDate;
    QString m_Folder;
    quint64 m_ParentMessageId;
    QString m_Subject;
    QString m_Body;
    UserPtr m_FromUser;
    UserPtr m_ToUser;
    Messages_t m_MessageHistory;
    quint64 m_NextMessageId;
    quint64 m_PreviousMessageId;

public:
    Message(QObject *parent = nullptr);

    quint64 GetId() const;
    void SetId(quint64 id);
    QDateTime GetCreateDate() const;
    void SetCreateDate(const QDateTime& dt);
    QDateTime GetUpdateDate() const;
    void SetUpdateDate(const QDateTime& dt);
    QDateTime GetReadDate() const;
    void SetReadDate(const QDateTime& dt);
    QString GetFolder() const;
    void SetFolder(const QString& folder);
    quint64 GetParentMessageId() const;
    void SetParentMessageId(quint64 messageId);
    QString GetSubject() const;
    void SetSubject(const QString& subject);
    QString GetBody() const;
    void SetBody(const QString& body);
    UserPtr GetFromUserPtr() const;
    User *GetFromUser() const;
    void SetFromUser(const UserPtr& user);
    UserPtr GetToUserPtr() const;
    User* GetToUser() const;
    void SetToUser(const UserPtr& user);
    Messages_t GetMessageHistory() const;
    void SetMessageHistory(const Messages_t& messages);
    quint64 GetNextMessageId() const;
    void SetNextMessageId(quint64 messageId);
    quint64 GetPreviousMessageId() const;
    void SetPreviousMessageId(quint64 messageId);
};

} // namespace Sailreads
