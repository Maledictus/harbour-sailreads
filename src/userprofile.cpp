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

#include "userprofile.h"

namespace SailReads
{
	UserProfile::UserProfile (QObject *parent)
	: QObject (parent)
	, Age_ (0)
	, FriendsCount_ (0)
	, GroupsCount_ (0)
	, BooksCount_ (0)
	, PrivateProfile_ (false)
	{
	}

	QString UserProfile::getID () const
	{
		return ID_;
	}

	void UserProfile::setID (const QString& id)
	{
		ID_ = id;
	}

	QString UserProfile::getName () const
	{
		return Name_;
	}

	void UserProfile::setName (const QString& name)
	{
		Name_ = name;
	}

	QString UserProfile::getNickname () const
	{
		return Nickname_;
	}

	void UserProfile::setNickname (const QString& nickname)
	{
		Nickname_ = nickname;
	}

	QUrl UserProfile::getUserProfileUrl () const
	{
		return ProfileUrl_;
	}

	void UserProfile::setUserProfileUrl (const QUrl& url)
	{
		ProfileUrl_ = url;
	}

	QUrl UserProfile::getUserImageUrl () const
	{
		return ProfileImageUrl_;
	}

	void UserProfile::setUserImageUrl (const QUrl& url)
	{
		ProfileImageUrl_ = url;
	}

	QString UserProfile::getAbout () const
	{
		return About_;
	}

	void UserProfile::setAbout (const QString& about)
	{
		About_ = about;
	}

	quint32 UserProfile::getAge () const
	{
		return Age_;
	}

	void UserProfile::setAge (quint32 age)
	{
		Age_ = age;
	}

	QString UserProfile::getGender () const
	{
		return Gender_;
	}

	void UserProfile::setGender (const QString& gender)
	{
		Gender_ = gender;
	}

	QString UserProfile::getLocation () const
	{
		return Location_;
	}

	void UserProfile::setLocation (const QString& location)
	{
		Location_ = location;
	}

	QUrl UserProfile::getWebsite () const
	{
		return Website_;
	}

	void UserProfile::setWebsite (const QUrl& website)
	{
		Website_ = website;
	}

	QString UserProfile::getInterests () const
	{
		return Interests_;
	}

	void UserProfile::setInterests (const QString& interests)
	{
		Interests_ = interests;
	}

	quint32 UserProfile::getFriendsCount () const
	{
		return FriendsCount_;
	}

	void UserProfile::setFriendsCount (quint32 friendsCount)
	{
		FriendsCount_ = friendsCount;
	}

	quint32 UserProfile::getGroupsCount () const
	{
		return GroupsCount_;
	}

	void UserProfile::setGroupsCount (quint32 groupsCount)
	{
		GroupsCount_ = groupsCount;
	}

	quint32 UserProfile::getBooksCount () const
	{
		return BooksCount_;
	}

	void UserProfile::setBooksCount (quint32 booksCount)
	{
		BooksCount_ = booksCount;
	}

	bool UserProfile::getPrivateProfile () const
	{
		return PrivateProfile_;
	}

	void UserProfile::setPrivateProfile (bool privateProfile)
	{
		PrivateProfile_ = privateProfile;
	}
}
