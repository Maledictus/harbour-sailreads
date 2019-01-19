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

#include "seriesitem.h"

#include "../objects/series.h"
#include "../sailreadsmanager.h"

namespace Sailreads
{
SeriesItem::SeriesItem(QObject *parent)
: ItemRequestCanceler(parent)
, m_SeriesId(0)
{
    connect(SailreadsManager::Instance(), &SailreadsManager::gotSeries,
            this, &SeriesItem::handleGotSeries);
}

quint64 SeriesItem::GetSeriesId() const
{
    return m_SeriesId;
}

void SeriesItem::SetSeriesId(quint64 seriesId)
{
    if (m_SeriesId == seriesId) {
        return;
    }

    m_SeriesId = seriesId;
    updateSeries();
    emit seriesIdChanged();
}

Series* SeriesItem::GetSeries() const
{
    return m_Series.get();
}

void SeriesItem::SetSeries(const SeriesPtr& series)
{
    if (!series) {
        return;
    }

    m_Series = series;
    emit seriesChanged();
}

void SeriesItem::handleGotSeries(const SeriesPtr& series)
{
    if (!series || m_SeriesId != series->GetId()) {
        return;
    }
    SetSeries(series);
}

void SeriesItem::updateSeries()
{
    SailreadsManager::Instance()->loadSeries(this, m_SeriesId);
}
} // namespace Sailreads
