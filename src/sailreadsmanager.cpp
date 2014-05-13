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

#include "sailreadsmanager.h"
#include <algorithm>
#include <QQmlContext>
#include <QQmlEngine>
#include <QQuickItem>
#include <QQuickView>
#include <QtDebug>
#include "friendsmodel.h"
#include "goodreadsapi.h"
#include "groupsmodel.h"
#include "localstorage.h"
#include "notificationsmodel.h"
#include "recentupdatesmodel.h"
#include "shelvesmodel.h"
#include "userprofile.h"

namespace SailReads
{
	SailreadsManager::SailreadsManager (QQuickView *view, QObject *parent)
	: QObject (parent)
	, MainView_ (view)
	, GoodreadsApi_ (new GoodreadsApi (this))
	, LocalStorage_ (new LocalStorage (this))
	, UpdatesModel_ (new RecentUpdatesModel (this))
	, NotificationsModel_ (new NotificationsModel (this))
	, FriendsModel_ (new FriendsModel (this))
	, GroupsModel_ (new GroupsModel (this))
	, SearchGroupsModel_ (new GroupsModel (this))
	, ShelvesModel_ (new ShelvesModel (this))
	{
		connect (GoodreadsApi_,
				SIGNAL (requestInProcessChanged ()),
				this,
				SIGNAL (requestInProcessChanged ()));
		connect (GoodreadsApi_,
				SIGNAL (gotAuthUserID (QString)),
				this,
				SLOT (handleGotAuthUserID (QString)));
		connect (GoodreadsApi_,
				SIGNAL (gotUserProfile (UserProfile*)),
				this,
				SLOT (handleGotUserProfile (UserProfile*)));
		connect (GoodreadsApi_,
				SIGNAL (gotRecentUpdates (Updates_t)),
				this,
				SLOT (handleGotRecentUpdates (Updates_t)));
		connect (GoodreadsApi_,
				SIGNAL (gotNotifications (Notifications_t)),
				this,
				SLOT (handleGotNotifications (Notifications_t)));
		connect (GoodreadsApi_,
				SIGNAL (gotFriends (Friends_t)),
				this,
				SLOT (handleGotFriends (Friends_t)));
		connect (GoodreadsApi_,
				SIGNAL (gotGroups (Groups_t)),
				this,
				SLOT (handleGotGroups (Groups_t)));
		connect (GoodreadsApi_,
				SIGNAL (gotFoundGroups (Groups_t)),
				this,
				SLOT (handleGotFoundGroups (Groups_t)));
		connect (GoodreadsApi_,
				SIGNAL (gotShelves (Shelves_t)),
				this,
				SLOT (handleGotShelves (Shelves_t)));
		connect (GoodreadsApi_,
				SIGNAL (gotNewShelf (Shelf)),
				this,
				SLOT (handleGotNewShelf (Shelf)));
		connect (GoodreadsApi_,
				SIGNAL (shelvesUpdated ()),
				this,
				SLOT (handleShelvesUpdated ()));
	}

