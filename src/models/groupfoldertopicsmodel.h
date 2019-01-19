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

#include "basemodel.h"
#include "../types.h"

namespace Sailreads
{
class GroupFolderTopicsModel : public BaseModel<TopicPtr>
{
    Q_OBJECT
    Q_ENUMS(TopicRoles)

    quint64 m_GroupId;
    QString m_GroupFolderId;
    bool m_HasMore;
    quint64 m_CurrentPage;

    Q_PROPERTY(quint64 groupId READ GetGroupId WRITE SetGroupId NOTIFY groupIdChanged)
    Q_PROPERTY(QString groupFolderId READ GetGroupFolderId WRITE SetGroupFolderId NOTIFY groupFolderIdChanged)
    Q_PROPERTY(bool hasMore READ GetHasMore WRITE SetHasMore NOTIFY hasMoreChanged)
public:
    enum TopicRoles
    {
        Id = Qt::UserRole + 1,
        Title,
        CommentsCount,
        LastCommentDate,
        ContextType,
        ContextId,
        AuthorId,
        AuthorName,
        FolderId,
        FolderName,
        UnreadCount
    };

    GroupFolderTopicsModel(QObject *parent = nullptr);

    virtual QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    virtual QHash<int, QByteArray> roleNames() const override;

    quint64 GetGroupId() const;
    void SetGroupId(quint64 id);
    QString GetGroupFolderId() const;
    void SetGroupFolderId(const QString& id);
    bool GetHasMore() const;
    void SetHasMore(bool has);

private slots:
    void handleGotGroupFolderTopics(const QString& groupdFolderId, quint64 groupId,
            const CountedItems<TopicPtr>& topics);
    void handleGotGroupFolderTopic(const TopicPtr& topic);
public slots:
    void fetchMoreContent();

signals:
    void groupIdChanged();
    void groupFolderIdChanged();
    void hasMoreChanged();
};

} // namespace Sailreads
