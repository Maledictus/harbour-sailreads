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

#pragma once

#include <QObject>

#include "../itemrequestcanceler.h"
#include "../types.h"

namespace Sailreads
{
class ReviewItem : public ItemRequestCanceler
{
    Q_OBJECT

    quint64 m_ReviewId;
    ReviewPtr m_Review;

    Q_PROPERTY(quint64 reviewId READ GetReviewId WRITE SetReviewId NOTIFY reviewIdChanged)
    Q_PROPERTY(Review* review READ GetReview WRITE SetReview NOTIFY reviewChanged)

public:
    explicit ReviewItem(QObject *parent = nullptr);

    quint64 GetReviewId() const;
    void SetReviewId(quint64 reviewId);
    Review* GetReview() const;
    void SetReview(Review* review);
    void SetReview(const ReviewPtr& review);

private slots:
    void handleGotReview(const ReviewPtr& review);
public slots:
    void loadReview();

signals:
    void reviewIdChanged();
    void reviewChanged();
};
} // namespace Sailreads
