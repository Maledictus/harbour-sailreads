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
#include "commentsmodel.h"

#include <QtDebug>

#include "../sailreadsmanager.h"

namespace Sailreads
{
CommentsModel::CommentsModel(QObject *parent)
: BaseCommentsModel(parent)
{
}

void CommentsModel::classBegin()
{
    auto sm = SailreadsManager::Instance();
    connect(sm, &SailreadsManager::gotComments,
            this, &CommentsModel::handleGotComments);
    connect(sm, &SailreadsManager::newCommentAdded,
            this, &CommentsModel::handleNewCommentAdded);
}

void CommentsModel::componentComplete()
{
}

QString CommentsModel::GetResourceId() const
{
    return m_ResourceId;
}

void CommentsModel::SetResourceId(const QString& id)
{
    if (m_ResourceId != id) {
        m_ResourceId = id;
        resourceIdChanged();
    }
}

QString CommentsModel::GetResourceType() const
{
    return m_ResourceType;
}

void CommentsModel::SetResourceType(const QString& type)
{
    if (m_ResourceType != type) {
        m_ResourceType = type;
        resourceTypeChanged();
    }
}

void CommentsModel::handleGotComments(const QString& resourceId,
        const CountedItems<Comment>& comments)
{
    if (m_ResourceId != resourceId) {
        return;
    }

    BaseCommentsModel::handleGotComments(comments);
}

void CommentsModel::handleNewCommentAdded(const QString& resourceId, const Comment& comment)
{
    if (m_ResourceId != resourceId) {
        return;
    }

    if (!m_HasMore) {
        PrependItems(comment);
    }
}

void CommentsModel::fetchMoreContent()
{
    if (m_ResourceId.isEmpty() || m_ResourceType.isEmpty()) {
        return;
    }
    SailreadsManager::Instance()->loadComments(this, m_ResourceId, m_ResourceType, m_CurrentPage);
}

void CommentsModel::loadComments()
{
    if (m_ResourceId.isEmpty() || m_ResourceType.isEmpty()) {
        return;
    }
    SailreadsManager::Instance()->loadComments(this, m_ResourceId, m_ResourceType);
}

} // namespace Sailreads
