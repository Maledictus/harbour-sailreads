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

#include "messageitem.h"

#include "../objects/message.h"
#include "../sailreadsmanager.h"

namespace Sailreads
{
MessageItem::MessageItem(QObject *parent)
: ItemRequestCanceler(parent)
, m_MessageId(0)
{
    connect(SailreadsManager::Instance(), &SailreadsManager::gotMessage,
            this, &MessageItem::handleGotMessage);
}

quint64 MessageItem::GetMessageId() const
{
    return m_MessageId;
}

void MessageItem::SetMessageId(quint64 messageId)
{
    if (m_MessageId == messageId) {
        return;
    }

    m_MessageId = messageId;
    loadMessage(true);
    emit messageIdChanged();
}

Message* MessageItem::GetMessage() const
{
    return m_Message.get();
}

void MessageItem::SetMessage(Message *message)
{
    if (!message) {
        return;
    }

    if (!m_Message) {
        m_Message = MessagePtr(new Message());
    }
    m_Message->Update(message);
    emit messageChanged();
}

void MessageItem::SetMessage(const MessagePtr& message)
{
    if (!message) {
        return;
    }

    if (!m_Message) {
        m_Message = MessagePtr(new Message());
    }
    m_Message->Update(message);
    emit messageChanged();
}

void MessageItem::handleGotMessage(const MessagePtr& message)
{
    if (!message || m_MessageId != message->GetId()) {
        return;
    }
    SetMessage(message);
}

void MessageItem::loadMessage(bool useCache)
{
    if (m_MessageId <= 0) {
        return;
    }
    SailreadsManager::Instance()->loadMessage(this, m_MessageId, useCache);
}
} // namespace Sailreads
