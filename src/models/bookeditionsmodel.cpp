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

#include "bookeditionsmodel.h"

#include "../sailreadsmanager.h"

namespace Sailreads
{
BookEditionsModel::BookEditionsModel(QObject *parent)
: BaseBooksModel(parent)
, m_WorkId(0)
{
}

void BookEditionsModel::classBegin()
{
    auto sm = SailreadsManager::Instance();
    connect(sm, &SailreadsManager::gotBookEditions,
            this, &BookEditionsModel::handleGotBookEditions);
}

void BookEditionsModel::componentComplete()
{
}

quint64 BookEditionsModel::GetWorkId() const
{
    return m_WorkId;
}

void BookEditionsModel::SetWorkId(quint64 workId)
{
    if (m_WorkId == workId) {
        return;
    }

    m_WorkId = workId;
    m_CurrentPage = 1;
    emit workIdChanged();
}

void BookEditionsModel::fetchMoreContent()
{
    SailreadsManager::Instance()->loadBookEditions(this, m_WorkId, m_CurrentPage);
}

void BookEditionsModel::loadBookEditions()
{
    if (m_WorkId <= 0) {
        return;
    }
    Clear();
    m_CurrentPage = 1;
    SailreadsManager::Instance()->loadBookEditions(this, m_WorkId, m_CurrentPage, false);
}

void BookEditionsModel::handleGotBookEditions(quint64 workId, const CountedItems<BookPtr>& books)
{
    if (m_WorkId != workId) {
        return;
    }

    AddItems(books.m_Items);
    SetHasMore(books.m_Count);
    if (m_HasMore) {
        ++m_CurrentPage;
    }
}

} // namespace Sailreads
