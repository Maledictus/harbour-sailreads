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

#include <QDateTime>
#include <QObject>

#include "../../types.h"

namespace Sailreads
{
class UserStatusUpdate : public QObject
{
    Q_OBJECT

    quint64 m_Id;
    quint64 m_UserId;
    quint64 m_BookId;
    int m_Page;
    quint64 m_CommentsCount;
    QString m_Body;
    QDateTime m_CreateDate;
    QDateTime m_UpdateDate;
    int m_Percent;
    quint64 m_WorkId;
    quint64 m_ReviewId;
    BookPtr m_Book;

    Q_PROPERTY(quint64 id READ GetId NOTIFY idChanged)
    Q_PROPERTY(quint64 userId READ GetUserId NOTIFY userIdChanged)
    Q_PROPERTY(quint64 bookId READ GetBookId NOTIFY bookIdChanged)
    Q_PROPERTY(int page READ GetPage NOTIFY pageChanged)
    Q_PROPERTY(quint64 commentsCount READ GetCommentsCount NOTIFY commentsCountChanged)
    Q_PROPERTY(QString body READ GetBody NOTIFY bodyChanged)
    Q_PROPERTY(QDateTime createDate READ GetCreateDate NOTIFY createDateChanged)
    Q_PROPERTY(QDateTime updateDate READ GetUpdateDate NOTIFY updateDateChanged)
    Q_PROPERTY(int percent READ GetPercent NOTIFY percentChanged)
    Q_PROPERTY(quint64 workId READ GetWorkId NOTIFY workIdChanged)
    Q_PROPERTY(quint64 reviewId READ GetReviewId NOTIFY reviewIdChanged)
    Q_PROPERTY(Book* book READ GetBook NOTIFY bookChanged)

public:
    UserStatusUpdate(QObject *parent = nullptr);

    quint64 GetId() const;
    void SetId(quint64 id);
    quint64 GetUserId() const;
    void SetUserId(quint64 id);
    quint64 GetBookId() const;
    void SetBookId(quint64 id);
    int GetPage() const;
    void SetPage(int page);
    quint64 GetCommentsCount() const;
    void SetCommentsCount(quint64 count);
    QString GetBody() const;
    void SetBody(const QString& body);
    QDateTime GetCreateDate() const;
    void SetCreateDate(const QDateTime& dt);
    QDateTime GetUpdateDate() const;
    void SetUpdateDate(const QDateTime& dt);
    int GetPercent() const;
    void SetPercent(int percent);
    quint64 GetWorkId() const;
    void SetWorkId(quint64 id);
    quint64 GetReviewId() const;
    void SetReviewId(quint64 id);
    Book* GetBook() const;
    BookPtr GetBookPtr() const;
    void SetBook(const BookPtr& book);

signals:
    void idChanged();
    void userIdChanged();
    void bookIdChanged();
    void pageChanged();
    void commentsCountChanged();
    void bodyChanged();
    void createDateChanged();
    void updateDateChanged();
    void percentChanged();
    void workIdChanged();
    void reviewIdChanged();
    void bookChanged();
};
} // namespace Sailreads
