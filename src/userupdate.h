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
#include <QMap>
#include <QString>
#include <QUrl>

namespace Sailreads
{
class UserUpdate
{
public:
    enum class Type
    {
        NoType,
        UserStatus,
        UserChallenge,
        Review,
        ReadStatus
    };

    enum class ActionType
    {
        NoType,
        Rating
    };

    struct Actor
    {
        quint64 m_Id;
        QString m_Name;
        QUrl m_AvatarUrl;
        QUrl m_ProfileUrl;
    };

    struct Object
    {

    };

private:
    Type m_Type;
    QString m_ActionText;
    QUrl m_Url;
    QUrl m_Image;
    QDateTime m_Date;
    Actor m_Actor;
    QMap<ActionType, QString> m_Actions;

public:
    UserUpdate();

    Type GetType() const;
    void SetType(Type type);
    QString GetActionText() const;
    void SetActionText(const QString& text);
    QUrl GetUrl() const;
    void SetUrl(const QUrl& url);
    QUrl GetImageUrl() const;
    void SetImageUrl(const QUrl& url);
    QDateTime GetDate() const;
    void SetDate(const QDateTime& date);
    Actor GetActor() const;
    void SetActor(const Actor& actor);
    QMap<ActionType, QString> GetActions() const;
    void SetActions(const QMap<ActionType, QString>& actions);
};

typedef QList<UserUpdate> UserUpdates_t;
} // namespace Sailreads
