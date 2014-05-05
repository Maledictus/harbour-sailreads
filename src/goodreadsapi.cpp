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

#include "goodreadsapi.h"
#include <stdexcept>
#include <QDomDocument>
#include <QDomElement>
#include <QDomNode>
#include <QNetworkRequest>
#include "networkaccessmanager.h"
#include "oauthwrapper.h"

namespace SailReads
{
	GoodreadsApi::GoodreadsApi (QObject *parent)
	: QObject (parent)
	, ConsumerKey_ ("GRGhcLIXU8M8u1NcnoMVFg")
	, ConsumerSecret_ ("Epo3MYIT3V1JcuC6OkZxptHLEuD8yqUgAj7mLNw")
	, BaseUrl_ ("http://www.goodreads.com")
	, OAuthWrapper_ (new OAuthWrapper (ConsumerKey_, ConsumerSecret_, BaseUrl_, this))
	, NetworkAccessManager_ (new NetworkAccessManager (this))
	{
	}

	bool GoodreadsApi::IsRequestInProcess () const
	{
		return RequestInProcess_;
	}

	QUrl GoodreadsApi::GetAuthorizationUrl ()
	{
		return OAuthWrapper_->GetAuthorizationUrl ();
	}

	QPair<QString, QString> GoodreadsApi::GetAccessTokens () const
	{
		return OAuthWrapper_->GetAccessTokens ();
	}

	void GoodreadsApi::RequestAuthUserID (const QString& accessToken,
			const QString& accessTokenSecret)
	{
		const auto& url = OAuthWrapper_->MakeGetSignedUrl ({ ConsumerKey_,
				ConsumerSecret_, accessToken, accessTokenSecret,
				QUrl ("https://www.goodreads.com/api/auth_user") });
		RequestInProcess_ = true;
		emit requestInProcessChanged ();
		auto reply = NetworkAccessManager_->get (QNetworkRequest (url));
		connect (reply,
				SIGNAL (downloadProgress (qint64, qint64)),
				this,
				SLOT (handleDownloadProgress (qint64, qint64)));
		connect (reply,
				SIGNAL (error (QNetworkReply::NetworkError)),
				this,
				SLOT (handleReplyError (QNetworkReply::NetworkError)));
		connect (reply,
				SIGNAL (finished ()),
				this,
				SLOT (handleRequestAuthUserIDFinished ()));
	}

	void GoodreadsApi::RequestUserInfo (const QString& id)
	{
		QUrl url (QString ("https://www.goodreads.com/user/show/%1.xml?key=%2")
				.arg (id)
				.arg (ConsumerKey_));
		RequestInProcess_ = true;
		emit requestInProcessChanged ();
		auto reply = NetworkAccessManager_->get (QNetworkRequest (url));
		connect (reply,
				SIGNAL (downloadProgress (qint64, qint64)),
				this,
				SLOT (handleDownloadProgress (qint64, qint64)));
		connect (reply,
				SIGNAL (error (QNetworkReply::NetworkError)),
				this,
				SLOT (handleReplyError (QNetworkReply::NetworkError)));
		connect (reply,
				SIGNAL (finished ()),
				this,
				SLOT (handleRequestUserInfoFinished ()));
	}

	void GoodreadsApi::RequestFriendsUpdates (const QString& accessToken,
			const QString& accessTokenSecret)
	{
		const auto& url = OAuthWrapper_->MakeGetSignedUrl ({ ConsumerKey_,
				ConsumerSecret_, accessToken, accessTokenSecret,
				QUrl ("https://www.goodreads.com/updates/friends.xml") });
		RequestInProcess_ = true;
		emit requestInProcessChanged ();
		auto reply = NetworkAccessManager_->get (QNetworkRequest (url));
		connect (reply,
				SIGNAL (downloadProgress (qint64, qint64)),
				this,
				SLOT (handleDownloadProgress (qint64, qint64)));
		connect (reply,
				SIGNAL (error (QNetworkReply::NetworkError)),
				this,
				SLOT (handleReplyError (QNetworkReply::NetworkError)));
		connect (reply,
				SIGNAL (finished ()),
				this,
				SLOT (handleRequestFriendsUpdatesFinished ()));
	}

