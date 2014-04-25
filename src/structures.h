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

#include <QDateTime>
#include <QString>
#include <QStringList>
#include <QUrl>

namespace SailReads
{
	struct Shelf
	{
		QString ID_;
		QString Name_;
		quint32 BookCount_;
		QString Description_;
		QString DisplayFields_;
		bool ExclusiveFlag_;
		bool Featured_;

		Shelf ()
		: ExclusiveFlag_ (false)
		, Featured_ (false)
		{}
	};

	struct Update
	{
		enum class Type
		{
			NoType,
			ReadStatus,
			Review
		};

		Type Type_;
		QString ActionText_;
		QUrl Link_;
		QDateTime Date_;

		QString ActorID_;
		QString ActorName_;
		QUrl ActorProfileImage_;
		QUrl ActorProfileUrl_;

		Update ()
		: Type_ (Type::NoType)
		{}
	};

	typedef QList<Update> Updates_t;

	struct Notification
	{
		bool Read_;
		QDateTime Date_;
		QUrl Link_;
		QString Text_;
		QString ActorID_;
		QString ActorName_;
		QUrl ActorProfileUrl_;
		QUrl ActorProfileImageUrl_;

		Notification ()
		: Read_ (false)
		{}
	};

	typedef QList<Notification> Notifications_t;

	struct Friend
	{
		QString ID_;
		QString Name_;
		QUrl ProfileImageUrl_;
	};

	typedef QList<Friend> Friends_t;

	struct Group
	{

	};

	typedef QList<Group> Groups_t;
}

Q_DECLARE_METATYPE (SailReads::Update)
Q_DECLARE_METATYPE (SailReads::Updates_t)
