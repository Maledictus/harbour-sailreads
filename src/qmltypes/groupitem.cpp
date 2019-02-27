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
#include "groupitem.h"

#include "../objects/group.h"
#include "../sailreadsmanager.h"

namespace Sailreads
{
GroupItem::GroupItem(QObject *parent)
: ItemRequestCanceler(parent)
, m_GroupId(0)
{
    connect(SailreadsManager::Instance(), &SailreadsManager::gotUserGroup,
            this, &GroupItem::handleGotGroup);
}

quint64 GroupItem::GetGroupId() const
{
    return m_GroupId;
}

void GroupItem::SetGroupId(quint64 groupId)
{
    if (m_GroupId == groupId) {
        return;
    }

    m_GroupId = groupId;
    loadGroup(true);
    emit groupIdChanged();
}

Group* GroupItem::GetGroup() const
{
    return m_Group.get();
}

void GroupItem::SetGroup(const GroupPtr& group)
{
    if (!group) {
        return;
    }

    m_Group = group;
    emit groupChanged();
}

void GroupItem::handleGotGroup(quint64 inGroupId, const GroupPtr& group)
{
    if (m_GroupId != inGroupId) {
        return;
    }
    SetGroup(group);
}

void GroupItem::loadGroup(bool useCache)
{
    if (m_GroupId <= 0) {
        return;
    }
    SailreadsManager::Instance()->loadGroup(this, m_GroupId, useCache);
}
} // namespace Sailreads