	void GoodreadsApi::RequestNotifications (const QString& accessToken,
			const QString& accessTokenSecret)
	{
		const auto& url = OAuthWrapper_->MakeGetSignedUrl ({ ConsumerKey_,
				ConsumerSecret_, accessToken, accessTokenSecret,
				QUrl ("https://www.goodreads.com/notifications?format=xml") });

		RequestInProcess_ = true;
		emit requestInProcessChanged ();
		auto reply = NetworkAccessManager_->get (QNetworkRequest (url));
		connect (reply,
				SIGNAL (downloadProgress (qint64, qint64)),
				this,
				SLOT (handleDownloadProgress (qint64, qint64)));
		connect (reply,
				SIGNAL (error (QNetworkReply::NetworkError)),
				this,
				SLOT (handleReplyError (QNetworkReply::NetworkError)));
		connect (reply,
				SIGNAL (finished ()),
				this,
				SLOT (handleRequestNotificationsFinished ()));
	}

	void GoodreadsApi::RequestFriends (const QString& accessToken,
			const QString& accessTokenSecret, const QString& id)
	{
		const QUrl url (QString ("https://www.goodreads.com/friend/user/%1?format=xml")
				.arg (id));
		const auto& signedUrl = OAuthWrapper_->MakeGetSignedUrl ({ ConsumerKey_,
				ConsumerSecret_, accessToken, accessTokenSecret,
				url });
		RequestInProcess_ = true;
		emit requestInProcessChanged ();
		auto reply = NetworkAccessManager_->get (QNetworkRequest (signedUrl));
		connect (reply,
				SIGNAL (downloadProgress (qint64, qint64)),
				this,
				SLOT (handleDownloadProgress (qint64, qint64)));
		connect (reply,
				SIGNAL (error (QNetworkReply::NetworkError)),
				this,
				SLOT (handleReplyError (QNetworkReply::NetworkError)));
		connect (reply,
				SIGNAL (finished ()),
				this,
				SLOT (handleRequestFriendsFinished ()));
	}

	void GoodreadsApi::RequestGroups (const QString& id)
	{
		QUrl url (QString ("https://www.goodreads.com/group/list/%1.xml?key=%2")
				.arg (id)
				.arg (ConsumerKey_));
		RequestInProcess_ = true;
		emit requestInProcessChanged ();
		auto reply = NetworkAccessManager_->get (QNetworkRequest (url));
		connect (reply,
				SIGNAL (downloadProgress (qint64, qint64)),
				this,
				SLOT (handleDownloadProgress (qint64, qint64)));
		connect (reply,
				SIGNAL (error (QNetworkReply::NetworkError)),
				this,
				SLOT (handleReplyError (QNetworkReply::NetworkError)));
		connect (reply,
				SIGNAL (finished ()),
				this,
				SLOT (handleRequestGroupsFinished ()));
	}

	void GoodreadsApi::RequestShelves (const QString& id)
	{
		QUrl url (QString ("https://www.goodreads.com/shelf/list.xml?key=%1&user_id=%2")
				.arg (ConsumerKey_)
				.arg (id));
		RequestInProcess_ = true;
		emit requestInProcessChanged ();
		auto reply = NetworkAccessManager_->get (QNetworkRequest (url));
		connect (reply,
				SIGNAL (downloadProgress (qint64, qint64)),
				this,
				SLOT (handleDownloadProgress (qint64, qint64)));
		connect (reply,
				SIGNAL (error (QNetworkReply::NetworkError)),
				this,
				SLOT (handleReplyError (QNetworkReply::NetworkError)));
		connect (reply,
				SIGNAL (finished ()),
				this,
				 SLOT (handleRequestShelvesFinished ()));
	}

	void GoodreadsApi::AddShelf (const QString& name, bool exclusive,
			bool sortable, bool featured, const QString& accessToken,
			const QString& accessTokenSecret)
	{
		const auto& url = OAuthWrapper_->MakeGetSignedUrl ({ ConsumerKey_,
				ConsumerSecret_, accessToken, accessTokenSecret,
				QUrl ("https://www.goodreads.com/user_shelves.xml") });
		RequestInProcess_ = true;
		emit requestInProcessChanged ();

		QUrlQuery params;
		params.addQueryItem ("user_shelf[name]", name);
		params.addQueryItem ("user_shelf[exclusive_flag]", exclusive ? "true" : "false");
		params.addQueryItem ("user_shelf[sortable_flag]", sortable ? "true" : "false");
		params.addQueryItem ("user_shelf[featured]", featured ? "true" : "false");

		QNetworkRequest request (url);
		request.setHeader (QNetworkRequest::ContentTypeHeader,
				"application/x-www-form-urlencoded");
		auto reply = NetworkAccessManager_->post (request, params.toString ().toUtf8 ());
		connect (reply,
				SIGNAL (downloadProgress (qint64, qint64)),
				this,
				SLOT (handleDownloadProgress (qint64, qint64)));
		connect (reply,
				SIGNAL (error (QNetworkReply::NetworkError)),
				this,
				SLOT (handleReplyError (QNetworkReply::NetworkError)));
		connect (reply,
				SIGNAL (finished ()),
				this,
				SLOT (handleAddShelfFinished ()));
	}

