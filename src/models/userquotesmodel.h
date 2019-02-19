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
class UserQuotesModel : public QuotesBaseModel
{
    Q_OBJECT

    QString m_UserId;
    Q_PROPERTY(QString userId READ GetUserId WRITE SetUserId NOTIFY userIdChanged)

public:
    UserQuotesModel(QObject *parent = nullptr);

    virtual void classBegin() override;
    virtual void componentComplete() override;

    QString GetUserId() const;
    void SetUserId(const QString& userId);

public slots:
    virtual void fetchMoreContent() override;
    void loadUserQuotes();
private slots:
    void handleGotUserQuotes(const QString& userId, const Quotes_t& quotes);

signals:
    void userIdChanged();

};
} // namespace Sailreads
