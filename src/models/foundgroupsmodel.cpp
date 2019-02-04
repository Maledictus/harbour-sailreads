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

#include "foundgroupsmodel.h"

#include "../sailreadsmanager.h"

namespace Sailreads
{
FoundGroupsModel::FoundGroupsModel(QObject *parent)
: GroupsModel(parent)
, m_HasMore(true)
, m_CurrentPage(1)
{
    auto sm = SailreadsManager::Instance();
    connect(sm, &SailreadsManager::gotFoundGroups,
            this, &FoundGroupsModel::handleGotFoundGroups);
}

bool FoundGroupsModel::GetHasMore() const
{
    return m_HasMore;
}

void FoundGroupsModel::SetHasMore(bool has)
{
    if (m_HasMore != has) {
        m_HasMore = has;
        emit hasMoreChanged();
    }
}

void FoundGroupsModel::fetchMoreContent(const QString& text)
{
    if (text.isEmpty()) {
        return;
    }
    if (m_LastSearch != text) {
        m_CurrentPage = 1;
        m_HasMore = true;
        SetItems({});
    }
    m_LastSearch = text;

    SailreadsManager::Instance()->searchGroup(this, text, m_CurrentPage);
}

void FoundGroupsModel::handleGotFoundGroups(const CountedItems<GroupPtr>& groups)
{
    if (!groups.m_BeginIndex && !groups.m_EndIndex)
    {
        Clear();
    }
    else if (groups.m_BeginIndex == 1) {
        m_CurrentPage = 1;
        SetItems(groups.m_Items);
    }
    else {
        AddItems(groups.m_Items);
    }

    SetHasMore(groups.m_EndIndex != groups.m_Count);
    if (m_HasMore) {
        ++m_CurrentPage;
    }
}

} // namespace Sailreads
