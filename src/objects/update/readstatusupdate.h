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

#include "../../types.h"

namespace Sailreads
{
class ReadStatusUpdate : public QObject
{
    Q_OBJECT

    quint64 m_Id;
    QString m_ReviewId;
    QString m_UserId;
    QString m_OldStatus;
    QString m_CurrentStatus;
    QDateTime m_UpdatedDate;
    BookPtr m_Book;

    Q_PROPERTY(quint64 id READ GetId NOTIFY idChanged)
    Q_PROPERTY(QString reviewId READ GetReviewId NOTIFY reviewIdChanged)
    Q_PROPERTY(QString userId READ GetUserId NOTIFY userIdChanged)
    Q_PROPERTY(QString oldStatus READ GetOldStatus NOTIFY oldStatusChanged)
    Q_PROPERTY(QString currentStatus READ GetCurrentStatus NOTIFY currentStatusChanged)
    Q_PROPERTY(QDateTime updateDate READ GetUpdateDate NOTIFY updateDateChanged)
    Q_PROPERTY(Book* book READ GetBook NOTIFY bookChanged)

public:
    ReadStatusUpdate(QObject *parent = nullptr);

    quint64 GetId() const;
    void SetId(quint64 id);
    QString GetReviewId() const;
    void SetReviewId(const QString& id);
    QString GetUserId() const;
    void SetUserId(const QString& id);
    QString GetOldStatus() const;
    void SetOldStatus(const QString& status);
    QString GetCurrentStatus() const;
    void SetCurrentStatus(const QString& status);
    QDateTime GetUpdateDate() const;
    void SetUpdateDate(const QDateTime& dt);
    Book* GetBook() const;
    BookPtr GetBookPtr() const;
    void SetBook(const BookPtr& book);

signals:
    void idChanged();
    void reviewIdChanged();
    void userIdChanged();
    void oldStatusChanged();
    void currentStatusChanged();
    void updateDateChanged();
    void bookChanged();
};

} // namespace Sailreads
