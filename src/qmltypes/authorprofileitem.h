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

#include "../types.h"

namespace Sailreads
{
class AuthorProfileItem: public QObject
{
    Q_OBJECT

    quint64 m_AuthorId;
    AuthorPtr m_Author;
    Q_PROPERTY(quint64 authorId READ GetAuthorId WRITE SetAuthorId NOTIFY authorIdChanged)
    Q_PROPERTY(Author* author READ GetAuthor WRITE SetAuthor NOTIFY authorChanged)

public:
    AuthorProfileItem(QObject *parent = nullptr);

    quint64 GetAuthorId() const;
    void SetAuthorId(quint64 authorId);
    void SetAuthor(const AuthorPtr& author);
    Author* GetAuthor() const;
    void SetAuthor(Author *author);

private slots:
    void handleGotAuthorProfile(const AuthorPtr& author);
    void handleAuthorFollowed(quint64 authorId, quint64 followingId);
    void handleAuthorUnfollowed(quint64 authorId);
public slots:
    void updateAuthorProfile();
    void cancelRequest();

signals:
    void authorIdChanged();
    void authorChanged();
};
}
