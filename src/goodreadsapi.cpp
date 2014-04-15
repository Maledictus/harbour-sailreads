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
#include <QNetworkReply>
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
				SIGNAL (finished ()),
				this,
				SLOT (handleRequestFriendsUpdatesFinished ()));
	}

	void GoodreadsApi::handleDownloadProgress(qint64 bytesReceived, qint64 bytesTotal)
	{
		RequestInProcess_ = (bytesReceived != bytesTotal);
		emit requestInProcessChanged ();
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
					shelf.BookCount_ = fieldElement.text ().toUInt ();
				else if (fieldElement.tagName () == "description")
					shelf.Description_ = fieldElement.text ();
				else if (fieldElement.tagName () == "display_fields")
					shelf.Description_ = fieldElement.text ();
				else if (fieldElement.tagName () == "exclusive_flag")
					shelf.ExclusiveFlag_ = fieldElement.text () == "true" ? true : false;
				else if (fieldElement.tagName () == "featured")
					shelf.Featured_ = fieldElement.text () == "true" ? true : false;
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
					update.Date_ = QDateTime::fromString (fieldElement.text (),
							"ddd, dd MMM yyyy hh:mm:ss zzz");
				else if (fieldElement.tagName () == "actor")
				{
					const auto& actorFieldsList = fieldElement.childNodes ();
					for (int j = 0, actorFieldsCount = actorFieldsList.size (); j < actorFieldsCount; ++j)
					{
						const auto& actorFieldElement = actorFieldsList.at (i)
								.toElement ();
						if (actorFieldElement.tagName () == "id")
							update.ActorID_ = actorFieldElement.text ();
						else if (actorFieldElement.tagName () == "name")
							update.ActorName_ = actorFieldElement.text ();
						else if (actorFieldElement.tagName () == "image_url")
							update.ActorProfileImage_ = QUrl (actorFieldElement.firstChild ()
									.toCDATASection ().data ());
						else if (actorFieldElement.tagName () == "link")
							update.ActorProfileUrl_ = QUrl (actorFieldElement.firstChild ()
									.toCDATASection ().data ());
					}
				}
			}

			return update;
		}

		UserProfile CreateUserProfile (const QDomDocument& doc)
		{
			UserProfile profile;
			const auto& responseElement = doc.firstChildElement("GoodreadsResponse");
			if (responseElement.isNull ())
				return profile;

			const auto& userElement = responseElement.firstChildElement ("user");
			const auto& fieldsList = userElement.childNodes ();
			for (int i = 0, fieldsCount = fieldsList.size (); i < fieldsCount; ++i)
			{
				const auto& fieldElement = fieldsList.at (i).toElement ();
				if (fieldElement.tagName () == "id")
					profile.ID_ = fieldElement.text ();
				else if (fieldElement.tagName () == "name")
					profile.Name_ = fieldElement.text ();
				else if (fieldElement.tagName () == "user_name")
					profile.Nickname_ = fieldElement.text ();
				else if (fieldElement.tagName () == "link")
					profile.ProfileUrl_ = QUrl (fieldElement.firstChild ()
							.toCDATASection ().data ());
				else if (fieldElement.tagName () == "image_url")
					profile.ProfileImage_ = QUrl (fieldElement.firstChild ()
							.toCDATASection ().data ());
				else if (fieldElement.tagName () == "about")
					profile.About_ = fieldElement.text ();
				else if (fieldElement.tagName () == "age")
					profile.Age_ = fieldElement.text ().toUInt ();
				else if (fieldElement.tagName () == "gender")
					profile.Gender_ = fieldElement.text ();
				else if (fieldElement.tagName () == "location")
					profile.Location_ = fieldElement.text ();
				else if (fieldElement.tagName () == "website")
					profile.WebSite_ = QUrl (fieldElement.firstChild ()
							.toCDATASection ().data ());
				else if (fieldElement.tagName () == "interests")
					profile.Interests_ = fieldElement.text ().split (',',
							QString::SkipEmptyParts);
				else if (fieldElement.tagName () == "favorite_books")
					profile.FavoriteBooks_ = fieldElement.text ();
				else if (fieldElement.tagName () == "favorite_authors")
					profile.FavoriteAuthors_ = fieldElement.text ().split (',',
							QString::SkipEmptyParts);
				else if (fieldElement.tagName () == "updates_rss_url")
					profile.UpdatesRSS_ = QUrl (fieldElement.firstChild ()
							.toCDATASection ().data ());
				else if (fieldElement.tagName () == "reviews_rss_url")
					profile.ReviewsRSS_ = QUrl (fieldElement.firstChild ()
							.toCDATASection ().data ());
				else if (fieldElement.tagName () == "friends_count")
					profile.FriendsCount_ = fieldElement.text ().toUInt ();
				else if (fieldElement.tagName () == "groups_count")
					profile.GroupsCount_ = fieldElement.text ().toUInt ();
				else if (fieldElement.tagName () == "reviews_count")
					profile.ReviewsCount_ = fieldElement.text ().toUInt ();
				else if (fieldElement.tagName () == "user_shelves")
				{
					const auto& shelvesList = fieldElement.childNodes ();
					for (int j = 0, shelvesCount = shelvesList.size (); j < shelvesCount; ++j)
						profile.Shelves_ << CreateShelf (shelvesList.at (j).toElement ());
				}
				else if (fieldElement.tagName () == "updates")
				{
					const auto& updatesList = fieldElement.childNodes ();
					for (int j = 0, updatesCount = updatesList.size (); j < updatesCount; ++j)
						profile.Updates_ << CreateUpdate (updatesList.at (j).toElement ());
				}
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
}
