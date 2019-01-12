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
#include <QString>
#include <QObject>

#include "../types.h"

namespace Sailreads
{
class Notification : public QObject
{
    Q_OBJECT

    Users_t m_Actors;
    bool m_IsNew;
    QDateTime m_CreateDate;
    QString m_PlainTextBody;
    QString m_HtmlTextBody;
    QUrl m_Url;
    QString m_ResourceType;
    QString m_GroupResourceType;

    Q_PROPERTY(QObjectList actors READ GetActors NOTIFY actorsChanged)
    Q_PROPERTY(bool isNew READ GetIsNew NOTIFY isNewChanged)
    Q_PROPERTY(QDateTime createDate READ GetCreateDate NOTIFY createDateChanged)
    Q_PROPERTY(QString plainTextBody READ GetPlainTextBody NOTIFY plainTextBodyChanged)
    Q_PROPERTY(QString htmlTextBody READ GetHtmlTextBody NOTIFY htmlTextBodyChanged)
    Q_PROPERTY(QUrl url READ GetUrl NOTIFY urlChanged)
    Q_PROPERTY(QString resourceType READ GetResourceType NOTIFY resourceTypeChanged)
    Q_PROPERTY(QString groupResourceType READ GetGroupResourceType NOTIFY groupResourceTypeChanged)

public:
    Notification(QObject *parent = nullptr);
    ~Notification();

    QObjectList GetActors() const;
    Users_t GetActorsPtr() const;
    void SetActors(const Users_t& actors);
    bool GetIsNew() const;
    void SetIsNew(bool isNew);
    QDateTime GetCreateDate() const;
    void SetCreateDate(const QDateTime& dt);
    QString GetPlainTextBody() const;
    void SetPlainTextBody(const QString& text);
    QString GetHtmlTextBody() const;
    void SetHtmlTextBody(const QString& text);
    QUrl GetUrl() const;
    void SetUrl(const QUrl& url);
    QString GetResourceType() const;
    void SetResourceType(const QString& resourceType);
    QString GetGroupResourceType() const;
    void SetGroupResourceType(const QString& groupResourceType);

signals:
    void actorsChanged();
    void isNewChanged();
    void createDateChanged();
    void plainTextBodyChanged();
    void urlChanged();
    void resourceTypeChanged();
    void groupResourceTypeChanged();
    void htmlTextBodyChanged(QString htmlTextBody);
};

} // namespace Sailreads
