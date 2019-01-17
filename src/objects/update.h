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

#include <memory>

#include <QDateTime>
#include <QObject>
#include <QString>
#include <QUrl>

#include "../types.h"

namespace Sailreads
{
class Update : public QObject
{
    Q_OBJECT

    QString m_ActionText;
    QUrl m_Link;
    QUrl m_ImageUrl;
    UserPtr m_Actor;
    QDateTime m_UpdatedDate;
    QObjectPtr m_UpdateObject;
public:
    enum UpdateType
    {
        Unknown = -1,
        ReadStatus = 0,
        Review,
        UserStatus,
        Friend,
        Comment,
        UserChallenge
    };
    Q_ENUMS(UpdateType)
private:
    UpdateType m_UpdateType;

public:
    Update(QObject *parent = nullptr);
    ~Update();

    QString GetActionText() const;
    void SetActionText(const QString& actionText);
    QString GetHeaderText() const;
    QUrl GetLink() const;
    void SetLink(const QUrl& link);
    QUrl GetImageUrl() const;
    void SetImageUrl(const QUrl& url);
    UserPtr GetActorPtr() const;
    User* GetActor() const;
    void SetActor(const UserPtr& actor);
    QDateTime GetUpdatedDate() const;
    void SetUpdateDate(const QDateTime& dt);
    UpdateType GetUpdateType() const;
    void SetUpdateType(UpdateType ut);
    QObjectPtr GetUpdateObject() const;
    void SetUpdateObject(const QObjectPtr& object);
};

} // namespace Sailreads
