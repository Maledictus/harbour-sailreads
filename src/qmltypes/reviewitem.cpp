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
    connect(SailreadsManager::Instance(), &SailreadsManager::gotReviewInfo,
            this, &ReviewItem::handleGotReviewInfo);
    connect(SailreadsManager::Instance(this), &SailreadsManager::likeAdded,
            this, &ReviewItem::handleLikeAdded);
    connect(SailreadsManager::Instance(this), &SailreadsManager::likeRemoved,
            this, &ReviewItem::handleLikeRemoved);
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
    loadReview(true);
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

void ReviewItem::handleGotReviewInfo(const ReviewInfo& reviewInfo)
{
    if (!m_Review || m_Review->GetId() != reviewInfo.m_ReviewId) {
        return;
    }

    m_Review->SetRating(reviewInfo.m_Rating);
    m_Review->SetAddedDate(reviewInfo.m_CreateDate);
    m_Review->SetUpdatedDate(reviewInfo.m_UpdateDate);
    m_Review->SetReadDate(reviewInfo.m_ReadDate);
    m_Review->SetStartedDate(reviewInfo.m_StartedDate);
    m_Review->SetReadCount(reviewInfo.m_ReadCount);
    m_Review->SetBody(reviewInfo.m_Review);
    emit reviewChanged();
}

void ReviewItem::handleLikeAdded(const QString& resourceId, quint64 ratingId)
{
    if (m_Review && resourceId != m_ReviewId) {
        return;
    }

    m_Review->SetRatingId(ratingId);
    m_Review->SetLikesCount(m_Review->GetLikesCount() + 1);
    emit reviewChanged();
}

void ReviewItem::handleLikeRemoved(const QString& resourceId)
{
    if (m_Review && resourceId != m_ReviewId) {
        return;
    }

    m_Review->SetRatingId(0);
    m_Review->SetLikesCount(m_Review->GetLikesCount() - 1);
    emit reviewChanged();
}

void ReviewItem::loadReview(bool useCache)
{
    if (m_ReviewId.isEmpty()) {
        return;
    }
    SailreadsManager::Instance()->loadReview(this, m_ReviewId, useCache);
}
} // namespace Sailreads
