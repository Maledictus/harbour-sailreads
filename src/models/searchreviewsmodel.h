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

#include "reviewsmodel.h"
#include "../types.h"

namespace Sailreads
{
class SearchReviewsModel : public ReviewsModel
{
    Q_OBJECT
    Q_ENUMS(ReviewRoles)

    QString m_SearchText;
    Q_PROPERTY(QString searchText READ GetSearchText WRITE SetSearchText NOTIFY searchTextChanged)

    QString m_searchText;

public:
    SearchReviewsModel(QObject *parent = nullptr);

    QString GetSearchText() const;
    void SetSearchText(const QString& searchText);

    virtual void classBegin() override;
    virtual void componentComplete() override;

private slots:
    void handleGotFoundReviews(const CountedItems<ReviewPtr>& reviews);
public slots:
    virtual void fetchMoreContent() override;

signals:
    void searchTextChanged();
};

} // namespace Sailreads

