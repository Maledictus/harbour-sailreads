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
#include <QQuickItem>
#include <QQuickView>
#include <QtDebug>
#include "goodreadsapi.h"
#include "localstorage.h"

namespace SailReads
{
	SailreadsManager::SailreadsManager (QQuickView *view, QObject *parent)
	: QObject (parent)
	, MainView_ (view)
	, GoodreadsApi_ (new GoodreadsApi (this))
	, LocalStorage_ (new LocalStorage (this))
	{
	}

	void SailreadsManager::Init ()
	{
		connect (MainView_->rootObject (),
				SIGNAL (applicationAuthorized (bool)),
				this,
				SLOT (handleApplicationAuthorized (bool)));

		AccessToken_ = LocalStorage_->GetValue ("AccessToken");
		AccessTokenSecret_ = LocalStorage_->GetValue ("AccessTokenSecret");

		if (AccessToken_.isEmpty () || AccessTokenSecret_.isEmpty ())
			AuthorizeApplication ();
		else
			RequestUserId ();
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

	void SailreadsManager::RequestUserId ()
	{
		GoodreadsApi_->RequestUserID (AccessToken_, AccessTokenSecret_);
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

			RequestUserId ();
		}
	}
}
