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

#include "groupsmodel.h"
#include "../types.h"

namespace Sailreads
{
class FoundGroupsModel : public GroupsModel
{
    Q_OBJECT

    bool m_HasMore;
    quint64 m_CurrentPage;
    QString m_LastSearch;

    Q_PROPERTY(bool hasMore READ GetHasMore WRITE SetHasMore NOTIFY hasMoreChanged)

public:
    explicit FoundGroupsModel(QObject *parent = nullptr);

    bool GetHasMore() const;
    void SetHasMore(bool has);

public slots:
    void fetchMoreContent(const QString& text);
private slots:
    void handleGotFoundGroups(const CountedItems<GroupPtr>& groups);

signals:
    void hasMoreChanged();
};

} // namespace Sailreads


