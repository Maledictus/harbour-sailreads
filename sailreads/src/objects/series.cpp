/*
Copyright (c) 2018 Oleg Linkin <maledictusdemagog@gmail.com>

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

#include <QtDebug>

namespace Sailreads
{
Series::Series(QObject *parent)
: QObject(parent)
, m_Id(0)
, m_SeriesWorksCount(0)
, m_PrimaryWorkCount(0)
, m_Numbered(false)
{
    qDebug() << this << "CONSTRUCTED";
}

Series::~Series()
{
    qDebug() << this << "DESTRUCTED";
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
} // namespace Sailreads
