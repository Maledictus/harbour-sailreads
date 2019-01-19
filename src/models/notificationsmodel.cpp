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

#include "notificationsmodel.h"

#include "../sailreadsmanager.h"
#include "../objects/notification.h"

namespace Sailreads
{
NotificationsModel::NotificationsModel(QObject *parent)
: BaseModel<NotificationPtr>(parent)
, m_HasMore(true)
{
    connect(SailreadsManager::Instance(), &SailreadsManager::gotNotifications,
            this, &NotificationsModel::handleGotNotifications);
}

bool NotificationsModel::GetHasMore() const
{
    return m_HasMore;
}

void NotificationsModel::SetHasMore(bool has)
{
    if (m_HasMore != has) {
        m_HasMore = has;
        emit hasMoreChanged();
    }
}

QVariant NotificationsModel::data(const QModelIndex& index, int role) const
{
    if (index.row() > m_Items.count() - 1 || index.row() < 0) {
        return QVariant();
    }

    const auto& notification = m_Items.at(index.row());
    switch (role) {
    case Actors:
        return QVariant::fromValue(notification->GetActors());
    case IsNew:
        return notification->GetIsNew();
    case CreateDate:
        return notification->GetCreateDate();
    case PlainTextBody:
        return notification->GetPlainTextBody();
    case HtmlTextBody:
        return notification->GetHtmlTextBody();
    case Url:
        return notification->GetUrl();
    case ResourceType:
        return notification->GetResourceType();
    case GroupResourceType:
        return notification->GetGroupResourceType();
    case ResourceId:
        return notification->GetResourceId();
    default:
        return QVariant();
    }
}

QHash<int, QByteArray> NotificationsModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[Actors] = "notificationActors";
    roles[IsNew] = "notificationIsNew";
    roles[CreateDate] = "notificationCreateDate";
    roles[PlainTextBody] = "notificationPlainText";
    roles[HtmlTextBody] = "notificationHtmlText";
    roles[Url] = "notificationUrl";
    roles[ResourceType] = "notificationResourceType";
    roles[GroupResourceType] = "notificationGroupResourceType";
    roles[ResourceId] = "notificationResourceId";
    return roles;
}

void NotificationsModel::fetchMoreContent()
{
    SailreadsManager::Instance()->loadNotifications(this, m_PageToken);
}

void NotificationsModel::updateNotifications()
{
    m_PageToken.clear();
    SailreadsManager::Instance()->loadNotifications(this, m_PageToken);
}

void NotificationsModel::handleGotNotifications(const QString& pageToken,
        const Notifications_t& notifications)
{
    if (m_PageToken.isEmpty()) {
        SetItems(notifications);
    }
    else {
        AddItems(notifications);
    }

    SetHasMore(!pageToken.isEmpty());
    if (m_HasMore) {
        m_PageToken = pageToken;
    }
}
} // namespace Sailreads
