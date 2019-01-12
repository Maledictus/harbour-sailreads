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

    Q_PROPERTY(quint64 id READ GetId NOTIFY idChanged)
    Q_PROPERTY(QDateTime createDate READ GetCreateDate NOTIFY createDateChanged)
    Q_PROPERTY(QDateTime updateDate READ GetUpdateDate NOTIFY updateDateChanged)
    Q_PROPERTY(QDateTime readDate READ GetReadDate NOTIFY readDateChanged)
    Q_PROPERTY(QString folder READ GetFolder NOTIFY folderChanged)
    Q_PROPERTY(quint64 parentMessageId READ GetParentMessageId NOTIFY parentMessageIdChanged)
    Q_PROPERTY(QString subject READ GetSubject NOTIFY subjectChanged)
    Q_PROPERTY(QString body READ GetBody NOTIFY bodyChanged)
    Q_PROPERTY(User* fromUser READ GetFromUser NOTIFY fromUserChanged)
    Q_PROPERTY(User* toUser READ GetToUser NOTIFY toUserChanged)
    Q_PROPERTY(QObjectList messageHistory READ GetMessageHistory NOTIFY messageHistoryChanged)
    Q_PROPERTY(quint64 nextMessageId READ GetNextMessageId NOTIFY nextMessageIdChanged)
    Q_PROPERTY(quint64 previousMessageId READ GetPreviousMessageId NOTIFY previousMessageIdChanged)
    Q_PROPERTY(bool isRead READ GetIsRead NOTIFY isReadChanged)

public:
    Message(QObject *parent = nullptr);
    ~Message();

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
    Messages_t GetMessageHistoryPtr() const;
    QObjectList GetMessageHistory() const;
    void SetMessageHistory(const Messages_t& messages);
    quint64 GetNextMessageId() const;
    void SetNextMessageId(quint64 messageId);
    quint64 GetPreviousMessageId() const;
    void SetPreviousMessageId(quint64 messageId);
    bool GetIsRead() const;

    void Update(Message *newMessage);
    void Update(const MessagePtr& newMessage);


signals:
    void idChanged();
    void createDateChanged();
    void updateDateChanged();
    void readDateChanged();
    void folderChanged();
    void parentMessageIdChanged();
    void subjectChanged();
    void bodyChanged();
    void fromUserChanged();
    void toUserChanged();
    void messageHistoryChanged();
    void nextMessageIdChanged();
    void previousMessageIdChanged();
    void isReadChanged();
};

} // namespace Sailreads
