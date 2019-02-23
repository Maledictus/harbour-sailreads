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

#include "quotesbasemodel.h"

namespace Sailreads
{
class BookQuotesModel : public QuotesBaseModel
{
    Q_OBJECT

    quint64 m_WorkId;
    Q_PROPERTY(quint64 workId READ GetWorkId WRITE SetWorkId NOTIFY workIdChanged)

public:
    BookQuotesModel(QObject *parent = nullptr);

    virtual void classBegin() override;
    virtual void componentComplete() override;

    quint64 GetWorkId() const;
    void SetWorkId(quint64 workId);

public slots:
    virtual void fetchMoreContent() override;
    void loadBookQuotes();
private slots:
    void handleGotBookQuotes(quint64 workId, const PageCountedItems<Quote>& quotes);

signals:
    void workIdChanged();
};
} // namespace Sailreads
