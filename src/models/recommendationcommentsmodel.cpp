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
#include "recommendationcommentsmodel.h"

#include "../objects/recommendation.h"
#include "../sailreadsmanager.h"

namespace Sailreads
{
RecommendationCommentsModel::RecommendationCommentsModel(QObject *parent)
: BaseCommentsModel(parent)
, m_RecommendationId(0)
{
}

void RecommendationCommentsModel::classBegin()
{
    auto sm = SailreadsManager::Instance();
    connect(sm, &SailreadsManager::gotRecommendation,
            this, &RecommendationCommentsModel::handleGotRecommendation);
}

void RecommendationCommentsModel::componentComplete()
{
}

quint64 RecommendationCommentsModel::GetRecommendationId() const
{
    return m_RecommendationId;
}

void RecommendationCommentsModel::SetRecommendationId(quint64 id)
{
    if (m_RecommendationId != id) {
        m_RecommendationId = id;
        recommendationIdChanged();
    }
}

void RecommendationCommentsModel::fetchMoreContent()
{
    if (!rowCount()) {
        return;
    }

    SailreadsManager::Instance()->loadRecommendation(this, m_RecommendationId, m_CurrentPage);
}

void RecommendationCommentsModel::handleGotRecommendation(const RecommendationPtr& recommendation)
{
    if (!recommendation || recommendation->GetId() != m_RecommendationId) {
        return;
    }
    handleGotComments(recommendation->GetComments());
}

} // namespace Sailreads
