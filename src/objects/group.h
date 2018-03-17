/*
Copyright (c) 2018 Oleg Linkin <maledictusdemagog@gmail.com>

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
class Group
{
    quint64 m_Id;
    QString m_Name;
    bool m_IsPublic;
    quint64 m_UsersCount;
    QUrl m_ImageUrl;
    QDateTime m_LastActivity;
    QUrl m_Url;

public:
    Group();

    quint64 GetId() const;
    void SetId(quint64 id);
    QString GetName() const;
    void SetName(const QString& name);
    bool GetIsPublic() const;
    void SetIsPublic(bool isPublic);
    quint64 GetUsersCount() const;
    void SetUsersCount(quint64 count);
    QUrl GetImageUrl() const;
    void SetImageUrl(const QUrl& url);
    QDateTime GetLastActivity() const;
    void SetLastActivity(const QDateTime& dt);
    QUrl GetUrl() const;
    void SetUrl(const QUrl& url);
};

typedef QList<Group> Groups_t;
} // namespace Sailreads
