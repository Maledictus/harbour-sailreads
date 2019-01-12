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
class MessagesModel : public BaseModel<MessagePtr>
{
    Q_OBJECT

    QString m_Folder;
    bool m_HasMore;
    quint64 m_CurrentPage;

    Q_PROPERTY(QString folder READ GetFolder WRITE SetFolder NOTIFY folderChanged)
    Q_PROPERTY(bool hasMore READ GetHasMore WRITE SetHasMore NOTIFY hasMoreChanged)

public:
    enum MessageRoles
    {
        Id = Qt::UserRole + 1,
        CreateDate,
        UpdateDate,
        ReadDate,
        Folder,
        ParentMessageId,
        Subject,
        Body,
        FromUser,
        ToUser,
        NextMessageId,
        PreviousMessageId,
        IsRead,
        Message
    };

    MessagesModel(QObject *parent = nullptr);

    QString GetFolder() const;
    void SetFolder(const QString& folder);
    bool GetHasMore() const;
    void SetHasMore(bool has);

    virtual QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    virtual QHash<int, QByteArray> roleNames() const override;

public slots:
    void fetchMoreContent();

private slots:
    void handleGotMessages(const QString& folder, const CountedItems<MessagePtr>& messages);
    void handleGotMessage(const MessagePtr& message);

signals:
    void folderChanged();
    void hasMoreChanged();
};

} // namespace Sailreads
