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

#include "goodreadsapi.h"

#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QXmlQuery>

#include "oauthwrapper.h"

namespace Sailreads
{
GoodReadsApi::GoodReadsApi(QObject *parent)
: QObject(parent)
, m_ConsumerKey("GRGhcLIXU8M8u1NcnoMVFg")
, m_ConsumerSecretKey("Epo3MYIT3V1JcuC6OkZxptHLEuD8yqUgAj7mLNw")
, m_BaseUrl("https://www.goodreads.com")
, m_NAM(new QNetworkAccessManager(this))
, m_OAuthWrapper(new OAuthWrapper(m_ConsumerKey, m_ConsumerSecretKey, m_BaseUrl, this))
{
}

void GoodReadsApi::ObtainRequestToken() const
{
    QUrl requestTokenUrl = m_OAuthWrapper->GetRequestTokenUrl();

    QNetworkReply *reply = m_NAM->get(QNetworkRequest(requestTokenUrl));

    connect(reply,
            &QNetworkReply::finished,
            this,
            &GoodReadsApi::handleObtainRequestToken);
}

void GoodReadsApi::RequestAccessToken() const
{
    QUrl accessTokenUrl = m_OAuthWrapper->GetAccessTokenUrl(m_RequestToken, m_RequestTokenSecret);
    QNetworkReply *reply = m_NAM->get(QNetworkRequest(accessTokenUrl));

    connect(reply,
            &QNetworkReply::finished,
            this,
            &GoodReadsApi::handleRequestAccessToken);
}

void GoodReadsApi::AuthUser(const QString& accessToken, const QString& accessTokenSecret)
{
    const auto& url = m_OAuthWrapper->MakeGetSignedUrl(accessToken, accessTokenSecret,
            QUrl("https://www.goodreads.com/api/auth_user"));
    auto reply = m_NAM->get(QNetworkRequest(url));
    connect (reply,
             &QNetworkReply::finished,
             this,
             &GoodReadsApi::handleAuthUser);
}

QByteArray GoodReadsApi::GetReply(QObject *sender, bool& ok)
{
    auto reply = qobject_cast<QNetworkReply*>(sender);
    QByteArray data;
    if (!reply) {
        qWarning() << "Invalid reply";
        ok = false;
        return data;
    }
    reply->deleteLater();

    if (reply->error() != QNetworkReply::NoError) {
        qWarning() << Q_FUNC_INFO << "There is network error: "
                << reply->error() << reply->errorString();
        ok = false;
        return data;
    }

    ok = true;
    return reply->readAll();
}

void GoodReadsApi::handleObtainRequestToken()
{
    emit requestFinished();

    bool ok = false;
    QByteArray data = GetReply(sender(), ok);
    if (!ok || data.isEmpty()) {
        return;
    }

    QList<QByteArray> tokens = data.split('&');
    if (tokens.size() != 2 ||
            !tokens.value(0).contains("oauth_token") ||
            !tokens.value(1).contains("oauth_token_secret")) {
        qWarning() << Q_FUNC_INFO << "Invalid answer";
        return;
    }

    m_RequestToken = QString::fromUtf8(tokens.value(0).mid(12));
    m_RequestTokenSecret = QString::fromUtf8(tokens.value(1).mid(19));

    emit requestTokenChanged(m_RequestToken);
}

void GoodReadsApi::handleRequestAccessToken()
{
    emit requestFinished();

    bool ok = false;
    QByteArray data = GetReply(sender(), ok);
    if (!ok || data.isEmpty()) {
        return;
    }

    QList<QByteArray> tokens = data.split('&');
    if (tokens.size() != 2 ||
            !tokens.value(0).contains("oauth_token") ||
            !tokens.value(1).contains("oauth_token_secret")) {
        qWarning() << Q_FUNC_INFO << "Invalid answer";
        return;
    }

    QString accessToken = QString::fromUtf8(tokens.value(0).mid(12));
    QString accessTokenSecret = QString::fromUtf8(tokens.value(1).mid(19));

    emit accessTokensChanged(accessToken, accessTokenSecret);
}

namespace
{
QString GetQueryResult(QXmlQuery& query, const QString& request)
{
    QString result;
    query.setQuery(request);
    if (!query.evaluateTo(&result)) {
        result = QString();
    }
    return result.trimmed();
}
}

void GoodReadsApi::handleAuthUser()
{
    emit requestFinished();

    bool ok = false;
    QByteArray data = GetReply(sender(), ok);
    if (!ok || data.isEmpty()) {
        return;
    }

    QXmlQuery query;
    query.setFocus(data);
    const QString id(GetQueryResult(query, "/GoodreadsResponse/user/@id/string()"));
    const QString name(GetQueryResult(query, "/GoodreadsResponse/user/name/text()"));
    const QString link(GetQueryResult(query, "/GoodreadsResponse/user/link/text()"));

    emit gotAuthUserInfo(id, name, link);
}


}
