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

#include "groupfoldertopicsmodel.h"

#include <QtDebug>

#include "../objects/topic.h"
#include "../objects/user.h"
#include "../sailreadsmanager.h"

namespace Sailreads {

GroupFolderTopicsModel::GroupFolderTopicsModel(QObject *parent)
: BaseModel<TopicPtr>(parent)
, m_GroupId(0)
, m_GroupFolderId(0)
, m_HasMore(true)
, m_CurrentPage(1)
{
    connect(SailreadsManager::Instance(), &SailreadsManager::gotGroupFolderTopics,
            this, &GroupFolderTopicsModel::handleGotGroupFolderTopics);
    connect(SailreadsManager::Instance(), &SailreadsManager::gotGroupFolderTopic,
            this, &GroupFolderTopicsModel::handleGotGroupFolderTopic);
}

QVariant GroupFolderTopicsModel::data(const QModelIndex& index, int role) const
{
    if (index.row() > m_Items.count() - 1 || index.row() < 0) {
        return QVariant();
    }

    const auto& topic = m_Items.at(index.row());
    auto author = topic->GetAuthor();
    switch (role) {
    case Id:
        return topic->GetId();
    case Title:
        return topic->GetTitle();
    case CommentsCount:
        return topic->GetCommentsCount();
    case LastCommentDate:
        return topic->GetLastCommentDate();
    case ContextType:
        return topic->GetContextType();
    case ContextId:
        return topic->GetContextId();
    case AuthorId:
        return author ? author->GetId() : 0;
    case AuthorName:
        return author ? author->GetFirstName() : "";
    case FolderId:
        return topic->GetGroupFolder().GetId();
    case FolderName:
        return topic->GetGroupFolder().GetName();
    case UnreadCount:
        return topic->GetUnreadCount();
    default:
        return QVariant();
    }
}

QHash<int, QByteArray> GroupFolderTopicsModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[Id] = "topicId";
    roles[Title] = "topicTitle";
    roles[CommentsCount] = "topicCommentsCount";
    roles[LastCommentDate] = "topicLastCommentDate";
    roles[ContextType] = "topicContextType";
    roles[ContextId] = "topicContextId";
    roles[AuthorId] = "topicAuthorId";
    roles[AuthorName] = "topicAuthorName";
    roles[FolderId] = "topicFolderId";
    roles[FolderName] = "topicFolderName";
    roles[UnreadCount] = "topicUnreadCount";
    return roles;
}

quint64 GroupFolderTopicsModel::GetGroupId() const
{
    return m_GroupId;
}

void GroupFolderTopicsModel::SetGroupId(quint64 id)
{
    if (m_GroupId != id) {
        m_GroupId = id;
        emit groupIdChanged();
    }
}

quint64 GroupFolderTopicsModel::GetGroupFolderId() const
{
    return m_GroupFolderId;
}

void GroupFolderTopicsModel::SetGroupFolderId(quint64 id)
{
    if (m_GroupFolderId != id) {
        m_GroupFolderId = id;
        emit groupFolderIdChanged();
    }
}

bool GroupFolderTopicsModel::GetHasMore() const
{
    return m_HasMore;
}

void GroupFolderTopicsModel::SetHasMore(bool has)
{
    if (m_HasMore != has) {
        m_HasMore = has;
        emit hasMoreChanged();
    }
}

void GroupFolderTopicsModel::fetchMoreContent()
{
    SailreadsManager::Instance()->loadGroupFolderTopics(m_GroupFolderId, m_GroupId, m_CurrentPage);
}

void GroupFolderTopicsModel::handleGotGroupFolderTopics(quint64 groupdFolderId,
        quint64 groupId, const CountedItems<TopicPtr> &topics)
{
    if (m_GroupId != groupId || m_GroupFolderId != groupdFolderId) {
        return;
    }

    SetHasMore(topics.m_EndIndex != topics.m_Count);
    if (m_HasMore) {
        ++m_CurrentPage;
    }

    if (topics.m_BeginIndex == 1) {
        SetItems(topics.m_Items);
    }
    else {
        AddItems(topics.m_Items);
    }
}

void GroupFolderTopicsModel::handleGotGroupFolderTopic(const TopicPtr& topic)
{
    auto it = std::find_if(m_Items.begin(), m_Items.end(),
            [topic](decltype(m_Items.front()) item) {
                return item->GetId() == topic->GetId();
            });
    if (it != m_Items.end()) {
        (*it)->SetUnreadCount(topic->GetUnreadCount());
        const int pos = std::distance(m_Items.begin(), it);
        emit dataChanged(index(pos), index(pos), { UnreadCount });
    }
}
} // namespace Sailreads
