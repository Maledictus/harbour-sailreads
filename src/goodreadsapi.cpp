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
#include "oauthwrapper.h"

namespace SailReads
{
	GoodreadsApi::GoodreadsApi (QObject *parent)
	: QObject (parent)
	, ConsumerKey_ ("GRGhcLIXU8M8u1NcnoMVFg")
	, ConsumerSecret_ ("Epo3MYIT3V1JcuC6OkZxptHLEuD8yqUgAj7mLNw")
	, BaseUrl_ ("http://www.goodreads.com")
	, OAuthWrapper_ (new OAuthWrapper (ConsumerKey_, ConsumerSecret_, BaseUrl_, this))
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
}