	void GoodreadsApi::EditShelf (const QString& id, const QString& name,
			bool exclusive, bool sortable, bool featured, const QString& accessToken,
			const QString& accessTokenSecret)
	{
		const QUrl url (QString ("https://www.goodreads.com/user_shelves/update.xml?id=%1")
				.arg (id));
		const auto& signedUrl = OAuthWrapper_->MakeGetSignedUrl ({ ConsumerKey_,
				ConsumerSecret_, accessToken, accessTokenSecret,
				url });
		qDebug () << signedUrl << accessToken << accessTokenSecret;
		QUrlQuery params;
		params.addQueryItem ("name", name);
		params.addQueryItem ("exclusive_flag", exclusive ? "true" : "false");
		params.addQueryItem ("sortable_flag", sortable ? "true" : "false");
		params.addQueryItem ("featured", featured ? "true" : "false");
		RequestInProcess_ = true;
		emit requestInProcessChanged ();
		QNetworkRequest request (signedUrl);
		request.setHeader (QNetworkRequest::ContentTypeHeader,
				"application/x-www-form-urlencoded");
		auto reply = NetworkAccessManager_->post (request, params.toString ().toUtf8 ());
		connect (reply,
				SIGNAL (downloadProgress (qint64, qint64)),
				this,
				SLOT (handleDownloadProgress (qint64, qint64)));
		connect (reply,
				SIGNAL (error (QNetworkReply::NetworkError)),
				this,
				SLOT (handleReplyError (QNetworkReply::NetworkError)));
		connect (reply,
				SIGNAL (finished ()),
				this,
				SLOT (handleEditShelfFinished ()));
	}

	void GoodreadsApi::handleDownloadProgress(qint64 bytesReceived, qint64 bytesTotal)
	{
		RequestInProcess_ = (bytesReceived != bytesTotal);
		emit requestInProcessChanged ();
	}

	void GoodreadsApi::handleReplyError (QNetworkReply::NetworkError error)
	{
		auto reply = qobject_cast<QNetworkReply*> (sender ());
		if (!reply)
			return;

		reply->deleteLater ();
		qWarning () << Q_FUNC_INFO
				<< "Networ error:"
				<< error
				<< reply->errorString ();
	}

	namespace
	{
		bool FillDomDocument (const QByteArray& data, QDomDocument &document)
		{
			QString errorMsg;
			int errorLine = -1, errorColumn = -1;
			if (!document.setContent (data, &errorMsg, &errorLine, &errorColumn))
			{
				qWarning () << Q_FUNC_INFO
						<< errorMsg
						<< "in line:"
						<< errorLine
						<< "column:"
						<< errorColumn;
				return false;
			}

			return true;
		}
	}

	void GoodreadsApi::handleRequestAuthUserIDFinished ()
	{
		auto reply = qobject_cast<QNetworkReply*> (sender ());
		if (!reply)
			return;

		QDomDocument document;
		reply->deleteLater ();
		if (!FillDomDocument (reply->readAll (), document))
		{
			qWarning () << Q_FUNC_INFO
					<< "unable to get auth user id";
			return;
		}

		const auto& userTags = document.elementsByTagName ("user");
		if (!userTags.isEmpty ())
		{
			const auto& element = userTags.at (0).toElement ();
			if (element.hasAttribute ("id"))
			{
				const auto& userId = element.attribute ("id");
				if (!userId.isEmpty ())
					emit gotAuthUserID (userId);
			}
		}
	}

