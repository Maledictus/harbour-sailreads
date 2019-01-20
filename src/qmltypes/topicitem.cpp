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

#include "topicitem.h"

#include "../objects/topic.h"
#include "../sailreadsmanager.h"

namespace Sailreads
{
TopicItem::TopicItem(QObject *parent)
: ItemRequestCanceler(parent)
, m_TopicId("")
{
    connect(SailreadsManager::Instance(), &SailreadsManager::gotGroupFolderTopic,
            this, &TopicItem::handleGotTopic);
}

QString TopicItem::GetTopicId() const
{
    return m_TopicId;
}

void TopicItem::SetTopicId(const QString& topicId)
{
    if (m_TopicId == topicId) {
        return;
    }

    m_TopicId = topicId;
    loadTopic();
    emit topicIdChanged();
}

Topic *TopicItem::GetTopic() const
{
    return m_Topic.get();
}

void TopicItem::SetTopic(const TopicPtr& topic)
{
    if (!topic) {
        return;
    }
    m_Topic = topic;
    emit topicChanged();
}

void TopicItem::handleGotTopic(const TopicPtr& topic)
{
    if (!topic || (m_TopicId != topic->GetId() && !m_TopicId.startsWith(topic->GetId()))) {
        return;
    }
    m_TopicId = topic->GetId();
    emit topicIdChanged();
    SetTopic(topic);
}

void TopicItem::loadTopic()
{
    if (m_TopicId.isEmpty()) {
        return;
    }
    SailreadsManager::Instance()->loadGroupFolderTopic(this, m_TopicId);
}
} // namespace Sailreads
