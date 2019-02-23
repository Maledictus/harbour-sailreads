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

#include "bookquotesmodel.h"

#include "../sailreadsmanager.h"

namespace Sailreads
{
BookQuotesModel::BookQuotesModel(QObject *parent)
: QuotesBaseModel(parent)
, m_WorkId(0)
{
}

void BookQuotesModel::classBegin()
{
    auto sm = SailreadsManager::Instance();
    connect(sm, &SailreadsManager::gotBookQuotes,
            this, &BookQuotesModel::handleGotBookQuotes);
}

void BookQuotesModel::componentComplete()
{
}

quint64 BookQuotesModel::GetWorkId() const
{
    return m_WorkId;
}

void BookQuotesModel::SetWorkId(quint64 workId)
{
    if (m_WorkId != workId) {
        m_WorkId = workId;
        m_CurrentPage = 1;
        SetHasMore(true);
        emit workIdChanged();
    }
}

void BookQuotesModel::fetchMoreContent()
{
    if (!m_WorkId) {
        return;
    }
    SailreadsManager::Instance()->loadBookQuotes(this, m_WorkId, m_CurrentPage);
    SetFetching(true);
}

void BookQuotesModel::loadBookQuotes()
{
    if (!m_WorkId) {
        return;
    }
    Clear();
    m_HasMore = true;
    m_CurrentPage = 1;
    SailreadsManager::Instance()->loadBookQuotes(this, m_WorkId);
    SetFetching(true);
}

void BookQuotesModel::handleGotBookQuotes(quint64 workId, const PageCountedItems<Quote>& quotes)
{
    if (m_WorkId != workId || !quotes.m_Items.count()) {
        return;
    }

    if (!quotes.m_Page && !quotes.m_PagesCount)
    {
        Clear();
    }
    else if (quotes.m_Page == 1) {
        m_CurrentPage = quotes.m_Page;
        SetItems(quotes.m_Items);
    }
    else {
        AddItems(quotes.m_Items);
    }

    SetHasMore(quotes.m_Page != quotes.m_PagesCount);
    if (m_HasMore) {
        ++m_CurrentPage;
    }
    SetFetching(false);
}

} // namespace Sailreads
