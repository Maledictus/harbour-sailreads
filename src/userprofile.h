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

#include <QObject>
#include <QUrl>

namespace SailReads
{
	class UserProfile : public QObject
	{
		Q_OBJECT

		QString ID_;
		QString Name_;
		QString Nickname_;
		QUrl ProfileUrl_;
		QUrl ProfileImageUrl_;
		QString About_;
		quint32 Age_;
		QString Gender_;
		QString Location_;
		QUrl Website_;
		QString Interests_;
		quint32 FriendsCount_;
		quint32 GroupsCount_;
		quint32 BooksCount_;
		bool PrivateProfile_;

		Q_PROPERTY (QString id READ getID)
		Q_PROPERTY (QString name READ getName)
		Q_PROPERTY (QString nickname READ getNickname)
		Q_PROPERTY (QUrl userProfileUrl READ getUserProfileUrl)
		Q_PROPERTY (QUrl userImageUrl READ getUserImageUrl)
		Q_PROPERTY (QString about READ getAbout)
		Q_PROPERTY (quint32 age READ getAge)
		Q_PROPERTY (QString gender READ getGender)
		Q_PROPERTY (QString location READ getLocation)
		Q_PROPERTY (QUrl website READ getWebsite)
		Q_PROPERTY (QString interests READ getInterests)
		Q_PROPERTY (quint32 friendsCount READ getFriendsCount)
		Q_PROPERTY (quint32 groupsCount READ getGroupsCount)
		Q_PROPERTY (quint32 booksCount READ getBooksCount)
		Q_PROPERTY (bool privateProfile READ getPrivateProfile)
	public:
		explicit UserProfile(QObject *parent = 0);

		QString getID () const;
		void setID (const QString& id);
		QString getName () const;
		void setName (const QString& name);
		QString getNickname () const;
		void setNickname (const QString& nickname);
		QUrl getUserProfileUrl () const;
		void setUserProfileUrl (const QUrl& url);
		QUrl getUserImageUrl () const;
		void setUserImageUrl (const QUrl& url);
		QString getAbout () const;
		void setAbout (const QString& about);
		quint32 getAge () const;
		void setAge (quint32 age);
		QString getGender () const;
		void setGender (const QString& gender);
		QString getLocation () const;
		void setLocation (const QString& location);
		QUrl getWebsite () const;
		void setWebsite (const QUrl& website);
		QString getInterests () const;
		void setInterests (const QString& interests);
		quint32 getFriendsCount () const;
		void setFriendsCount (quint32 friendsCount);
		quint32 getGroupsCount () const;
		void setGroupsCount (quint32 groupsCount);
		quint32 getBooksCount () const;
		void setBooksCount (quint32 booksCount);
		bool getPrivateProfile () const;
		void setPrivateProfile (bool privateProfile);
	};
}
