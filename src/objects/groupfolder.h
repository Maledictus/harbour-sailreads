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

#include <QObject>
#include <QList>
#include <QDateTime>

namespace Sailreads
{
class GroupFolder
{
    Q_GADGET

    QString m_Id;
    QString m_Name;
    quint64 m_ItemsCount;
    quint64 m_SubItemsCount;
    QDateTime m_UpdatedAt;

    Q_PROPERTY(QString id READ GetId)
    Q_PROPERTY(QString name READ GetName)
    Q_PROPERTY(quint64 itemsCount READ GetItemsCount)
    Q_PROPERTY(quint64 subItemssCount READ GetSubItemsCount)
    Q_PROPERTY(QDateTime updatedAt READ GetUpdatedAt)
public:
    GroupFolder();

    QString GetId() const;
    void SetId(const QString& id);
    QString GetName() const;
    void SetName(const QString& name);
    quint64 GetItemsCount() const;
    void SetItemsCount(quint64 count);
    quint64 GetSubItemsCount() const;
    void SetSubItemsCount(quint64 count);
    QDateTime GetUpdatedAt() const;
    void SetUpdatedAt(const QDateTime& dt);
};

typedef QList<GroupFolder> GroupFolders_t;
} // namespace Sailreads
