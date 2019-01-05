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

#include "series.h"

#include <numeric>

#include <QtDebug>

#include "serieswork.h"
#include "work.h"

namespace Sailreads
{
Series::Series(QObject *parent)
: QObject(parent)
, m_Id(0)
, m_SeriesWorksCount(0)
, m_PrimaryWorkCount(0)
, m_Numbered(false)
, m_RatingsCount(0)
, m_AverageRating(0.0)
{
#ifdef QT_DEBUG
    qDebug() << this << "CONSTRUCTED";
#endif
}

Series::~Series()
{
#ifdef QT_DEBUG
    qDebug() << this << "DESTRUCTED";
#endif
}

quint64 Series::GetId() const
{
    return m_Id;
}

void Series::SetId(quint64 id)
{
    m_Id = id;
}

QString Series::GetTitle() const
{
    return m_Title;
}

void Series::SetTitle(const QString& title)
{
    m_Title = title;
}

QString Series::GetDescription() const
{
    return m_Description;
}

void Series::SetDescription(const QString& description)
{
    m_Description = description;
}

QString Series::GetNote() const
{
    return m_Note;
}

void Series::SetNote(const QString& note)
{
    m_Note = note;
}

int Series::GetSeriesWorksCount() const
{
    return m_SeriesWorksCount;
}

void Series::SetSeriesWorksCount(int seriesWorksCount)
{
    m_SeriesWorksCount = seriesWorksCount;
}

int Series::GetPrimaryWorkCount() const
{
    return m_PrimaryWorkCount;
}

void Series::SetPrimaryWorkCount(int primaryWorkCount)
{
    m_PrimaryWorkCount = primaryWorkCount;
}

bool Series::GetNumbered() const
{
    return m_Numbered;
}

void Series::SetNumbered(bool numbered)
{
    m_Numbered = numbered;
}

QObjectList Series::GetSeriesWorks() const
{
    QObjectList objList;
    for (const auto& seriesWork : m_SeriesWorks) {
        objList << seriesWork.get();
    }
    return objList;
}

void Series::SetSeriesWorks(const SeriesWorks_t& seriesWorks)
{
    m_SeriesWorks = seriesWorks;
    m_RatingsCount = std::accumulate (m_SeriesWorks.begin(), m_SeriesWorks.end(),
            0, [](int i, decltype(m_SeriesWorks.front()) sw)
            { return i + sw->GetWorkPtr()->GetRatingsCount(); });
    const quint64 ratingSum = std::accumulate (m_SeriesWorks.begin(), m_SeriesWorks.end(),
            0, [](int i, decltype(m_SeriesWorks.front()) sw)
            { return i + sw->GetWorkPtr()->GetRatingSum(); });
    m_AverageRating = m_RatingsCount > 0 ?
            static_cast<qreal>(ratingSum) / m_RatingsCount :
            0.0;
}

qreal Series::GetAverageRating() const
{
    return m_AverageRating;
}

quint64 Series::GetRatingsCount() const
{
    return m_RatingsCount;
}
} // namespace Sailreads
