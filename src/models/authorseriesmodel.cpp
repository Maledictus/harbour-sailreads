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

#include "authorseriesmodel.h"

#include "../sailreadsmanager.h"
#include "../objects/series.h"

namespace Sailreads
{
AuthorSeriesModel::AuthorSeriesModel(QObject *parent)
: BaseModel<SeriesPtr>(parent)
, m_AuthorId("")
, m_HasMore(true)
, m_CurrentPage(1)
{
    auto sm = SailreadsManager::Instance();
    connect(sm, &SailreadsManager::gotAuthorSeries,
            this, &AuthorSeriesModel::handleGotAuthorSeries);
}

QVariant AuthorSeriesModel::data(const QModelIndex &index, int role) const
{
    if (index.row() > m_Items.count() - 1 || index.row() < 0) {
        return QVariant();
    }

    const auto& series = m_Items.at(index.row());
    switch (role) {
    case Id:
        return series->GetId();
    case Title:
        return series->GetTitle();
    case PrimaryWorksCount:
        return series->GetPrimaryWorkCount();
    case AverageRating:
        return series->GetAverageRating();
    case RatingsCount:
        return series->GetRatingsCount();
    default:
        return QVariant();
    }
}

QHash<int, QByteArray> AuthorSeriesModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[Id] = "seriesId";
    roles[Title] = "seriesTitle";
    roles[PrimaryWorksCount] = "seriesWorksCount";
    roles[AverageRating] = "seriesAverageRating";
    roles[RatingsCount] = "seriesRatingsCount";
    return roles;
}

QString AuthorSeriesModel::GetAuthorId() const
{
    return m_AuthorId;
}

void AuthorSeriesModel::SetAuthorId(const QString& id)
{
    if (m_AuthorId != id) {
        m_AuthorId = id;
        loadAuthorSeries(true);
        emit authorIdChanged();
    }
}

bool AuthorSeriesModel::GetHasMore() const
{
    return m_HasMore;
}

void AuthorSeriesModel::SetHasMore(bool has)
{
    if (m_HasMore != has) {
        m_HasMore = has;
        emit hasMoreChanged();
    }
}

void AuthorSeriesModel::handleGotAuthorSeries(const QString& authorId, const Series_t& series)
{
    if (m_AuthorId != authorId) {
        return;
    }

    SetItems(series);
}

void AuthorSeriesModel::loadAuthorSeries(bool useCache)
{
    if (m_AuthorId.isEmpty()) {
        return;
    }
    SailreadsManager::Instance()->loadAuthorSeries(this, m_AuthorId, useCache);
}
} // namespace Sailreads