	void SailreadsManager::Init ()
	{
		connect (MainView_->rootObject (),
				SIGNAL (applicationAuthorized (bool)),
				this,
				SLOT (handleApplicationAuthorized (bool)));
		connect (MainView_->rootObject (),
				SIGNAL (refreshUpdates ()),
				this,
				SLOT (handleRefreshUpdates ()));
		connect (MainView_->rootObject (),
				SIGNAL (requestUserProfile (QString)),
				this,
				SLOT (handleRequestUserProfile (QString)));
		connect (MainView_->rootObject (),
				SIGNAL (requestNotifications ()),
				this,
				SLOT (handleRequestNotifications ()));
		connect (MainView_->rootObject (),
				SIGNAL (requestFriendsList (QString)),
				this,
				SLOT (handleRequestFriendsList (QString)));
		connect (MainView_->rootObject (),
				SIGNAL (requestGroupsList (QString)),
				this,
				SLOT (handleRequestGroupsList (QString)));
		connect (MainView_->rootObject (),
				SIGNAL (requestSearchGroups (QString)),
				this,
				SLOT (handleRequestSearchGroups (QString)));
		connect (MainView_->rootObject (),
				SIGNAL (requestShelvesList (QString)),
				this,
				SLOT (handleRequestShelvesList (QString)));
		connect (MainView_->rootObject (),
				SIGNAL (requestAddBooksShelf (QString, bool)),
				this,
				SLOT (handleRequestAddBooksShelf (QString, bool)));
		connect (MainView_->rootObject (),
				SIGNAL (requestEditBooksShelf (QString, QString, bool)),
				this,
				SLOT (handleRequestEditBooksShelf (QString, QString, bool)));

		MainView_->rootContext ()->setContextProperty ("updatesModel", UpdatesModel_);
		MainView_->rootContext ()->setContextProperty ("notificationsModel", NotificationsModel_);
		MainView_->rootContext ()->setContextProperty ("friendsModel", FriendsModel_);
		MainView_->rootContext ()->setContextProperty ("groupsModel", GroupsModel_);
		MainView_->rootContext ()->setContextProperty ("searchGroupsModel", SearchGroupsModel_);
		MainView_->rootContext ()->setContextProperty ("shelvesModel", ShelvesModel_);

		AccessToken_ = LocalStorage_->GetValue ("AccessToken");
		AccessTokenSecret_ = LocalStorage_->GetValue ("AccessTokenSecret");

		if (AccessToken_.isEmpty () || AccessTokenSecret_.isEmpty ())
			AuthorizeApplication ();
		else
			RequestAuthUserId ();
	}

	bool SailreadsManager::IsRequestInProcess () const
	{
		return GoodreadsApi_->IsRequestInProcess ();
	}

	void SailreadsManager::AuthorizeApplication ()
	{
		int failTimes = 3;
		QUrl authUrl;
		while (failTimes && authUrl.isEmpty ())
		{
			authUrl = GoodreadsApi_->GetAuthorizationUrl ();
			if (authUrl.isEmpty ())
				--failTimes;
		}

		if (authUrl.isEmpty ())
		{
			//TODO make notifiacation
			qWarning () << Q_FUNC_INFO
					<< "Unbale to get authorization page. Please try later";
			return;
		}

		QMetaObject::invokeMethod (MainView_->rootObject (),
				"authorizeApplication",
				Q_ARG (QVariant, QVariant::fromValue (authUrl)));
	}

	void SailreadsManager::RequestAuthUserId ()
	{
		GoodreadsApi_->RequestAuthUserID (AccessToken_, AccessTokenSecret_);
	}

	void SailreadsManager::handleApplicationAuthorized (bool authorized)
	{
		if (authorized)
		{
			const auto& tokens = GoodreadsApi_->GetAccessTokens ();
			if (tokens.first.isEmpty () || tokens.second.isEmpty ())
			{
				//TODO notification
				qWarning () << Q_FUNC_INFO
						<<  "Unable to get access token and access token secret";
				return;
			}

			LocalStorage_->SetValue ("AccessToken", tokens.first);
			LocalStorage_->SetValue ("AccessTokenSecret", tokens.second);
			AccessToken_ = tokens.first;
			AccessTokenSecret_ = tokens.second;

			RequestAuthUserId ();
		}
	}

	void SailreadsManager::handleRefreshUpdates ()
	{
		UpdatesModel_->Clear();
		GoodreadsApi_->RequestFriendsUpdates (AccessToken_, AccessTokenSecret_);
	}

	void SailreadsManager::handleRequestUserProfile (const QString& id)
	{
		GoodreadsApi_->RequestUserInfo (id == "self" ? AuthUserID_ : id);
	}

	void SailreadsManager::handleRequestNotifications ()
	{
		NotificationsModel_->Clear ();
		GoodreadsApi_->RequestNotifications (AccessToken_, AccessTokenSecret_);
	}