	namespace
	{
		Shelf CreateShelf (const QDomElement& shelfElement)
		{
			Shelf shelf;
			const auto& fieldsList = shelfElement.childNodes ();
			for (int i = 0, fieldsCount = fieldsList.size (); i < fieldsCount; ++i)
			{
				const auto& fieldElement = fieldsList.at (i).toElement ();
				if (fieldElement.tagName () == "id")
					shelf.ID_ = fieldElement.text ();
				else if (fieldElement.tagName () == "name")
					shelf.Name_ = fieldElement.text ();
				else if (fieldElement.tagName () == "book_count")
					shelf.BooksCount_ = fieldElement.text ().toUInt ();
				else if (fieldElement.tagName () == "description")
					shelf.Description_ = fieldElement.text ();
				else if (fieldElement.tagName () == "exclusive_flag")
					shelf.Exclusive_ = fieldElement.text () == "true";
				else if (fieldElement.tagName () == "featured")
					shelf.Featured_ = fieldElement.text () == "true";
				else if (fieldElement.tagName () == "sort")
					shelf.Sortable_ = !(fieldElement.hasAttribute("nil") &&
							fieldElement.attribute ("nil") == "true");
			}

			return shelf;
		}

		Update CreateUpdate (const QDomElement& updateElement)
		{
			Update update;
			if (updateElement.hasAttribute ("type"))
			{
				if (updateElement.attribute ("type") == "readstatus")
					update.Type_ = Update::Type::ReadStatus;
				else if (updateElement.attribute ("type") == "review")
					update.Type_ = Update::Type::Review;
			}

			const auto& fieldsList = updateElement.childNodes ();
			for (int i = 0, fieldsCount = fieldsList.size (); i < fieldsCount; ++i)
			{
				const auto& fieldElement = fieldsList.at (i).toElement ();
				if (fieldElement.tagName () == "action_text")
					update.ActionText_ = fieldElement.text ();
				else if (fieldElement.tagName () == "link")
					update.Link_ = QUrl (fieldElement.text ());
				else if (fieldElement.tagName () == "updated_at")
				{
					const auto& dateTime = fieldElement.text().left (25);
					update.Date_ = QDateTime::fromString (dateTime,
							"ddd, dd MMM yyyy hh:mm:ss");
				}
				else if (fieldElement.tagName () == "actor")
				{
					const auto& actorFieldsList = fieldElement.childNodes ();
					for (int j = 0, actorFieldsCount = actorFieldsList.size (); j < actorFieldsCount; ++j)
					{
						const auto& actorFieldElement = actorFieldsList.at (j)
								.toElement ();
						if (actorFieldElement.tagName () == "id")
							update.ActorID_ = actorFieldElement.text ();
						else if (actorFieldElement.tagName () == "name")
							update.ActorName_ = actorFieldElement.text ();
						else if (actorFieldElement.tagName () == "image_url")
							update.ActorProfileImage_ = QUrl (actorFieldElement.text ());
						else if (actorFieldElement.tagName () == "link")
							update.ActorProfileUrl_ = QUrl (actorFieldElement.text ());
					}
				}
			}

			return update;
		}

