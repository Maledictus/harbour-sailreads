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

#include "reviewsmodel.h"
#include "../sailreadsmanager.h"

#include "../objects/book.h"
#include "../objects/review.h"

namespace Sailreads
{
ReviewsModel::ReviewsModel(QObject *parent)
: BaseModel<ReviewPtr>(parent)
, m_UserId(0)
, m_BookShelfId(0)
, m_SortOrder(Qt::DescendingOrder)
, m_SortField("date_added")
, m_HasMore(true)
, m_CurrentPage(1)
{
}

quint64 ReviewsModel::GetUserId() const
{
    return m_UserId;
}

void ReviewsModel::SetUserId(quint64 id)
{
    if (m_UserId != id) {
        m_UserId = id;
        emit userIdChanged();
    }
}

quint64 ReviewsModel::GetBookShelfId() const
{
    return m_BookShelfId;
}

void ReviewsModel::SetBookShelfId(quint64 id)
{
    if (m_BookShelfId != id) {
        m_BookShelfId = id;
        emit bookShelfIdChanged();
    }
}

QString ReviewsModel::GetBookShelf() const
{
    return m_BookShelf;
}

void ReviewsModel::SetBookShelf(const QString& shelf)
{
    if (m_BookShelf != shelf) {
        m_BookShelf = shelf;
        emit bookShelfChanged();
    }
}

bool ReviewsModel::GetHasMore() const
{
    return m_HasMore;
}

void ReviewsModel::SetHasMore(bool has)
{
    if (m_HasMore != has) {
        m_HasMore = has;
        emit hasMoreChanged();
    }
}

Qt::SortOrder ReviewsModel::GetSortOrder() const
{
    return m_SortOrder;
}

void ReviewsModel::SetSortOrder(Qt::SortOrder sortOrder)
{
    if (m_SortOrder != sortOrder) {
        m_SortOrder = sortOrder;
        emit sortOrdereChanged();
    }
}

QString ReviewsModel::GetSortField() const
{
    return m_SortField;
}

void ReviewsModel::SetSortField(const QString& sortField)
{
    if (m_SortField != sortField) {
        m_SortField = sortField;
        emit sortFieldChanged();
    }
}

QVariant ReviewsModel::data(const QModelIndex& index, int role) const
{
    if (index.row() > m_Items.count() - 1 || index.row() < 0) {
        return QVariant();
    }

    const auto& review = m_Items.at(index.row());
    switch (role) {
    case Id:
        return review->GetId();
    case Book:
        return QVariant::fromValue(review->GetBook());
    case Rating:
        return review->GetRating();
    case Votes:
        return review->GetVotes();
    case Shelves:
        return review->GetShelvesList();
    case AddedDate:
        return review->GetAddedDate();
    case UpdatedDate:
        return review->GetUpdatedDate();
    case ReadDate:
        return review->GetReadDate();
    case StartedDate:
        return review->GetStartedDate();
    case ReadCount:
        return review->GetReadCount();
    case Body:
        return review->GetBody();
    case CommentsCount:
        return review->GetCommentsCount();
    case OwnedCount:
        return review->GetOwned();
    case Url:
        return review->GetUrl();
    case ReviewItem:
        return QVariant::fromValue(review.get());
    default:
        return QVariant();
    }
}

QHash<int, QByteArray> ReviewsModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[Id] = "reviewId";
    roles[Book] = "reviewBook";
    roles[Rating] = "reviewRating";
    roles[Votes] = "reviewVotes";
    roles[Shelves] = "reviewShelves";
    roles[AddedDate] = "reviewAddDate";
    roles[UpdatedDate] = "reviewUpdateDate";
    roles[ReadDate] = "reviewReadDate";
    roles[StartedDate] = "reviewStartDate";
    roles[ReadCount] = "reviewReadCount";
    roles[Body] = "reviewBody";
    roles[CommentsCount] = "reviewCommentsCount";
    roles[OwnedCount] = "reviewOwnedCount";
    roles[Url] = "reviewUrl";
    roles[ReviewItem] = "reviewReview";
    return roles;
}

void ReviewsModel::classBegin()
{
    connect(SailreadsManager::Instance(), &SailreadsManager::gotReviews,
            this, &ReviewsModel::handleGotReviews);
}

void ReviewsModel::componentComplete()
{
}

void ReviewsModel::fetchMoreContent()
{
    if (m_BookShelf.isEmpty() || !m_UserId) {
        SetHasMore(false);
        return;
    }

    SailreadsManager::Instance()->loadReviews(m_UserId, m_BookShelf, m_CurrentPage,
                                              m_SortOrder, m_SortField);
}

void ReviewsModel::update()
{
    SetHasMore(true);
    m_CurrentPage = 1;
    fetchMoreContent();
}

void ReviewsModel::handleGotReviews(quint64 booksShelfId, const CountedItems<ReviewPtr>& reviews)
{
    if (m_BookShelfId != booksShelfId) {
        return;
    }

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

}
