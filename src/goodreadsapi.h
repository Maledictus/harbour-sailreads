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
#include <QDomDocument>
#include <QNetworkReply>
#include <QQueue>
#include <QUrl>
#include "structures.h"
#include "userprofile.h"

namespace SailReads
{
	class OAuthWrapper;
	class NetworkAccessManager;

	class GoodreadsApi : public QObject
	{
		Q_OBJECT

		const QString ConsumerKey_;
		const QString ConsumerSecret_;
		const QUrl BaseUrl_;
		OAuthWrapper *OAuthWrapper_;
		NetworkAccessManager *NetworkAccessManager_;

		QQueue<SignedRequests> RequestsQueue_;

		bool RequestInProcess_;

	public:
		explicit GoodreadsApi (QObject *parent = 0);

		bool IsRequestInProcess () const;

		QUrl GetAuthorizationUrl ();
		QPair<QString, QString> GetAccessTokens () const;

		void RequestAuthUserID (const QString& accessToken,
				const QString& accessTokenSecret);
		void RequestUserInfo (const QString& id);
		void RequestFriendsUpdates (const QString& accessToken,
				const QString& accessTokenSecret);
		void RequestNotifications (const QString& accessToken,
				const QString& accessTokenSecret);
		void RequestFriends (const QString& accessToken,
				const QString& accessTokenSecret, const QString& id);
		void RequestGroups (const QString& id);

		void RequestShelves (const QString& id);
		void AddShelf (const QString& name, bool exclusive,
				const QString& accessToken, const QString& accessTokenSecret);
		void EditShelf (const QString& id, const QString& name, bool exclusive,
				const QString& accessToken, const QString& accessTokenSecret);

	private slots:
		void handleDownloadProgress (qint64 bytesReceived, qint64 bytesTotal);
		void handleReplyError (QNetworkReply::NetworkError error);
		void handleSignedRequestReady (const QByteArray& data);

		void handleRequestAuthUserIDFinished ();
		void handleRequestUserInfoFinished ();
		void handleRequestFriendsUpdatesFinished ();
		void handleRequestNotificationsFinished ();
		void handleRequestFriendsFinished ();
		void handleRequestGroupsFinished ();
		void handleRequestShelvesFinished ();
		void handleAddShelfFinished (const QDomDocument& document);
		void handleEditShelfFinished (const QDomDocument& document);

	signals:
		void requestInProcessChanged ();

		void gotAuthUserID (const QString& id);
		void gotUserProfile (UserProfile *profile);
		void gotRecentUpdates (const Updates_t& updates);
		void gotNotifications (const Notifications_t& notifications);
		void gotFriends (const Friends_t& friends);
		void gotGroups (const Groups_t& groups);
		void gotShelves (const Shelves_t& shelves);
		void gotNewShelf (const Shelf& shelf);
	};
}
