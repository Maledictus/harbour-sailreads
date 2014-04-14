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
	struct SignedUrlData
	{
		QString ConsumerKey_;
		QString ConsumerSecret_;
		QString AccessToken_;
		QString AccessTokenSecret_;
		QUrl BaseUrl_;
	};

	class OAuthWrapper : public QObject
	{
		Q_OBJECT

		const QString ConsumerKey_;
		const QString ConsumerSecret_;
		const QUrl BaseUrl_;
		QString RequestToken_;
		QString RequestTokenSecret_;

	public:
		explicit OAuthWrapper (const QString& consumerKey,
				const QString& consumerSecret, const QUrl& baseUrl,
				QObject *parent = 0);

		QUrl GetAuthorizationUrl ();
		QPair<QString, QString> GetAccessTokens () const;

		QUrl MakeGetSignedUrl (const SignedUrlData& data) const;
	};
}
