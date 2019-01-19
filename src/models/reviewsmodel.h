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

#include <QQmlParserStatus>

#include "basemodel.h"
#include "../types.h"

namespace Sailreads
{
class ReviewsModel : public BaseModel<ReviewPtr>
                    , public QQmlParserStatus
{
    Q_OBJECT
    Q_INTERFACES(QQmlParserStatus)

    Q_ENUMS(ReviewRoles)
protected:
    quint64 m_UserId;
    quint64 m_BookShelfId;
    QString m_BookShelf;
    Qt::SortOrder m_SortOrder;
    QString m_SortField;
    bool m_HasMore;
    quint64 m_CurrentPage;

    Q_PROPERTY(quint64 userId READ GetUserId WRITE SetUserId NOTIFY userIdChanged)
    Q_PROPERTY(quint64 bookShelfId READ GetBookShelfId WRITE SetBookShelfId NOTIFY bookShelfIdChanged)
    Q_PROPERTY(QString bookShelf READ GetBookShelf WRITE SetBookShelf NOTIFY bookShelfChanged)
    Q_PROPERTY(bool hasMore READ GetHasMore WRITE SetHasMore NOTIFY hasMoreChanged)
    Q_PROPERTY(Qt::SortOrder sortOrder READ GetSortOrder WRITE SetSortOrder NOTIFY sortOrdereChanged)
    Q_PROPERTY(QString sortField READ GetSortField WRITE SetSortField NOTIFY sortFieldChanged)

public:
    enum ReviewRoles
    {
        Id = Qt::UserRole + 1,
        Book,
        Rating,
        Votes,
        Shelves,
        AddedDate,
        UpdatedDate,
        ReadDate,
        StartedDate,
        ReadCount,
        Body,
        CommentsCount,
        OwnedCount,
        Url,
        ReviewItem
    };

    ReviewsModel(QObject *parent = nullptr);

    quint64 GetUserId() const;
    void SetUserId(quint64 id);
    quint64 GetBookShelfId() const;
    void SetBookShelfId(quint64 id);
    QString GetBookShelf() const;
    void SetBookShelf(const QString& shelf);
    bool GetHasMore() const;
    void SetHasMore(bool has);
    Qt::SortOrder GetSortOrder() const;
    void SetSortOrder(Qt::SortOrder sortOrder);
    QString GetSortField() const;
    void SetSortField(const QString& sortField);

    virtual QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    virtual QHash<int, QByteArray> roleNames() const override;

    virtual void classBegin();
    virtual void componentComplete();

private slots:
    void handleGotReviews(quint64 booksShelfId, const CountedItems<ReviewPtr>& reviews);
public slots:
    virtual void fetchMoreContent();
    void loadReviews();

signals:
    void userIdChanged();
    void bookShelfIdChanged();
    void bookShelfChanged();
    void hasMoreChanged();
    void sortOrdereChanged();
    void sortFieldChanged();
};

} // namespace Sailreads

