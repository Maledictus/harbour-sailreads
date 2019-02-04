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
#include <QString>

#include "../types.h"
#include "comment.h"

namespace Sailreads
{
class UserStatus : public QObject
{
    Q_OBJECT

    QString m_Id;
    QString m_Header;
    QString m_Body;
    QDateTime m_CreateDate;
    QDateTime m_UpdateDate;
    quint64 m_LikesCount;
    quint64 m_CommentsCount;
    bool m_IsLiked;
    quint64 m_RatingId;
    int m_Page;
    int m_Percent;
    quint64 m_WorkId;
    UserPtr m_User;
    BookPtr m_Book;
    CountedItems<Comment> m_Comments;
    UserStatuses_t m_PreviousStatuses;

    Q_PROPERTY(QString id READ GetId NOTIFY idChanged)
    Q_PROPERTY(QString header READ GetHeader NOTIFY headerChanged)
    Q_PROPERTY(QString body READ GetBody NOTIFY bodyChanged)
    Q_PROPERTY(QDateTime createDate READ GetCreateDate NOTIFY createDateChanged)
    Q_PROPERTY(QDateTime updateDate READ GetUpdateDate NOTIFY updateDateChanged)
    Q_PROPERTY(quint64 likesCount READ GetLikesCount NOTIFY likesCountChanged)
    Q_PROPERTY(quint64 commentsCount READ GetCommentsCount NOTIFY commentsCountChanged)
    Q_PROPERTY(bool isLiked READ GetIsLiked NOTIFY isLikedChanged)
    Q_PROPERTY(quint64 ratingId READ GetRatingId NOTIFY ratingIdChanged)
    Q_PROPERTY(int page READ GetPage NOTIFY pageChanged)
    Q_PROPERTY(int percent READ GetPercent NOTIFY percentChanged)
    Q_PROPERTY(quint64 workId READ GetWorkId NOTIFY workIdChanged)
    Q_PROPERTY(User* user READ GetUser NOTIFY userChanged)
    Q_PROPERTY(Book* book READ GetBook NOTIFY bookChanged)

public:
    UserStatus(QObject *parent = nullptr);
    virtual ~UserStatus();

    QString GetId() const;
    void SetId(const QString& id);
    QString GetHeader() const;
    void SetHeader(const QString& header);
    QString GetBody() const;
    void SetBody(const QString& body);
    QDateTime GetCreateDate() const;
    void SetCreateDate(const QDateTime& dt);
    QDateTime GetUpdateDate() const;
    void SetUpdateDate(const QDateTime& dt);
    quint64 GetLikesCount() const;
    void SetLikesCount(quint64 count);
    quint64 GetCommentsCount() const;
    void SetCommentsCount(quint64 count);
    bool GetIsLiked() const;
    void SetIsLiked(bool isLiked);
    void SetRatingId(quint64 ratingId);
    quint64 GetRatingId() const;
    int GetPage() const;
    void SetPage(int page);
    int GetPercent() const;
    void SetPercent(int percent);
    quint64 GetWorkId() const;
    void SetWorkId(quint64 workId);
    User* GetUser() const;
    UserPtr GetUserPtr() const;
    void SetUser(const UserPtr& user);
    Book* GetBook() const;
    BookPtr GetBookPtr() const;
    void SetBook(const BookPtr& book);
    CountedItems<Comment> GetComments() const;
    void SetComments(const CountedItems<Comment>& comments);
    UserStatuses_t GetPreviousStatuses() const;
    void SetPreviousStatuses(const UserStatuses_t& userStatuses);

signals:
    void idChanged();
    void headerChanged();
    void bodyChanged();
    void createDateChanged();
    void updateDateChanged();
    void likesCountChanged();
    void commentsCountChanged();
    void isLikedChanged();
    void ratingIdChanged();
    void pageChanged();
    void percentChanged();
    void workIdChanged();
    void userChanged();
    void bookChanged();
};
} // namespace Sailreads
