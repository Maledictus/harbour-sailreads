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
#include "quotesbasemodel.h"

namespace Sailreads
{
QuotesBaseModel::QuotesBaseModel(QObject *parent)
: BaseModel<Quote>(parent)
, m_HasMore(true)
, m_CurrentPage(1)
{
}

QVariant QuotesBaseModel::data(const QModelIndex& index, int role) const
{
    if (index.row() > m_Items.count() - 1 || index.row() < 0) {
        return QVariant();
    }

    const auto& quoteItem = m_Items.at(index.row());
    switch (role) {
    case Body:
        return quoteItem.GetQuote();
    case Author:
        return quoteItem.GetAuthor();
    case AuthorLink:
        return quoteItem.GetAuthorLink();
    case Book:
        return quoteItem.GetBook();
    case BookLink:
        return quoteItem.GetBookLink();
    default:
        return QVariant();
    }
}

QHash<int, QByteArray> QuotesBaseModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[Body] = "quoteBody";
    roles[Author] = "quoteAuthor";
    roles[AuthorLink] = "quoteAuthorLink";
    roles[Book] = "quoteBook";
    roles[BookLink] = "quoteBookLink";
    return roles;
}

bool QuotesBaseModel::GetHasMore() const
{
    return m_HasMore;
}

void QuotesBaseModel::SetHasMore(bool has)
{
    if (m_HasMore != has) {
        m_HasMore = has;
        emit hasMoreChanged();
    }
}
} // namespace Sailreads
