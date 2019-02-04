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
class Recommendation : public QObject
{
    Q_OBJECT

    QString m_Id;
    QDateTime m_CreateDate;
    BookPtr m_Book;
    UserPtr m_FromUser;
    UserPtr m_ToUser;
    quint64 m_LikesCount;
    quint64 m_RatingId;
    CountedItems<Comment> m_Comments;

    Q_PROPERTY(QString id READ GetId NOTIFY idChanged)
    Q_PROPERTY(QDateTime createDate READ GetCreateDate NOTIFY createDateChanged)
    Q_PROPERTY(Book* book READ GetBook NOTIFY bookChanged)
    Q_PROPERTY(User* fromUser READ GetFromUser NOTIFY fromUserChanged)
    Q_PROPERTY(User* toUser READ GetToUser NOTIFY toUserChanged)
    Q_PROPERTY(quint64 likesCount READ GetLikesCount NOTIFY likesCountChanged)
    Q_PROPERTY(quint64 commentsCount READ GetCommentsCount NOTIFY commentsCountChanged)
    Q_PROPERTY(bool isLiked READ GetIsLiked NOTIFY isLikedChanged)
    Q_PROPERTY(quint64 ratingId READ GetRatingId NOTIFY ratingIdChanged)

public:
    Recommendation(QObject *parent = nullptr);
    virtual ~Recommendation();

    QString GetId() const;
    void SetId(const QString& id);
    QDateTime GetCreateDate() const;
    void SetCreateDate(const QDateTime& dt);
    Book* GetBook() const;
    BookPtr GetBookPtr() const;
    void SetBook(const BookPtr& book);
    User* GetFromUser() const;
    UserPtr GetFromUserPtr() const;
    void SetFromUser(const UserPtr& user);
    User* GetToUser() const;
    UserPtr GetToUserPtr() const;
    void SetToUser(const UserPtr& user);
    quint64 GetLikesCount() const;
    void SetLikesCount(quint64 count);
    bool GetIsLiked() const;
    quint64 GetCommentsCount() const;
    quint64 GetRatingId() const;
    void SetRatingId(quint64 ratingId);
    CountedItems<Comment> GetComments() const;
    void SetComments(const CountedItems<Comment>& comments);

signals:
    void idChanged();
    void createDateChanged();
    void bookChanged();
    void fromUserChanged();
    void toUserChanged();
    void likesCountChanged();
    void commentsCountChanged();
    void isLikedChanged();
    void ratingIdChanged();
};

} // namespace Sailreads
