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

#include "recommendationitem.h"

#include "../objects/recommendation.h"
#include "../sailreadsmanager.h"

namespace Sailreads
{
RecommendationItem::RecommendationItem(QObject *parent)
: ItemRequestCanceler(parent)
{
    connect(SailreadsManager::Instance(this), &SailreadsManager::gotRecommendation,
            this, &RecommendationItem::handleGotRecommendation);
}

QString RecommendationItem::GetRecommendationId() const
{
    return m_RecommendationId;
}

void RecommendationItem::SetRecommendationId(const QString& id)
{
    if (m_RecommendationId != id) {
        m_RecommendationId = id;
        loadRecommendation();
        emit recommendationIdChanged();
    }
}

Recommendation* RecommendationItem::GetRecommendation() const
{
    return m_Recommendation.get();
}

void RecommendationItem::SetRecommendation(const RecommendationPtr& recommendation)
{
    if (m_Recommendation != recommendation) {
        m_Recommendation = recommendation;
        emit recommendationChanged();
    }
}

void RecommendationItem::handleGotRecommendation(const RecommendationPtr& recommendation)
{
    if (!recommendation || m_RecommendationId != recommendation->GetId()) {
        return;
    }
    SetRecommendation(recommendation);
}

void RecommendationItem::loadRecommendation()
{
    SailreadsManager::Instance(this)->loadRecommendation(this, m_RecommendationId);
}

} // namespace Sailreads
