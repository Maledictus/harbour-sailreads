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

#include "groupfolder.h"

namespace Sailreads
{
GroupFolder::GroupFolder()
: m_ItemsCount(0)
, m_SubItemsCount(0)
{
}

QString GroupFolder::GetId() const
{
    return m_Id;
}

void GroupFolder::SetId(const QString& id)
{
    m_Id = id;
}

QString GroupFolder::GetName() const
{
    return m_Name;
}

void GroupFolder::SetName(const QString& name)
{
    m_Name = name;
}

quint64 GroupFolder::GetItemsCount() const
{
    return m_ItemsCount;
}

void GroupFolder::SetItemsCount(quint64 count)
{
    m_ItemsCount = count;
}

quint64 GroupFolder::GetSubItemsCount() const
{
    return m_SubItemsCount;
}

void GroupFolder::SetSubItemsCount(quint64 count)
{
    m_SubItemsCount = count;
}

QDateTime GroupFolder::GetUpdatedAt() const
{
    return m_UpdatedAt;
}

void GroupFolder::SetUpdatedAt(const QDateTime& dt)
{
    m_UpdatedAt = dt;
}
} // namespace Sailreads
