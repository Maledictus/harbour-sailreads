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

#include "userquotesmodel.h"

#include "../sailreadsmanager.h"

namespace Sailreads
{
UserQuotesModel::UserQuotesModel(QObject *parent)
: QuotesBaseModel(parent)
{
}

void UserQuotesModel::classBegin()
{
    auto sm = SailreadsManager::Instance();
    connect(sm, &SailreadsManager::gotUserQuotes,
            this, &UserQuotesModel::handleGotUserQuotes);
}

void UserQuotesModel::componentComplete()
{
}

QString UserQuotesModel::GetUserId() const
{
    return m_UserId;
}

void UserQuotesModel::SetUserId(const QString& userId)
{
    if (m_UserId != userId) {
        m_UserId = userId;
        m_CurrentPage = 1;
        emit userIdChanged();
    }
}

void UserQuotesModel::fetchMoreContent()
{
    if (m_UserId.isEmpty()) {
        return;
    }
    SailreadsManager::Instance()->loadUserQuotes(this, m_UserId, m_CurrentPage);
    SetFetching(true);
}

void UserQuotesModel::loadUserQuotes()
{
    if (m_UserId.isEmpty()) {
        return;
    }
    Clear();
    m_HasMore = true;
    m_CurrentPage = 1;
    SailreadsManager::Instance()->loadUserQuotes(this, m_UserId);
    SetFetching(true);
}

void UserQuotesModel::handleGotUserQuotes(const QString& userId, const Quotes_t& quotes)
{
    if (m_UserId != userId || quotes.isEmpty())
    {
        return;
    }

    AddItems(quotes);
    static const int quotesCountInAnswer = 30;
    SetHasMore(quotes.count() == quotesCountInAnswer);
    if (m_HasMore) {
        ++m_CurrentPage;
    }
    SetFetching(false);
}

} // namespace Sailreads