		UserProfile* CreateUserProfile (const QDomDocument& doc)
		{
			UserProfile *profile = new UserProfile;
			const auto& responseElement = doc.firstChildElement("GoodreadsResponse");
			if (responseElement.isNull ())
				return profile;

			const auto& userElement = responseElement.firstChildElement ("user");
			const auto& fieldsList = userElement.childNodes ();
			for (int i = 0, fieldsCount = fieldsList.size (); i < fieldsCount; ++i)
			{
				const auto& fieldElement = fieldsList.at (i).toElement ();
				if (fieldElement.tagName () == "id")
					profile->setID (fieldElement.text ());
				else if (fieldElement.tagName () == "name")
					profile->setName (fieldElement.text ());
				else if (fieldElement.tagName () == "user_name")
					profile->setNickname (fieldElement.text ());
				else if (fieldElement.tagName () == "link")
					profile->setUserImageUrl(QUrl (fieldElement.firstChild ()
							.toCDATASection ().data ()));
				else if (fieldElement.tagName () == "image_url")
					profile->setUserProfileUrl (QUrl (fieldElement.firstChild ()
							.toCDATASection ().data ()));
				else if (fieldElement.tagName () == "about")
					profile->setAbout (fieldElement.text ());
				else if (fieldElement.tagName () == "age")
					profile->setAge (fieldElement.text ().toUInt ());
				else if (fieldElement.tagName () == "gender")
					profile->setGender (fieldElement.text ());
				else if (fieldElement.tagName () == "location")
					profile->setLocation (fieldElement.text ());
				else if (fieldElement.tagName () == "website")
					profile->setWebsite (QUrl (fieldElement.firstChild ()
							.toCDATASection ().data ()));
				else if (fieldElement.tagName () == "interests")
					profile->setInterests (fieldElement.text ());
//				else if (fieldElement.tagName () == "favorite_books")
//					profile.FavoriteBooks_ = fieldElement.text ();
//				else if (fieldElement.tagName () == "favorite_authors")
//					profile.FavoriteAuthors_ = fieldElement.text ().split (',',
//							QString::SkipEmptyParts);
//				else if (fieldElement.tagName () == "updates_rss_url")
//					profile.UpdatesRSS_ = QUrl (fieldElement.firstChild ()
//							.toCDATASection ().data ());
//				else if (fieldElement.tagName () == "reviews_rss_url")
//					profile.ReviewsRSS_ = QUrl (fieldElement.firstChild ()
//							.toCDATASection ().data ());
				else if (fieldElement.tagName () == "friends_count")
					profile->setFriendsCount (fieldElement.text ().toUInt ());
				else if (fieldElement.tagName () == "groups_count")
					profile->setGroupsCount (fieldElement.text ().toUInt ());
				else if (fieldElement.tagName () == "reviews_count")
					profile->setBooksCount (fieldElement.text ().toUInt ());
				else if (fieldElement.tagName () == "private")
					profile->setPrivateProfile (fieldElement.text () == "true");
				else if (fieldElement.tagName () == "user_shelves")
				{
					const auto& shelvesList = fieldElement.childNodes ();
					Shelves_t shelves;
					for (int j = 0, shelvesCount = shelvesList.size (); j < shelvesCount; ++j)
						shelves << CreateShelf (shelvesList.at (j).toElement ());
					profile->setShelves (shelves);
				}
//				else if (fieldElement.tagName () == "updates")
//				{
//					const auto& updatesList = fieldElement.childNodes ();
//					for (int j = 0, updatesCount = updatesList.size (); j < updatesCount; ++j)
//						profile.Updates_ << CreateUpdate (updatesList.at (j).toElement ());
//				}
			}

			return profile;
		}

		Updates_t CreateUpdates (const QDomDocument& doc)
		{
			Updates_t updates;
			const auto& responseElement = doc.firstChildElement("GoodreadsResponse");
			if (responseElement.isNull ())
				return updates;

			const auto& updatesElement = responseElement.firstChildElement ("updates");
			const auto& fieldsList = updatesElement.childNodes ();
			for (int i = 0, fieldsCount = fieldsList.size (); i < fieldsCount; ++i)
			{
				const auto& fieldElement = fieldsList.at (i).toElement ();
				if (fieldElement.tagName () == "update")
					updates << CreateUpdate (fieldElement);
			}

			return updates;
		}

		Notification CreateNotification (const QDomElement& notificationElement)
		{
			Notification notification;

			const auto& fieldsList = notificationElement.childNodes ();
			for (int i = 0, fieldsCount = fieldsList.size (); i < fieldsCount; ++i)
			{
				const auto& fieldElement = fieldsList.at (i).toElement ();
				if (fieldElement.tagName () == "new")
					notification.Read_ = (fieldElement.text () == "false");
				else if (fieldElement.tagName () == "created_at")
				{
					const auto& dateTime = fieldElement.text().left (18);
					notification.Date_ = QDateTime::fromString (dateTime,
							Qt::ISODate);
				}
				else if (fieldElement.tagName () == "actors")
				{
					const auto& actorFieldsList = fieldElement.childNodes ();
					for (int j = 0, actorFieldsCount = actorFieldsList.size (); j < actorFieldsCount; ++j)
					{
						const auto& actorFieldElement = actorFieldsList.at (j)
								.toElement ();
						if (actorFieldElement.tagName () != "user")
							continue;

						const auto& userFieldsList = actorFieldElement.childNodes ();
						for (int k = 0, userFieldsCount = userFieldsList.size (); k < userFieldsCount; ++k)
						{
							const auto& userFieldElement = userFieldsList.at (k)
									.toElement ();
							if (userFieldElement.tagName () == "id")
								notification.ActorID_ = userFieldElement.text ();
							else if (userFieldElement.tagName () == "name")
								notification.ActorName_ = userFieldElement.text ();
							else if (userFieldElement.tagName () == "image_url")
								notification.ActorProfileImageUrl_ = QUrl (userFieldElement.firstChild ()
										.toCDATASection ().data ());
							else if (userFieldElement.tagName () == "link")
								notification.ActorProfileUrl_ = QUrl (userFieldElement.firstChild ()
										.toCDATASection ().data ());
						}
					}
				}
				else if (fieldElement.tagName () == "body")
				{
					const auto& actorFieldsList = fieldElement.childNodes ();
					for (int j = 0, actorFieldsCount = actorFieldsList.size (); j < actorFieldsCount; ++j)
					{
						const auto& actorFieldElement = actorFieldsList.at (j)
								.toElement ();
						if (actorFieldElement.tagName () == "html")
							notification.Text_ = actorFieldElement.firstChild ()
									.toCDATASection ().data ();
					}
				}
			}

			return notification;
		}

