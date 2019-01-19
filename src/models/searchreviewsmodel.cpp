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

#include "searchreviewsmodel.h"

#include "../sailreadsmanager.h"

namespace Sailreads
{
SearchReviewsModel::SearchReviewsModel(QObject *parent)
: ReviewsModel(parent)
{
}

QString SearchReviewsModel::GetSearchText() const
{
    return m_SearchText;
}

void SearchReviewsModel::SetSearchText(const QString& searchText)
{
    if (m_SearchText != searchText) {
        m_SearchText = searchText;
        m_CurrentPage = 1;
        m_HasMore = true;
        fetchMoreContent();
        emit searchTextChanged();
    }
}

void SearchReviewsModel::classBegin()
{
    auto sm = SailreadsManager::Instance();
    connect(sm, &SailreadsManager::gotFoundReviews,
            this, &SearchReviewsModel::handleGotFoundReviews);
}

void SearchReviewsModel::componentComplete()
{
}

void SearchReviewsModel::fetchMoreContent()
{
    if (m_SearchText.isEmpty()) {
        return;
    }
    SailreadsManager::Instance()->searchReviews(this, m_UserId, m_SearchText, m_CurrentPage);
}

void SearchReviewsModel::cancelRequest()
{
    SailreadsManager::Instance(this)->abortRequest(this);
}

void SearchReviewsModel::handleGotFoundReviews(const CountedItems<ReviewPtr>& reviews)
{
    if (reviews.m_BeginIndex == 1) {
        m_CurrentPage = 1;
        SetItems(reviews.m_Items);
    }
    else {
        AddItems(reviews.m_Items);
    }

    SetHasMore(reviews.m_EndIndex != reviews.m_Count);
    if (m_HasMore) {
        ++m_CurrentPage;
    }
}

} // namespace Sailreads
