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

#include "basemodel.h"
#include "../types.h"

namespace Sailreads
{
class AuthorSeriesModel : public BaseModel<SeriesWorkPtr>
{
    Q_OBJECT

    quint64 m_AuthorId;
    bool m_HasMore;
    quint64 m_CurrentPage;

    Q_PROPERTY(quint64 authorId READ GetAuthorId WRITE SetAuthorId NOTIFY authorIdChanged)
    Q_PROPERTY(bool hasMore READ GetHasMore WRITE SetHasMore NOTIFY hasMoreChanged)
public:
    AuthorSeriesModel(QObject *parent = nullptr);

    enum SeriesRoles
    {
        Id = Qt::UserRole + 1
    };

    virtual QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    virtual QHash<int, QByteArray> roleNames() const override;

    quint64 GetAuthorId() const;
    void SetAuthorId(quint64 id);
    bool GetHasMore() const;
    void SetHasMore(bool has);

private slots:
    void handleGotAuthorSeries(const Series_t& series);

signals:
    void authorIdChanged();
    void hasMoreChanged();
};

} // namespace Sailreads
