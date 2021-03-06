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

class QAbstractItemModel;

namespace Sailreads
{
class AuthorBooksModel;
class AuthorProfileItem: public ItemRequestCanceler
{
    Q_OBJECT

    QString m_AuthorId;
    AuthorPtr m_Author;
    AuthorBooksModel *m_ShortBooksModel;

    Q_PROPERTY(QString authorId READ GetAuthorId WRITE SetAuthorId NOTIFY authorIdChanged)
    Q_PROPERTY(Author* author READ GetAuthor WRITE SetAuthor NOTIFY authorChanged)
    Q_PROPERTY(AuthorBooksModel* shortBooksModel READ GetShortBooksModel NOTIFY shortBooksModelChanged)

public:
    AuthorProfileItem(QObject *parent = nullptr);

    QString GetAuthorId() const;
    void SetAuthorId(const QString& authorId);
    void SetAuthor(const AuthorPtr& author);
    Author* GetAuthor() const;
    void SetAuthor(Author *author);
    AuthorBooksModel* GetShortBooksModel() const;

private slots:
    void handleGotAuthorProfile(const AuthorPtr& author);
    void handleAuthorFollowed(const QString& authorId, quint64 followingId);
    void handleAuthorUnfollowed(const QString& authorId);
public slots:
    void loadAuthorProfile(bool useCache);

signals:
    void authorIdChanged();
    void authorChanged();
    void shortBooksModelChanged();
};
}
