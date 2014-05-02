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

#include "groupsmodel.h"

namespace SailReads {

	GroupsModel::GroupsModel (QObject *parent)
	: ObjectsModel<Group> (parent)
	{
		RoleNames_ [GRID] = "groupID";
		RoleNames_ [GRName] = "groupName";
		RoleNames_ [GRImageUrl] = "groupImageUrl";
		RoleNames_ [GRUsersCount] = "groupUsersCount";
		RoleNames_ [GRPublic] = "groupPublic";
	}

	QVariant GroupsModel::data (const QModelIndex& index, int role) const
	{
		if (index.row () > Objects_.count () - 1 || index.row () < 0)
			return QVariant ();

		const auto& group = Objects_.at (index.row ());
		switch (role)
		{
		case GRID:
			return group.ID_;
		case GRName:
			return group.Name_;
		case GRImageUrl:
			return group.ImageUrl_;
		case GRUsersCount:
			return group.UsersCount_;
		case GRPublic:
			return group.Public_;
		default:
			return QVariant ();
		}
	}
}
