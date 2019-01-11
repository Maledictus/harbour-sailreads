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

#include "messagesmodel.h"

#include "../sailreadsmanager.h"
#include "../objects/message.h"
#include "../objects/user.h"

namespace Sailreads
{
MessagesModel::MessagesModel(QObject *parent)
: BaseModel<MessagePtr>(parent)
, m_HasMore(true)
, m_CurrentPage(1)
{
    connect(SailreadsManager::Instance(), &SailreadsManager::gotMessages,
            this, &MessagesModel::handleGotMessages);
    connect(SailreadsManager::Instance(), &SailreadsManager::gotMessage,
            this, &MessagesModel::handleGotMessage);
}

QString MessagesModel::GetFolder() const
{
    return m_Folder;
}

void MessagesModel::SetFolder(const QString& folder)
{
    if (m_Folder != folder) {
        m_Folder = folder;
        m_CurrentPage = 1;
        SetHasMore(true);
        fetchMoreContent();
        emit folderChanged();
    }
}

bool MessagesModel::GetHasMore() const
{
    return m_HasMore;
}

void MessagesModel::SetHasMore(bool has)
{
    if (m_HasMore != has) {
        m_HasMore = has;
        emit hasMoreChanged();
    }
}

QVariant MessagesModel::data(const QModelIndex &index, int role) const
{
    if (index.row() > m_Items.count() - 1 || index.row() < 0) {
        return QVariant();
    }

    const auto& message = m_Items.at(index.row());
    switch (role) {
    case Id:
        return message->GetId();
    case CreateDate:
        return message->GetCreateDate();
    case UpdateDate:
        return message->GetUpdateDate();
    case ReadDate:
        return message->GetReadDate();
    case Folder:
        return message->GetFolder();
    case ParentMessageId:
        return message->GetParentMessageId();
    case Subject:
        return message->GetSubject();
    case Body:
        return message->GetBody();
    case FromUser:
        return QVariant::fromValue(message->GetFromUser());
    case ToUser:
        return QVariant::fromValue(message->GetToUser());
    case NextMessageId:
        return message->GetNextMessageId();
    case PreviousMessageId:
        return message->GetPreviousMessageId();
    case IsRead:
        return !message->GetReadDate().isNull();
    case Message:
        return QVariant::fromValue(message.get());
    default:
        return QVariant();
    }
}

QHash<int, QByteArray> MessagesModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[Id] = "messageId";
    roles[CreateDate] = "messageCreateDate";
    roles[UpdateDate] = "messageUpdateDate";
    roles[ReadDate] = "messageReadDate";
    roles[Folder] = "messageFolder";
    roles[ParentMessageId] = "messageParentMessageId";
    roles[Subject] = "messageSubject";
    roles[Body] = "messageBody";
    roles[FromUser] = "messageFromUser";
    roles[ToUser] = "messageToUser";
    roles[NextMessageId] = "messageNextMessageId";
    roles[PreviousMessageId] = "messagePreviousMessageId";
    roles[IsRead] = "messageRead";
    roles[Message] = "messageMessage";
    return roles;
}


void MessagesModel::fetchMoreContent()
{
    if (m_Folder.isEmpty()) {
        return;
    }

    SailreadsManager::Instance()->loadMessages(m_Folder, m_CurrentPage);
}

void MessagesModel::handleGotMessages(const QString& folder, const CountedItems<MessagePtr>& messages)
{
    if (m_Folder != folder) {
        return;
    }

    if (!messages.m_BeginIndex && !messages.m_EndIndex) {
        SetItems(messages.m_Items);
    }
    else if (messages.m_BeginIndex == 1) {
        m_CurrentPage = 1;
        SetItems(messages.m_Items);
    }
    else {
        AddItems(messages.m_Items);
    }

    SetHasMore(messages.m_EndIndex != messages.m_Count);
    if (m_HasMore) {
        ++m_CurrentPage;
    }
}

void MessagesModel::handleGotMessage(const MessagePtr& message)
{
    auto it = std::find_if(m_Items.begin(), m_Items.end(),
            [message](decltype(m_Items.front()) item) {
                return item->GetId() == message->GetId();
            });
    if (it != m_Items.end()) {
        (*it)->SetUpdateDate(message->GetUpdateDate());
        (*it)->SetReadDate(message->GetReadDate());
        (*it)->SetNextMessageId(message->GetNextMessageId());
        (*it)->SetPreviousMessageId(message->GetPreviousMessageId());
        const int pos = std::distance(m_Items.begin(), it);
        emit dataChanged(index(pos), index(pos),
                { UpdateDate, ReadDate, NextMessageId, PreviousMessageId, IsRead, Message });
    }
}
} // namespace Sailreads
