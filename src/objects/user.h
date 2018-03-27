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

#include <QObject>
#include <QUrl>

namespace Sailreads
{
class User
{
    Q_GADGET

    quint64 m_Id;
    QString m_FirstName;
    QString m_LastName;
    QString m_NickName;
    QUrl m_Avatar;

    Q_PROPERTY(quint64 id READ GetId)
    Q_PROPERTY(QString firstName READ GetFirstName)
    Q_PROPERTY(QString lastName READ GetLastName)
    Q_PROPERTY(QString nickName READ GetNickName)
    Q_PROPERTY(QUrl avatar READ GetAvatar)
public:
    User();

    quint64 GetId() const;
    void SetId(quint64 id);
    QString GetFirstName() const;
    void SetFirstName(const QString& name);
    QString GetLastName() const;
    void SetLastName(const QString& name);
    QString GetNickName() const;
    void SetNickName(const QString& name);
    QUrl GetAvatar() const;
    void SetAvatar(const QUrl& url);
};

} // namespace Sailreads
