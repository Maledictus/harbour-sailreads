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

#include "reviewitem.h"

#include "../objects/review.h"
#include "../sailreadsmanager.h"

namespace Sailreads
{
ReviewItem::ReviewItem(QObject *parent)
: ItemRequestCanceler(parent)
, m_ReviewId("")
{
    connect(SailreadsManager::Instance(), &SailreadsManager::gotReview,
            this, &ReviewItem::handleGotReview);
}

QString ReviewItem::GetReviewId() const
{
    return m_ReviewId;
}

void ReviewItem::SetReviewId(const QString& reviewId)
{
    if (m_ReviewId == reviewId) {
        return;
    }

    m_ReviewId = reviewId;
    loadReview();
    emit reviewIdChanged();
}

Review* ReviewItem::GetReview() const
{
    return m_Review.get();
}

void ReviewItem::SetReview(Review *review)
{
    if (!review) {
        return;
    }

    if (!m_Review) {
        m_Review = ReviewPtr(new Review());
    }
    m_Review->Update(review);
    emit reviewChanged();
}

void ReviewItem::SetReview(const ReviewPtr& review)
{
    if (!review) {
        return;
    }

    if (!m_Review) {
        m_Review = ReviewPtr(new Review());
    }
    m_Review->Update(review);
    emit reviewChanged();
}

void ReviewItem::handleGotReview(const ReviewPtr& review)
{
    if (!review || (m_ReviewId != review->GetId() && !m_ReviewId.startsWith(review->GetId()))) {
        return;
    }

    m_ReviewId = review->GetId();
    emit reviewIdChanged();
    SetReview(review);
}

void ReviewItem::loadReview()
{
    if (m_ReviewId.isEmpty()) {
        return;
    }
    SailreadsManager::Instance()->loadReview(this, m_ReviewId);
}
} // namespace Sailreads
