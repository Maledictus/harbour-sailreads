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

#include "serieswork.h"

#include <QtDebug>

namespace Sailreads
{
SeriesWork::SeriesWork(QObject *parent)
: QObject(parent)
, m_Id(0)
, m_Position(0)
{
#ifdef QT_DEBUG
    qDebug() << this << "CONSTRUCTED";
#endif
}

SeriesWork::~SeriesWork()
{
#ifdef QT_DEBUG
    qDebug() << this << "DESTRUCTED";
#endif
}

quint64 SeriesWork::GetId() const
{
    return m_Id;
}

void SeriesWork::SetId(quint64 id)
{
    m_Id = id;
}

int SeriesWork::GetPosition() const
{
    return m_Position;
}

void SeriesWork::SetPosition(int position)
{
    m_Position = position;
}

Work *SeriesWork::GetWork() const
{
    return m_Work.get();
}

void SeriesWork::SetWork(const WorkPtr& work)
{
    m_Work = work;
}
} // namespace Sailreads
