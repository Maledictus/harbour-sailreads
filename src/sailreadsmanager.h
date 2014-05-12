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
#include "structures.h"

class QQuickView;

namespace SailReads
{
	class FriendsModel;
	class GoodreadsApi;
	class GroupsModel;
	class LocalStorage;
	class NotificationsModel;
	class RecentUpdatesModel;
	class ShelvesModel;
	class UserProfile;

	class SailreadsManager : public QObject
	{
		Q_OBJECT

		QQuickView *MainView_;
		GoodreadsApi *GoodreadsApi_;
		LocalStorage *LocalStorage_;

		QString AccessToken_;
		QString AccessTokenSecret_;
		QString AuthUserID_;

		RecentUpdatesModel *UpdatesModel_;
		NotificationsModel *NotificationsModel_;
		FriendsModel *FriendsModel_;
		GroupsModel *GroupsModel_;
		ShelvesModel *ShelvesModel_;

		Q_PROPERTY (bool requestInProcess READ IsRequestInProcess NOTIFY requestInProcessChanged)

	public:
		explicit SailreadsManager (QQuickView *view, QObject *parent = 0);

		void Init ();
		bool IsRequestInProcess () const;
	private:
		void AuthorizeApplication ();
		void RequestAuthUserId ();

	private slots:
		void handleApplicationAuthorized (bool authorized);
		void handleRefreshUpdates ();
		void handleRequestUserProfile (const QString& id);
		void handleRequestNotifications ();
		void handleRequestFriendsList (const QString& id);
		void handleRequestGroupsList (const QString& id);
		void handleRequestShelvesList (const QString& id);
		void handleRequestAddBooksShelf (const QString& name, bool exclusive);
		void handleRequestEditBooksShelf (const QString& id, const QString& name,
				bool exclusive);

		void handleGotAuthUserID (const QString& id);
		void handleGotUserProfile (UserProfile *profile);
		void handleGotRecentUpdates (const Updates_t& updates);
		void handleGotNotifications (const Notifications_t& notifications);
		void handleGotFriends (const Friends_t& friends);
		void handleGotGroups (const Groups_t& groups);
		void handleGotShelves (const Shelves_t& shelves);
		void handleGotNewShelf (const Shelf& shelf);

	signals:
		void requestInProcessChanged ();
	};
}
