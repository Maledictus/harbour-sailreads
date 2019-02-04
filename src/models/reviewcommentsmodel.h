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

#include "basecommentsmodel.h"

namespace Sailreads
{
class ReviewCommentsModel: public BaseCommentsModel
{
    Q_OBJECT

    QString m_ReviewId;
    Q_PROPERTY(QString reviewId READ GetReviewId WRITE SetReviewId NOTIFY reviewIdChanged)

public:
    explicit ReviewCommentsModel(QObject *parent = nullptr);

    virtual void classBegin() override;
    virtual void componentComplete() override;

    QString GetReviewId() const;
    void SetReviewId(const QString& id);

private slots:
    void handleGotReview(const ReviewPtr& review);
    void handleNewCommentAdded(const QString& resourceId, const Comment& comment);
public slots:
    void fetchMoreContent();

signals:
    void reviewIdChanged();
};

} // namespace Sailreads
