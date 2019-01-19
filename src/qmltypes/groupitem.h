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

#include <QObject>

#include "../itemrequestcanceler.h"
#include "../types.h"

namespace Sailreads
{
class GroupItem : public ItemRequestCanceler
{
    Q_OBJECT

    quint64 m_GroupId;
    GroupPtr m_Group;

    Q_PROPERTY(quint64 groupId READ GetGroupId WRITE SetGroupId NOTIFY groupIdChanged)
    Q_PROPERTY(Group* group READ GetGroup NOTIFY groupChanged)

public:
    explicit GroupItem(QObject *parent = nullptr);

    quint64 GetGroupId() const;
    void SetGroupId(quint64 groupId);
    Group* GetGroup() const;
    void SetGroup(const GroupPtr& group);

private slots:
    void handleGotGroup(quint64 inGroupId, const GroupPtr& group);
public slots:
    void loadGroup();

signals:
    void groupIdChanged();
    void groupChanged();
};

} // namespace Sailreads