		Notifications_t CreateNotifications (const QDomDocument& doc)
		{
			Notifications_t notifications;
			const auto& responseElement = doc.firstChildElement("GoodreadsResponse");
			if (responseElement.isNull ())
				return notifications;

			const auto& updatesElement = responseElement.firstChildElement ("notifications");
			const auto& fieldsList = updatesElement.childNodes ();
			for (int i = 0, fieldsCount = fieldsList.size (); i < fieldsCount; ++i)
			{
				const auto& fieldElement = fieldsList.at (i).toElement ();
				if (fieldElement.tagName () == "notification")
					notifications << CreateNotification (fieldElement);
			}

			return notifications;
		}

		Friend CreateFriend (const QDomElement& friendElement)
		{
			Friend friendItem;
			const auto& fieldsList = friendElement.childNodes ();
			for (int i = 0, fieldsCount = fieldsList.size (); i < fieldsCount; ++i)
			{
				const auto& fieldElement = fieldsList.at (i).toElement ();
				if (fieldElement.tagName () == "id")
					friendItem.ID_ = fieldElement.text ();
				else if (fieldElement.tagName () == "name")
					friendItem.Name_ = fieldElement.text ();
				else if (fieldElement.tagName () == "image_url")
					friendItem.ProfileImageUrl_ = QUrl (fieldElement.firstChild ()
							.toCDATASection ().data ());
			}

			return friendItem;
		}

		Friends_t CreateFriends (const QDomDocument& doc)
		{
			Friends_t friends;
			const auto& responseElement = doc.firstChildElement("GoodreadsResponse");
			if (responseElement.isNull ())
				return friends;

			const auto& friendsElement = responseElement.firstChildElement ("friends");
			const auto& fieldsList = friendsElement.childNodes ();
			for (int i = 0, fieldsCount = fieldsList.size (); i < fieldsCount; ++i)
			{
				const auto& fieldElement = fieldsList.at (i).toElement ();
				if (fieldElement.tagName () == "user")
					friends << CreateFriend (fieldElement);
			}

			return friends;
		}

		Group CreateGroup (const QDomElement& groupElement)
		{
			Group group;
			const auto& fieldsList = groupElement.childNodes ();
			for (int i = 0, fieldsCount = fieldsList.size (); i < fieldsCount; ++i)
			{
				const auto& fieldElement = fieldsList.at (i).toElement ();
				if (fieldElement.tagName () == "id")
					group.ID_ = fieldElement.text ();
				else if (fieldElement.tagName () == "access")
					group.Public_ = fieldElement.text () == "public";
				else if (fieldElement.tagName () == "users_count")
					group.UsersCount_ = fieldElement.text ().toInt ();
				else if (fieldElement.tagName () == "image_url")
					group.ImageUrl_ = QUrl (fieldElement.firstChild ()
							.toCDATASection ().data ());
				else if (fieldElement.tagName () == "title")
					group.Name_ = fieldElement.firstChild ()
							.toCDATASection ().data ();
			}

			return group;
		}

		Groups_t CreateGroups (const QDomDocument& doc)
		{
			Groups_t groups;
			const auto& responseElement = doc.firstChildElement("GoodreadsResponse");
			if (responseElement.isNull ())
				return groups;

			const auto& groupsElement = responseElement.firstChildElement ("groups");

			const auto& groupsList = groupsElement.firstChildElement ("list").childNodes ();
			for (int i = 0, groupsCount = groupsList.size (); i < groupsCount; ++i)
			{
				const auto& groupElement = groupsList.at (i).toElement ();
				if (groupElement.tagName () == "group")
					groups << CreateGroup (groupElement);
			}

			return groups;
		}

