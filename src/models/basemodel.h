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

#include <QAbstractListModel>

#include "../modelrequestcanceler.h"

namespace Sailreads
{
template<typename T>
class BaseModel : public ModelRequestCanceler
{
protected:
    QList<T> m_Items;

public:
    BaseModel(QObject *parent = 0)
    : ModelRequestCanceler(parent)
    {}

    virtual int rowCount(const QModelIndex& = QModelIndex()) const override
    {
        return m_Items.count();
    }

    void PrependItems(const T& item)
    {
        beginInsertRows(QModelIndex(), 0, 0);
        m_Items.insert(0, item);
        endInsertRows();
    }

    void AddItems(const T& item)
    {
        beginInsertRows(QModelIndex(), rowCount(), rowCount());
        m_Items.append(item);
        endInsertRows();
    }

    void AddItems(const QList<T>& items)
    {
        beginInsertRows(QModelIndex(), rowCount(), rowCount() + items.count() - 1);
        m_Items.append(items);
        endInsertRows();
    }

    void SetItems(const QList<T>& items)
    {
        beginResetModel();
        m_Items.clear();
        m_Items.append(items);
        endResetModel();
    }

    QList<T> GetItems() const
    {
        return m_Items;
    }

    void Clear()
    {
        beginResetModel();
        m_Items.clear();
        endResetModel();
    }
};
}
