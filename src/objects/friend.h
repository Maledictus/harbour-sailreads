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
#include <QList>
#include <QUrl>

namespace Sailreads
{
class Friend
{
    QString m_Id;
    QString m_Name;
    QUrl m_Url;
    QUrl m_AvatarUrl;
    QUrl m_SmallAvatarUrl;
    quint64 m_FriendsCount;
    quint64 m_BooksCount;
    QDateTime m_CreatedAt;
    bool m_IsMutualFriend;
public:
    explicit Friend();
    ~Friend();

    QString GetId() const;
    void SetId(const QString& id);
    QString GetName() const;
    void SetName(const QString& name);
    QUrl GetUrl() const;
    void SetUrl(const QUrl& url);
    QUrl GetAvatarUrl() const;
    void SetAvatarUrl(const QUrl& url);
    QUrl GetSmallAvatarUrl() const;
    void SetSmallAvatarUrl(const QUrl& url);
    quint64 GetFriendsCount() const;
    void SetFriendsCount(quint64 count);
    quint64 GetBooksCount() const;
    void SetBooksCount(quint64 count);
    QDateTime GetCreatedDate() const;
    void SetCreatedDate(const QDateTime& dt);
    bool GetIsMutualFriend() const;
    void SetIsMutualFriend(bool isMutualFriend);
};
typedef QList<Friend> Friends_t;
} // namespace Sailreads