	void SailreadsManager::handleRequestFriendsList (const QString& id)
	{
		FriendsModel_->Clear ();
		GoodreadsApi_->RequestFriends (AccessToken_,
				AccessTokenSecret_, id == "self" ? AuthUserID_ : id);
	}

	void SailreadsManager::handleRequestGroupsList (const QString& id)
	{
		GroupsModel_->Clear ();
		GoodreadsApi_->RequestGroups (id == "self" ? AuthUserID_ : id);
	}

	void SailreadsManager::handleRequestSearchGroups (const QString& query)
	{
		SearchGroupsModel_->Clear ();
		GoodreadsApi_->RequestSearchGroups (query);
	}

	void SailreadsManager::handleRequestShelvesList (const QString& id)
	{
		ShelvesModel_->Clear ();
		GoodreadsApi_->RequestShelves (id == "self" ? AuthUserID_ : id);
	}

	void SailreadsManager::handleRequestAddBooksShelf (const QString& name,
			bool exclusive)
	{
		GoodreadsApi_->AddShelf (name, exclusive, AccessToken_, AccessTokenSecret_);
	}

	void SailreadsManager::handleRequestEditBooksShelf (const QString& id,
			const QString& name, bool exclusive)
	{
		GoodreadsApi_->EditShelf (id, name, exclusive, AccessToken_,
				AccessTokenSecret_);
	}

	void SailreadsManager::handleGotAuthUserID (const QString& id)
	{
		AuthUserID_ = id;
		GoodreadsApi_->RequestFriendsUpdates (AccessToken_, AccessTokenSecret_);
	}

	void SailreadsManager::handleGotUserProfile (UserProfile *profile)
	{
		QMetaObject::invokeMethod (MainView_->rootObject (),
				"setUserProfile",
				Q_ARG (QVariant, QVariant::fromValue<QObject*> (profile)));
		MainView_->engine ()->setObjectOwnership (profile, QQmlEngine::CppOwnership);
		handleGotShelves (profile->getShelves ());
		profile->deleteLater ();
	}

	void SailreadsManager::handleGotRecentUpdates (const Updates_t& updates)
	{
		UpdatesModel_->Clear ();
		UpdatesModel_->AddItems (updates);
	}

	void SailreadsManager::handleGotNotifications (const Notifications_t& notifications)
	{
		int newNotificationsCount = std::count_if (notifications.begin (), notifications.end (),
				[] (decltype (notifications.first ()) notification)
				{
					return !notification.Read_;
				});
		QMetaObject::invokeMethod (MainView_->rootObject (),
				"setUnreadNotificationsCount",
				Q_ARG (QVariant, QVariant::fromValue (newNotificationsCount)));

		if (newNotificationsCount)
		{
			//TODO
			qDebug () << "you have unread notificaion";
		}

		NotificationsModel_->Clear ();
		NotificationsModel_->AddItems (notifications);
	}

	void SailreadsManager::handleGotFriends (const Friends_t& friends)
	{
		FriendsModel_->Clear ();
		FriendsModel_->AddItems (friends);
	}

	void SailreadsManager::handleGotGroups (const Groups_t& groups)
	{
		GroupsModel_->Clear ();
		GroupsModel_->AddItems (groups);
	}

	void SailreadsManager::handleGotFoundGroups(const Groups_t& groups)
	{
		SearchGroupsModel_->Clear ();
		SearchGroupsModel_->AddItems (groups);
	}

	void SailreadsManager::handleGotShelves (const Shelves_t& shelves)
	{
		ShelvesModel_->Clear ();
		ShelvesModel_->AddItems (shelves);
	}

	void SailreadsManager::handleGotNewShelf (const Shelf& shelf)
	{
		ShelvesModel_->AddItems (shelf);
	}

	void SailreadsManager::handleShelvesUpdated ()
	{
		handleRequestShelvesList ("self");
	}
}