		Shelves_t CreateShelves (const QDomDocument& doc)
		{
			Shelves_t shelves;
			const auto& responseElement = doc.firstChildElement("GoodreadsResponse");
			if (responseElement.isNull ())
				return shelves;

			const auto& shelvesElement = responseElement.firstChildElement ("shelves");
			const auto& shelvesList = shelvesElement.childNodes ();
			for (int i = 0, shelvesCount = shelvesList.size (); i < shelvesCount; ++i)
			{
				const auto& shelfElement = shelvesList.at (i).toElement ();
				if (shelfElement.tagName () == "user_shelf")
					shelves << CreateShelf (shelfElement);
			}

			return shelves;
		}
	}

	void GoodreadsApi::handleRequestUserInfoFinished ()
	{
		auto reply = qobject_cast<QNetworkReply*> (sender ());
		if (!reply)
			return;

		QDomDocument document;
		reply->deleteLater ();
		if (!FillDomDocument (reply->readAll (), document))
		{
			qWarning () << Q_FUNC_INFO
					<< "unable to get user profile";
			return;
		}

		emit gotUserProfile (CreateUserProfile (document));
	}

	void GoodreadsApi::handleRequestFriendsUpdatesFinished ()
	{
		auto reply = qobject_cast<QNetworkReply*> (sender ());
		if (!reply)
			return;

		QDomDocument document;
		reply->deleteLater ();
		if (!FillDomDocument (reply->readAll (), document))
		{
			qWarning () << Q_FUNC_INFO
					<< "unable to get recent updates";
			return;
		}

		emit gotRecentUpdates (CreateUpdates (document));
	}

	void GoodreadsApi::handleRequestNotificationsFinished ()
	{
		auto reply = qobject_cast<QNetworkReply*> (sender ());
		if (!reply)
			return;

		QDomDocument document;
		reply->deleteLater ();
		if (!FillDomDocument (reply->readAll (), document))
		{
			qWarning () << Q_FUNC_INFO
					<< "unable to get notifications";
			return;
		}

		emit gotNotifications (CreateNotifications (document));
	}

	void GoodreadsApi::handleRequestFriendsFinished ()
	{
		auto reply = qobject_cast<QNetworkReply*> (sender ());
		if (!reply)
			return;

		QDomDocument document;
		reply->deleteLater ();
		if (!FillDomDocument (reply->readAll (), document))
		{
			qWarning () << Q_FUNC_INFO
					<< "unable to get friends list";
			return;
		}

		emit gotFriends (CreateFriends (document));
	}

	void GoodreadsApi::handleRequestGroupsFinished ()
	{
		auto reply = qobject_cast<QNetworkReply*> (sender ());
		if (!reply)
			return;

		QDomDocument document;
		reply->deleteLater ();
		if (!FillDomDocument (reply->readAll (), document))
		{
			qWarning () << Q_FUNC_INFO
					<< "unable to get groups list";
			return;
		}

		emit gotGroups (CreateGroups (document));
	}

	void GoodreadsApi::handleRequestShelvesFinished ()
	{
		auto reply = qobject_cast<QNetworkReply*> (sender ());
		if (!reply)
			return;

		QDomDocument document;
		reply->deleteLater ();
		if (!FillDomDocument (reply->readAll (), document))
		{
			qWarning () << Q_FUNC_INFO
					<< "unable to get bookshelves list";
			return;
		}

		emit gotShelves (CreateShelves (document));
	}

	void GoodreadsApi::handleAddShelfFinished ()
	{
		auto reply = qobject_cast<QNetworkReply*> (sender ());
		if (!reply)
			return;

		QDomDocument document;
		reply->deleteLater ();
		qDebug () << reply->readAll ();
//		if (!FillDomDocument (reply->readAll (), document))
//		{
//			qWarning () << Q_FUNC_INFO
//					<< "unable to add bookshelf";
//			return;
		//		}
	}

	void GoodreadsApi::handleEditShelfFinished ()
	{
		auto reply = qobject_cast<QNetworkReply*> (sender ());
		if (!reply)
			return;

		QDomDocument document;
		reply->deleteLater ();
		qDebug () << reply->readAll ();
//		if (!FillDomDocument (reply->readAll (), document))
//		{
//			qWarning () << Q_FUNC_INFO
//					<< "unable to add bookshelf";
//			return;
		//		}
	}
}
