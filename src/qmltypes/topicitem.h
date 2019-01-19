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
class TopicItem : public ItemRequestCanceler
{
    Q_OBJECT

    quint64 m_TopicId;
    TopicPtr m_Topic;

    Q_PROPERTY(quint64 topicId READ GetTopicId WRITE SetTopicId NOTIFY topicIdChanged)
    Q_PROPERTY(Topic* topic READ GetTopic NOTIFY topicChanged)
public:
    TopicItem(QObject *parent = nullptr);

    quint64 GetTopicId() const;
    void SetTopicId(quint64 topicId);
    Topic* GetTopic() const;
    void SetTopic(const TopicPtr& topic);

private slots:
    void handleGotTopic(const TopicPtr& topic);
public slots:
    void updateTopic();

signals:
    void topicIdChanged();
    void topicChanged();
};

} // namespace Sailreads
