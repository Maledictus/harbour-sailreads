/*
The MIT License (MIT)

Copyright (c) 2018 Oleg Linkin <maledictusdemagog@gmail.com>

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

#include "oauthwrapper.h"

extern "C"
{
#include <oauth.h>
}

namespace Sailreads
{
OAuthWrapper::OAuthWrapper(const QString& consumerKey, const QString& consumerSecretKey,
        const QUrl& baseUrl, QObject *parent)
: QObject (parent)
, m_ConsumerKey(consumerKey)
, m_ConsumerSecretKey(consumerSecretKey)
, m_BaseUrl(baseUrl)
{
}

QUrl OAuthWrapper::GetRequestTokenUrl() const
{
    const QByteArray requestTokenUri((m_BaseUrl.toString() + "/oauth/request_token").toUtf8 ());
    const QString requestUrlStr(oauth_sign_url2(requestTokenUri.data(), NULL, OA_HMAC, NULL,
            m_ConsumerKey.toUtf8(), m_ConsumerSecretKey.toUtf8(), NULL, NULL));
    return QUrl(requestUrlStr);
}

QUrl OAuthWrapper::GetAccessTokenUrl(const QString& requestToken, const QString& requestTokenSecret) const
{
    const QByteArray accessTokenUri((m_BaseUrl.toString() + "/oauth/access_token").toUtf8 ());
    const QString requestUrlStr(oauth_sign_url2(accessTokenUri.data(), NULL, OA_HMAC, NULL,
            m_ConsumerKey.toUtf8(), m_ConsumerSecretKey.toUtf8(),
            requestToken.toUtf8(), requestTokenSecret.toUtf8()));
    return QUrl(requestUrlStr);
}

QUrl OAuthWrapper::MakeGetSignedUrl(const QString& accessToken, const QString& accessTokenSecret,
        const QUrl& url)
{
    return QUrl(oauth_sign_url2(url.toEncoded(), NULL, OA_HMAC, NULL,
            m_ConsumerKey.toUtf8(), m_ConsumerSecretKey.toUtf8(),
            accessToken.toUtf8(), accessTokenSecret.toUtf8()));
}
}
