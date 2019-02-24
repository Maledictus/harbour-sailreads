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

#include "review.h"

#include <QtDebug>

#include "book.h"

namespace Sailreads
{
Review::Review(QObject *parent)
: QObject(parent)
, m_Id("")
, m_Rating(0)
, m_LikesCount(0)
, m_ReadCount(0)
, m_CommentsCount(0)
, m_Owned(0)
, m_RatingId(0)
{
#ifdef QT_DEBUG
    qDebug() << this << "CONSTRUCTED";
#endif
}

Review::~Review()
{
#ifdef QT_DEBUG
    qDebug() << this << "DESTRUCTED";
#endif
}

QString Review::GetId() const
{
    return m_Id;
}

void Review::SetId(const QString& id)
{
    if (m_Id != id) {
        m_Id = id;
        emit idChanged();
    }
}

Book* Review::GetBook() const
{
    return m_Book.get();
}

BookPtr Review::GetBookPtr() const
{
    return m_Book;
}

void Review::SetBook(const BookPtr& book)
{
    if (m_Book != book) {
        m_Book = book;
        emit bookChanged();
    }
}

int Review::GetRating() const
{
    return m_Rating;
}

void Review::SetRating(int rating)
{
    if (m_Rating != rating) {
        m_Rating = rating;
        emit ratingChanged();
    }
}

int Review::GetLikesCount() const
{
    return m_LikesCount;
}

void Review::SetLikesCount(int votes)
{
    if (m_LikesCount != votes) {
        m_LikesCount = votes;
        emit likesCountChanged();
    }
}

BookShelves_t Review::GetShelves() const
{
    return m_Shelves;
}

void Review::SetShelves(const BookShelves_t& shelves)
{
    m_Shelves = shelves;
    emit shelvesListChanged();
    emit exclusiveShelfChanged();
}

QStringList Review::GetShelvesList() const
{
    QStringList list;
    std::transform(m_Shelves.begin(), m_Shelves.end(),
        std::back_inserter(list),
        [](decltype(m_Shelves.front()) shelf){ return shelf.GetName(); });
    return list;
}

QString Review::GetExclusiveShelf() const
{
    for (const auto& shelf : m_Shelves) {
        if (shelf.GetExclusive()) {
            return shelf.GetName();
        }
    }
    return "";
}

QDateTime Review::GetAddedDate() const
{
    return m_AddedDate;
}

void Review::SetAddedDate(const QDateTime& addedDate)
{
    if (m_AddedDate != addedDate) {
        m_AddedDate = addedDate;
        emit addedDateChanged();
    }
}

QDateTime Review::GetUpdatedDate() const
{
    return m_UpdatedDate;
}

void Review::SetUpdatedDate(const QDateTime& updatedDate)
{
    if (m_UpdatedDate != updatedDate) {
        m_UpdatedDate = updatedDate;
        emit updatedDateChanged();
    }
}

QDateTime Review::GetReadDate() const
{
    return m_ReadDate;
}

void Review::SetReadDate(const QDateTime& readDate)
{
    if (m_ReadDate != readDate) {
        m_ReadDate = readDate;
        emit readDateChanged();
    }
}

QDateTime Review::GetStartedDate() const
{
    return m_StartedDate;
}

void Review::SetStartedDate(const QDateTime& startedDate)
{
    if (m_StartedDate != startedDate) {
        m_StartedDate = startedDate;
        emit startedDateChanged();
    }
}

int Review::GetReadCount() const
{
    return m_ReadCount;
}

void Review::SetReadCount(int readCount)
{
    if (m_ReadCount != readCount) {
        m_ReadCount = readCount;
        emit readCountChanged();
    }
}

QString Review::GetBody() const
{
    return m_Body;
}

void Review::SetBody(const QString& body)
{
    if (m_Body != body) {
        m_Body = body;
        emit bodyChanged();
    }
}

quint64 Review::GetCommentsCount() const
{
    return m_CommentsCount;
}

void Review::SetCommentsCount(const quint64& commentsCount)
{
    if (m_CommentsCount != commentsCount) {
        m_CommentsCount = commentsCount;
        emit commentsCountChanged();
    }
}

quint64 Review::GetOwned() const
{
    return m_Owned;
}

void Review::SetOwned(const quint64& owned)
{
    if (m_Owned != owned) {
        m_Owned = owned;
        emit ownedChanged();
    }
}

QUrl Review::GetUrl() const
{
    return m_Url;
}

void Review::SetUrl(const QUrl& url)
{
    if (m_Url != url) {
        m_Url = url;
        emit urlChanged();
    }
}

User *Review::GetUser() const
{
    return m_User.get();
}

UserPtr Review::GetUserPtr() const
{
    return m_User;
}

void Review::SetUser(const UserPtr& user)
{
    if (m_User != user) {
        m_User = user;
        emit userChanged();
    }
}

CountedItems<Comment> Review::GetComments() const
{
    return m_Comments;
}

void Review::SetComments(const CountedItems<Comment>& comments)
{
    m_Comments = comments;
}

QString Review::GetShortDescription() const
{
    if (!m_Rating) {
        if (m_ReadDate.isValid() && !m_ReadDate.isNull()) {
            return tr("finished reading");
        }
        else if (m_StartedDate.isValid() && !m_StartedDate.isNull() &&
                 m_UpdatedDate == m_StartedDate) {
            return tr("started reading");
        }
        else if (m_StartedDate.isValid() && !m_StartedDate.isNull()) {
            return tr("is currently reading");
        }
        else if (m_AddedDate.isValid() && !m_AddedDate.isNull()) {
            return tr("wants to read");
        }
    }
    else {
        return tr("rated it");
    }
    return "";
}

bool Review::GetIsLiked() const
{
    return m_RatingId > 0;
}

void Review::SetRatingId(quint64 ratingId)
{
    if (m_RatingId != ratingId) {
        m_RatingId = ratingId;
        emit ratingIdChanged();
        emit isLikedChanged();
    }
}

quint64 Review::GetRatingId() const
{
    return m_RatingId;
}

bool Review::GetSpoilerFlag() const
{
    return m_SpoilerFlag;
}

void Review::SetSpoilerFlag(bool set)
{
    if (m_SpoilerFlag != set) {
        m_SpoilerFlag = set;
        emit spoilerFlagChanged();
    }
}

UserStatuses_t Review::GetUserStatuses() const
{
    return m_UserStatuses;
}

void Review::SetUserStatuses(const UserStatuses_t& userStatuses)
{
    m_UserStatuses = userStatuses;
    emit lastUserStatusChanged();
}

UserStatus* Review::GetLastUserStatus() const
{
    return m_UserStatuses.isEmpty() ? nullptr : m_UserStatuses.first().get();
}

ReadStatuses_t Review::GetReadStatuses() const
{
    return m_ReadStatuses;
}

void Review::SetReadStatuses(const ReadStatuses_t& readStatues)
{
    m_ReadStatuses = readStatues;
}

void Review::updateShelves(Review *newReview)
{
    if (!newReview) {
        return;
    }

    SetUpdatedDate(newReview->GetUpdatedDate());
    SetAddedDate(newReview->GetAddedDate());
    SetReadDate(newReview->GetReadDate());
    SetStartedDate(newReview->GetStartedDate());
    SetRating(newReview->GetRating());
    SetShelves(newReview->GetShelves());
}

void Review::updateShelves(const ReviewPtr& newReview)
{
    updateShelves(newReview.get());
}

void Review::Update(Review *newReview)
{
    if (!newReview) {
        return;
    }

    SetId(newReview->GetId());
    SetBook(newReview->GetBookPtr());
    SetRating(newReview->GetRating());
    SetLikesCount(newReview->GetLikesCount());
    SetShelves(newReview->GetShelves());
    SetAddedDate(newReview->GetAddedDate());
    SetUpdatedDate(newReview->GetUpdatedDate());
    SetReadDate(newReview->GetReadDate());
    SetStartedDate(newReview->GetStartedDate());
    SetReadCount(newReview->GetReadCount());
    SetBody(newReview->GetBody());
    SetCommentsCount(newReview->GetCommentsCount());
    SetOwned(newReview->GetOwned());
    SetUrl(newReview->GetUrl());
    SetUser(newReview->GetUserPtr());
}

void Review::Update(const ReviewPtr& newReview)
{
    if (!newReview) {
        return;
    }

    Update(newReview.get());
    SetComments(newReview->GetComments());
    SetUserStatuses(newReview->GetUserStatuses());
    SetReadStatuses(newReview->GetReadStatuses());
}

} // namespace Sailreads
