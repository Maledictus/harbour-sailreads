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

#include <QQmlParserStatus>

#include "basemodel.h"
#include "../types.h"

namespace Sailreads
{
class UpdatesModel : public BaseModel<UpdatePtr>
                    , public QQmlParserStatus
{
    Q_OBJECT

protected:
    bool m_HasMore;
    QString m_UpdateScope;
    QString m_UpdateItems;

    Q_PROPERTY(bool hasMore READ GetHasMore WRITE SetHasMore NOTIFY hasMoreChanged)
    Q_PROPERTY(QString updateScope READ GetUpdateScope WRITE SetUpdateScope NOTIFY updateScopeChanged)
    Q_PROPERTY(QString updateItems READ GetUpdateItems WRITE SetUpdateItems NOTIFY updateItemsChanged)
public:
    enum UpdateRoles
    {
        HeaderText = Qt::UserRole + 1,
        Link,
        ImageUrl,
        Actor,
        UpdatedDate,
        UpdateType,
        UpdateObject
    };

    UpdatesModel(QObject *parent = nullptr);

    bool GetHasMore() const;
    void SetHasMore(bool has);
    QString GetUpdateScope() const;
    void SetUpdateScope(const QString& scope);
    QString GetUpdateItems() const;
    void SetUpdateItems(const QString& items);

    virtual QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    virtual QHash<int, QByteArray> roleNames() const override;

    virtual void classBegin();
    virtual void componentComplete();

private slots:
    void handleGotUpdates(const Updates_t& updates);
public slots:
    void fetchMoreContent();
    void loadUpdates();

signals:
    void hasMoreChanged();
    void updateScopeChanged();
    void updateItemsChanged();
};
} // namespace Sailreads

