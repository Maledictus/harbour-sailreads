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

#include "shelvesmodel.h"

namespace SailReads
{
	ShelvesModel::ShelvesModel (QObject *parent)
	: ObjectsModel<Shelf> (parent)
	{
		RoleNames_ [SRID] = "shelfID";
		RoleNames_ [SRName] = "shelfName";
		RoleNames_ [SRBooksCount] = "shelfBooksCount";
		RoleNames_ [SRDescription] = "shelfDescription";
	}

	QVariant ShelvesModel::data (const QModelIndex& index, int role) const
	{
		if (index.row () > Objects_.count () - 1 || index.row () < 0)
			return QVariant ();

		const auto& shelf = Objects_.at (index.row ());
		switch (role)
		{
		case SRID:
			return shelf.ID_;
		case SRName:
			return shelf.Name_;
		case SRBooksCount:
			return shelf.BooksCount_;
		case SRDescription:
			return shelf.Description_;
		default:
			return QVariant ();
		}
	}
}
