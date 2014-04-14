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
#include <QDomDocument>
#include <QNetworkReply>
#include <QNetworkRequest>
#include "oauthwrapper.h"
#include "networkaccessmanager.h"

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

	QUrl GoodreadsApi::GetAuthorizationUrl ()
	{
		return OAuthWrapper_->GetAuthorizationUrl ();
	}

	QPair<QString, QString> GoodreadsApi::GetAccessTokens () const
	{
		return OAuthWrapper_->GetAccessTokens ();
	}

	void GoodreadsApi::RequestUserID (const QString& accessToken,
			const QString& accessTokenSecret)
	{
		const auto& url = OAuthWrapper_->MakeGetSignedUrl ({ ConsumerKey_,
				ConsumerSecret_, accessToken, accessTokenSecret,
				QUrl ("https://www.goodreads.com/api/auth_user") });
		auto reply = NetworkAccessManager_->get (QNetworkRequest (url));
		connect (reply,
				 SIGNAL (finished ()),
				 this,
				 SLOT (handleRequestUserIDFinished ()));
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

	void GoodreadsApi::handleRequestUserIDFinished ()
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

		QString userId;
		QString userName;
		QUrl profileUrl;

		const auto& userTags = document.elementsByTagName ("user");
		if (!userTags.isEmpty ())
		{
			const auto& element = userTags.at (0).toElement ();
			if (element.hasAttribute ("id"))
				userId = element.attribute ("id");
		}

		const auto& nameTags = document.elementsByTagName ("name");
		if (!nameTags.isEmpty ())
			userName = nameTags.at (0).toElement ().text ();

		const auto& linkTags = document.elementsByTagName ("link");
		if (!linkTags.isEmpty ())
			profileUrl = linkTags.at (0).firstChild ().toCDATASection ().data ();
	}
}
