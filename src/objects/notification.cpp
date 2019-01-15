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

#include "notification.h"

#include <QtDebug>

#include "../objects/user.h"

namespace Sailreads
{
Notification::Notification(QObject *parent)
: QObject(parent)
, m_IsNew(false)
, m_ResourceId(0)
{
#ifdef QT_DEBUG
    qDebug() << this << "CONSTRUCTED";
#endif
}

Notification::~Notification()
{
#ifdef QT_DEBUG
    qDebug() << this << "DESTRUCTED";
#endif
}

QObjectList Notification::GetActors() const
{
    QObjectList objList;
    for (const auto& actor : m_Actors) {
        objList << actor.get();
    }
    return objList;
}

Users_t Notification::GetActorsPtr() const
{
    return m_Actors;
}

void Notification::SetActors(const Users_t& actors)
{
    m_Actors = actors;
}

bool Notification::GetIsNew() const
{
    return m_IsNew;
}

void Notification::SetIsNew(bool isNew)
{
    m_IsNew = isNew;
}

QDateTime Notification::GetCreateDate() const
{
    return m_CreateDate;
}

void Notification::SetCreateDate(const QDateTime& dt)
{
    m_CreateDate = dt;
}

QString Notification::GetPlainTextBody() const
{
    return m_PlainTextBody;
}

void Notification::SetPlainTextBody(const QString& text)
{
    m_PlainTextBody = text;
}

QString Notification::GetHtmlTextBody() const
{
    return m_HtmlTextBody;
}

void Notification::SetHtmlTextBody(const QString& text)
{
    m_HtmlTextBody = text;
}

QUrl Notification::GetUrl() const
{
    return m_Url;
}

void Notification::SetUrl(const QUrl& url)
{
    m_Url = url;
}

QString Notification::GetResourceType() const
{
    return m_ResourceType;
}

void Notification::SetResourceType(const QString& resourceType)
{
    m_ResourceType = resourceType;
}

QString Notification::GetGroupResourceType() const
{
    return m_GroupResourceType;
}

void Notification::SetGroupResourceType(const QString& groupResourceType)
{
    m_GroupResourceType = groupResourceType;
}

quint64 Notification::GetResourceId() const
{
    return m_ResourceId;
}

void Notification::SetResourceId(quint64 resourceId)
{
    m_ResourceId = resourceId;
}

} // namespace Sailreads
