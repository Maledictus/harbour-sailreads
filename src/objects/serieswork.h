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

#include <QList>
#include <QObject>

#include "../types.h"

namespace Sailreads
{
class SeriesWork: public QObject
{
    Q_OBJECT

    quint64 m_Id;
    int m_Position;
    WorkPtr m_Work;
    SeriesPtr m_Series;

    Q_PROPERTY(quint64 id READ GetId NOTIFY idChanged)
    Q_PROPERTY(int position READ GetPosition NOTIFY positionChanged)
    Q_PROPERTY(Work* work READ GetWork NOTIFY workChanged)
    Q_PROPERTY(Series* series READ GetSeries NOTIFY seriesChanged)

public:
    SeriesWork(QObject *parent = nullptr);
    ~SeriesWork();

    quint64 GetId() const;
    void SetId(quint64 id);
    int GetPosition() const;
    void SetPosition(int position);
    Work* GetWork() const;
    WorkPtr GetWorkPtr() const;
    void SetWork(const WorkPtr& work);
    Series* GetSeries() const;
    SeriesPtr GetSeriesPtr() const;
    void SetSeries(const SeriesPtr& series);

signals:
    void idChanged();
    void positionChanged();
    void workChanged();
    void seriesChanged();
};
} // namespace Sailreads
