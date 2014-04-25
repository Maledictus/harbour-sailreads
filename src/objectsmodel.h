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

#include "basemodel.h"

namespace SailReads
{
	template<typename T> class ObjectsModel : public BaseModel
	{
	protected:
		QList<T> Objects_;

	public:
		ObjectsModel (QObject *parent = 0)
		: BaseModel (parent)
		{}

		virtual int rowCount (const QModelIndex& = QModelIndex ()) const
		{
			return Objects_.count ();
		}

		void AddItems (const T& object)
		{
			beginInsertRows (QModelIndex (), rowCount (), rowCount ());
			Objects_.append (object);
			endInsertRows ();
			emit countChanged ();
		}

		void AddItems (const QList<T>& objects)
		{
			beginInsertRows (QModelIndex (), rowCount (), rowCount () + objects.count ());
			Objects_.append (objects);
			endInsertRows ();
			emit countChanged ();
		}

		void Clear ()
		{
			beginResetModel ();
			Objects_.clear ();
			endResetModel ();
			emit countChanged ();
		}
	};
}
