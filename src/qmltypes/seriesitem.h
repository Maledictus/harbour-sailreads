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

#include "../types.h"

namespace Sailreads
{
class SeriesItem : public QObject
{
    Q_OBJECT

    quint64 m_SeriesId;
    SeriesPtr m_Series;

    Q_PROPERTY(quint64 seriesId READ GetSeriesId WRITE SetSeriesId NOTIFY seriesIdChanged)
    Q_PROPERTY(Series* series READ GetSeries NOTIFY seriesChanged)
public:
    explicit SeriesItem(QObject *parent = nullptr);

    quint64 GetSeriesId() const;
    void SetSeriesId(quint64 seriesId);
    Series* GetSeries() const;
    void SetSeries(const SeriesPtr& series);

private slots:
    void handleGotSeries(const SeriesPtr& series);
public slots:
    void updateSeries();
    void cancelRequest();

signals:
    void seriesIdChanged();
    void seriesChanged();
};

} // namespace Sailreads

