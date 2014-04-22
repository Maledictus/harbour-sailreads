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

#include "friendsmodel.h"
#include <QtDebug>

namespace SailReads
{
	FriendsModel::FriendsModel (QObject *parent)
	: QAbstractListModel (parent)
	{
		RoleNames_ [FRID] = "friendID";
		RoleNames_ [FRName] = "friendName";
		RoleNames_ [FRProfileImageUrl] = "friendProfileImageUrl";
	}

	QHash<int, QByteArray> FriendsModel::roleNames () const
	{
		return RoleNames_;
	}

	int FriendsModel::rowCount(const QModelIndex&) const
	{
		return Friends_.count ();
	}

	QVariant FriendsModel::data (const QModelIndex& index, int role) const
	{
		if (index.row () > Friends_.count () - 1 || index.row () < 0)
			return QVariant ();

		const auto& friendItem = Friends_.at (index.row ());
		switch (role)
		{
		case FRID:
			return friendItem.ID_;
		case FRName:
			return friendItem.Name_;
		case FRProfileImageUrl:
			return friendItem.ProfileImageUrl_;
		default:
			return QVariant ();
		}
	}

	void FriendsModel::AddItems (const Friend& friendItem)
	{
		beginInsertRows (QModelIndex (), rowCount (), rowCount () + 1);
		Friends_.append (friendItem);
		endInsertRows ();
		emit countChanged ();
	}

	void FriendsModel::AddItems (const Friends_t& friends)
	{
		beginInsertRows (QModelIndex (), rowCount (), rowCount () + friends.count ());
		Friends_.append (friends);
		endInsertRows ();
		emit countChanged ();
	}

	void FriendsModel::Clear ()
	{
		beginResetModel ();
		Friends_.clear ();
		endResetModel ();
		emit countChanged ();
	}


}
