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

#include "recentupdatesmodel.h"
#include <QtDebug>

namespace SailReads
{
	RecentUpdatesModel::RecentUpdatesModel (QObject *parent)
	: QAbstractListModel (parent)
	{
		RoleNames_ [URDate] = "updateDate";
		RoleNames_ [URLink] = "updateLink";
		RoleNames_ [URActionText] = "updateActionText";
		RoleNames_ [URActorID] = "updateActorID";
		RoleNames_ [URActorName] = "updateActorName";
		RoleNames_ [URActorPorfileImage] = "updateActorProfileImage";
		RoleNames_ [URActorProfileUrl] = "updateActorProfileUrl";
	}

	QHash<int, QByteArray> RecentUpdatesModel::roleNames () const
	{
		return RoleNames_;
	}

	int RecentUpdatesModel::rowCount(const QModelIndex&) const
	{
		return Updates_.count ();
	}

	QVariant RecentUpdatesModel::data (const QModelIndex& index, int role) const
	{
		if (index.row () > Updates_.count () - 1 || index.row () < 0)
			return QVariant ();

		const auto& update = Updates_.at (index.row ());
		switch (role)
		{
		case URDate:
			return update.Date_;
		case URLink:
			return update.Link_;
		case URActionText:
			return update.ActionText_;
		case URActorID:
			return update.ActorID_;
		case URActorName:
			return update.ActorName_;
		case URActorPorfileImage:
			return update.ActorProfileImage_;
		case URActorProfileUrl:
			return update.ActorProfileUrl_;
		default:
			return QVariant ();
		}
	}

	void RecentUpdatesModel::AddItems (const Update& update)
	{
		beginInsertRows (QModelIndex (), rowCount (), rowCount () + 1);
		Updates_.append (update);
		endInsertRows ();
		emit countChanged ();
	}

	void RecentUpdatesModel::AddItems (const Updates_t& updates)
	{
		beginInsertRows (QModelIndex (), rowCount (), rowCount () + updates.count ());
		Updates_.append (updates);
		endInsertRows ();
		emit countChanged ();
	}

	void RecentUpdatesModel::Clear ()
	{
		beginResetModel ();
		Updates_.clear ();
		endResetModel ();
		emit countChanged ();
	}


}
