/*
Copyright (c) 2014 Oleg Linkin <maledictusdemagog@gmail.com>

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
#include <QtDebug>

namespace SailReads
{
	NotificationsModel::NotificationsModel (QObject *parent)
	: QAbstractListModel (parent)
	{
		RoleNames_ [NRDate] = "notificationDate";
		RoleNames_ [NRRead] = "notificationRead";
		RoleNames_ [NRLink] = "notificationLink";
		RoleNames_ [NRText] = "notificationText";
		RoleNames_ [NRActorID] = "notificationActorID";
		RoleNames_ [NRActorName] = "notificationActorName";
		RoleNames_ [NRActorPorfileImage] = "notificationActorProfileImage";
		RoleNames_ [NRActorProfileUrl] = "notificationActorProfileUrl";
	}

	QHash<int, QByteArray> NotificationsModel::roleNames () const
	{
		return RoleNames_;
	}

	int NotificationsModel::rowCount (const QModelIndex&) const
	{
		return Notifications_.count ();
	}

	QVariant NotificationsModel::data (const QModelIndex& index, int role) const
	{
		if (index.row () > Notifications_.count () - 1 || index.row () < 0)
			return QVariant ();

		const auto& notification = Notifications_.at (index.row ());
		switch (role)
		{
		case NRDate:
			return notification.Date_;
		case NRRead:
			return notification.Read_;
		case NRLink:
			return notification.Link_;
		case NRText:
			return notification.Text_;
		case NRActorID:
			return notification.ActorID_;
		case NRActorName:
			return notification.ActorName_;
		case NRActorPorfileImage:
			return notification.ActorProfileImageUrl_;
		case NRActorProfileUrl:
			return notification.ActorProfileUrl_;
		default:
			return QVariant ();
		}
	}

	void NotificationsModel::AddItems (const Notification& notification)
	{
		beginInsertRows (QModelIndex (), rowCount (), rowCount () + 1);
		Notifications_.append (notification);
		endInsertRows ();
		emit countChanged ();
	}

	void NotificationsModel::AddItems (const Notifications_t& notifications)
	{
		beginInsertRows (QModelIndex (), rowCount (), rowCount () + notifications.count ());
		Notifications_.append (notifications);
		endInsertRows ();
		emit countChanged ();
	}

	void NotificationsModel::Clear ()
	{
		beginResetModel ();
		Notifications_.clear ();
		endResetModel ();
		emit countChanged ();
	}


}
