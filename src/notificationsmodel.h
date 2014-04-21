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

#pragma once

#include <QAbstractListModel>
#include "structures.h"

namespace SailReads
{
	class NotificationsModel : public QAbstractListModel
	{
		Q_OBJECT

		QHash<int, QByteArray> RoleNames_;
		Notifications_t Notifications_;

		Q_PROPERTY (int count READ rowCount NOTIFY countChanged)
	public:
		enum NotificationRoles
		{
			NRDate = Qt::UserRole + 1,
			NRRead,
			NRLink,
			NRText,
			NRActorID,
			NRActorName,
			NRActorPorfileImage,
			NRActorProfileUrl
		};


		explicit NotificationsModel(QObject *parent = 0);

		virtual QHash<int, QByteArray> roleNames () const;
		virtual int rowCount (const QModelIndex& parent = QModelIndex ()) const;
		virtual QVariant data (const QModelIndex& index, int role = Qt::DisplayRole) const;

		void AddItems (const Notification& notification);
		void AddItems (const Notifications_t& notifications);
		void Clear ();

	signals:
		void countChanged ();
	};
}
