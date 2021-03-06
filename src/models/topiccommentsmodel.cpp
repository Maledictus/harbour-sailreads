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
#include "topiccommentsmodel.h"

#include "../objects/topic.h"
#include "../sailreadsmanager.h"

namespace Sailreads
{
TopicCommentsModel::TopicCommentsModel(QObject *parent)
: BaseCommentsModel(parent)
, m_TopicId("")
{
}

void TopicCommentsModel::classBegin()
{
    auto sm = SailreadsManager::Instance();
    connect(sm, &SailreadsManager::gotGroupFolderTopic,
            this, &TopicCommentsModel::handleGotGroupFolderTopic);
    connect(sm, &SailreadsManager::newCommentAdded,
            this, &TopicCommentsModel::handleNewCommentAdded);
}

void TopicCommentsModel::componentComplete()
{
}

QString TopicCommentsModel::GetTopicId() const
{
    return m_TopicId;
}

void TopicCommentsModel::SetTopicId(const QString& id)
{
    if (m_TopicId != id) {
        m_TopicId = id;
        topicIdChanged();
    }
}

void TopicCommentsModel::fetchMoreContent()
{
    SailreadsManager::Instance()->loadGroupFolderTopic(this, m_TopicId, m_CurrentPage);
    SetFetching(true);
}

void TopicCommentsModel::handleGotGroupFolderTopic(const TopicPtr& topic)
{
    if (!topic || topic->GetId() != m_TopicId) {
        return;
    }
    handleGotComments(topic->GetComments());
    SetFetching(false);
}

void TopicCommentsModel::handleNewCommentAdded(const QString& resourceId, const Comment& comment)
{
    if (resourceId != m_TopicId) {
        return;
    }
    if (!m_HasMore) {
        AddItems(comment);
    }
}

} // namespace Sailreads
