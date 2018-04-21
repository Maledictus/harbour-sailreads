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

#pragma once

#include <QString>
#include <QUrl>

namespace Sailreads
{
struct FavoriteAuthor
{
    quint64 m_Id;
    QString m_Name;
    bool operator ==(const FavoriteAuthor& author)
    {
        return m_Id == author.m_Id && m_Name == author.m_Name;
    }
};
typedef QList<FavoriteAuthor> FavoriteAuthors_t;

template<typename T>
struct CountedItems
{
    quint64 m_BeginIndex;
    quint64 m_EndIndex;
    quint64 m_Count;
    QList<T> m_Items;

    CountedItems()
    : m_BeginIndex(0)
    , m_EndIndex(0)
    , m_Count(0)
    {}
};
}
