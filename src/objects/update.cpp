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

#include "update.h"

#include <QtDebug>

#include "../objects/user.h"

namespace Sailreads
{
Update::Update(QObject *parent)
: QObject(parent)
{
#ifdef QT_DEBUG
    qDebug() << this << "CONSTRUCTED";
#endif
}

Update::~Update()
{
#ifdef QT_DEBUG
    qDebug() << this << "DESTRUCTED";
#endif
}

QString Update::GetActionText() const
{
    return m_ActionText;
}

void Update::SetActionText(const QString& actionText)
{
    m_ActionText = actionText;
}

QString Update::GetHeaderText() const
{
    switch (m_UpdateType) {
    case Update::Comment:
        return m_ActionText;
    default:
        return QString("<a href=\"%1\" >%2</a> %3")
                .arg(QUrl(m_Actor ? m_Actor->GetWebUrl() : QUrl()).toString())
                .arg(m_Actor ? m_Actor->GetUserName() : QString())
                .arg(m_ActionText);
    }
    return "";
}

QUrl Update::GetLink() const
{
    return m_Link;
}

void Update::SetLink(const QUrl& link)
{
    m_Link = link;
}

QUrl Update::GetImageUrl() const
{
    return m_ImageUrl;
}

void Update::SetImageUrl(const QUrl& url)
{
    m_ImageUrl = url;
}

UserPtr Update::GetActorPtr() const
{
    return m_Actor;
}

User* Update::GetActor() const
{
    return m_Actor.get();
}

void Update::SetActor(const UserPtr& actor)
{
    m_Actor = actor;
}

QDateTime Update::GetUpdatedDate() const
{
    return m_UpdatedDate;
}

void Update::SetUpdateDate(const QDateTime& dt)
{
    m_UpdatedDate = dt;
}

Update::UpdateType Update::GetUpdateType() const
{
    return m_UpdateType;
}

void Update::SetUpdateType(Update::UpdateType ut)
{
    m_UpdateType = ut;
}

QObjectPtr Update::GetUpdateObject() const
{
    return m_UpdateObject;
}

void Update::SetUpdateObject(const QObjectPtr& object)
{
    m_UpdateObject = object;
}
} // namespace Sailreads
