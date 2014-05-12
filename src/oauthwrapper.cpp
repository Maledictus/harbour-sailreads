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

#include <functional>
#include <memory>
#include "oauthwrapper.h"
#include <QFuture>
#include <QtConcurrent/QtConcurrentRun>
#include <QtDebug>
#include "curlwrapper.h"

extern "C"
{
	#include <oauth.h>
}

namespace SailReads
{
	OAuthWrapper::OAuthWrapper(const QString& consumerKey,
			const QString& consumerSecret, const QUrl& baseUrl, QObject *parent)
	: QObject (parent)
	, ConsumerKey_ (consumerKey)
	, ConsumerSecret_ (consumerSecret)
	, BaseUrl_ (baseUrl)
	{
		connect (&Watcher_,
				 SIGNAL (finished ()),
				 this,
				 SLOT (handleSignedRequestFinished ()));
	}

	QUrl OAuthWrapper::GetAuthorizationUrl ()
	{
		const char *requestTokenUri = (BaseUrl_.toString () + "/oauth/request_token").toUtf8 ();
		char *requestUrl = NULL;
		char *reply = NULL;
		requestUrl = oauth_sign_url2 (requestTokenUri, NULL, OA_HMAC, NULL,
				ConsumerKey_.toUtf8 (), ConsumerSecret_.toUtf8 (), NULL, NULL);
		reply = oauth_http_get (requestUrl, NULL);

		if (requestUrl)
			free (requestUrl);

		if (!reply)
			return QUrl ();

		int rc;
		char **rv = NULL;
		rc = oauth_split_url_parameters (reply, &rv);
		qsort (rv, rc, sizeof (char*), oauth_cmpstringp);

		std::shared_ptr<void> guard (nullptr, [&rv, &reply] (void*)
			{
				if (rv)
					free (rv);

				free (reply);
			});

		if (rc == 2 && !strncmp (rv [0], "oauth_token=", 11)	&&
				!strncmp (rv [1], "oauth_token_secret=", 18))
		{
			RequestToken_ = strdup (&(rv [0][12]));
			RequestTokenSecret_ = strdup (&(rv [1][19]));
			return BaseUrl_.toString () + "/oauth/authorize?mobile=1&oauth_token=" +
					RequestToken_;
		}

		return QUrl ();
	}

	QPair<QString, QString> OAuthWrapper::GetAccessTokens () const
	{
		const char *accessTokenUri = (BaseUrl_.toString () + "/oauth/access_token").toUtf8 ();
		char *requestUrl = NULL;
		char *reply = NULL;
		char *postarg = NULL;
		requestUrl = oauth_sign_url2 (accessTokenUri, NULL, OA_HMAC, NULL,
				ConsumerKey_.toUtf8 (), ConsumerSecret_.toUtf8 (),
				RequestToken_.toUtf8 (), RequestTokenSecret_.toUtf8 ());
		reply = oauth_http_get (requestUrl, postarg);

		if (requestUrl)
			free (requestUrl);

		if (!reply)
			return QPair<QString, QString> ();

		int rc;
		char **rv = NULL;
		rc = oauth_split_url_parameters (reply, &rv);
		qsort (rv, rc, sizeof (char*), oauth_cmpstringp);

		std::shared_ptr<void> guard (nullptr, [&rv, &reply] (void*)
			{
				if (rv)
					free (rv);

				free (reply);
			});

		if (rc == 2 && !strncmp (rv [0], "oauth_token=", 11)	&&
				!strncmp (rv [1], "oauth_token_secret=", 18))
			return qMakePair<QString, QString> (strdup (&(rv [0][12])), strdup (&(rv [1][19])));

		return QPair<QString, QString> ();
	}

	QUrl OAuthWrapper::MakeGetSignedUrl (const SignedUrlData& data) const
	{
		return QUrl(oauth_sign_url2 (data.BaseUrl_.toEncoded (),
				NULL, OA_HMAC, NULL, data.ConsumerKey_.toUtf8 (),
				data.ConsumerSecret_.toUtf8 (), data.AccessToken_.toUtf8 (),
				data.AccessTokenSecret_.toUtf8 ()));
	}

	void OAuthWrapper::MakeSignedRequest (const SignedUrlData& data)
	{
		char *params = NULL;
		QUrl url(oauth_sign_url2 (data.BaseUrl_.toEncoded (),
				&params, OA_HMAC, data.RequestType_.toUtf8 (),
				data.ConsumerKey_.toUtf8 (), data.ConsumerSecret_.toUtf8 (),
				data.AccessToken_.toUtf8 (), data.AccessTokenSecret_.toUtf8 ()));
		QFuture<QByteArray> future;
		if (data.RequestType_ == "POST")
			future = QtConcurrent::run (std::bind (&CurlWrapper::Post, url, params));
		else if (data.RequestType_ == "PUT")
			future = QtConcurrent::run (std::bind (&CurlWrapper::Put, url, params));

		Watcher_.setFuture (future);
	}

	void OAuthWrapper::handleSignedRequestFinished ()
	{
		emit readReady (Watcher_.result ());
	}
}
