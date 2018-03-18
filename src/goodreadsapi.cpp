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

#include <QDomDocument>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QXmlQuery>

#include "oauthwrapper.h"
#include "rpcutils.h"

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

void GoodReadsApi::UpdateCredentials(const QString& accessToken, const QString& accessTokenSecret)
{
    m_AccessToken = accessToken;
    m_AccessTokenSecret = accessTokenSecret;
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

void GoodReadsApi::AuthUser()
{
    const auto& pair = m_OAuthWrapper->MakeSignedUrl(m_AccessToken, m_AccessTokenSecret,
            QUrl("https://www.goodreads.com/api/auth_user"));
    auto reply = m_NAM->get(QNetworkRequest(pair.first));
    connect (reply,
             &QNetworkReply::finished,
             this,
             &GoodReadsApi::handleAuthUser);
}

void GoodReadsApi::GetUserInfo(quint64 id)
{
    const QUrl url(QString("https://www.goodreads.com/user/show/%1.xml?key=%2")
            .arg(id)
            .arg(m_ConsumerKey));
    auto reply = m_NAM->get(QNetworkRequest(url));
    connect (reply,
             &QNetworkReply::finished,
             this,
             &GoodReadsApi::handleGetUserInfo);
}

void GoodReadsApi::GetUpdates()
{
    const auto& pair = m_OAuthWrapper->MakeSignedUrl(m_AccessToken, m_AccessTokenSecret,
            QUrl("https://www.goodreads.com/updates/friends.xml"));
    auto reply = m_NAM->get(QNetworkRequest(pair.first));
    connect (reply,
             &QNetworkReply::finished,
             this,
             &GoodReadsApi::handleGetUpdates);
}

void GoodReadsApi::GetBookShelves(quint64 userId)
{
    const QUrl url(QString("https://www.goodreads.com/shelf/list.xml?key=%1&user_id=%2")
            .arg(m_ConsumerKey)
            .arg(userId));
    auto reply = m_NAM->get(QNetworkRequest(url));
    connect(reply,
            &QNetworkReply::finished,
            this,
            [this, userId]() {
                handleGetBookShelves(userId);
            });
}

void GoodReadsApi::AddBookShelf(const QString& name, bool exclusive)
{
    QString urlString = QString("https://www.goodreads.com/user_shelves.xml?user_shelf[name]=%1&"
            "user_shelf[exclusive_flag]=%2").arg(name).arg(exclusive ? "true" : "false");
    const auto& pair = m_OAuthWrapper->MakeSignedUrl(m_AccessToken, m_AccessTokenSecret,
            QUrl(urlString), "POST");
    auto reply = m_NAM->post(QNetworkRequest(pair.first), pair.second);
    connect(reply,
            &QNetworkReply::finished,
            this,
            &GoodReadsApi::handleAddBookShelf);
}

void GoodReadsApi::EditBookShelf(quint64 id, const QString& name, bool exclusive)
{
    const auto& pair = m_OAuthWrapper->MakeSignedUrl(m_AccessToken, m_AccessTokenSecret,
            QUrl(QString("https://www.goodreads.com/user_shelves/%1.xml?user_shelf[name]=%2&"
                    "user_shelf[exclusive_flag]=%3").arg(id).arg(name)
                        .arg(exclusive ? "true" : "false")), "PUT");
    auto reply = m_NAM->put(QNetworkRequest(pair.first), QByteArray());
    connect(reply,
            &QNetworkReply::finished,
            this,
            &GoodReadsApi::handleEditBookShelf);
}

void GoodReadsApi::RemoveBookShelf(quint64)
{
}

void GoodReadsApi::GetGroups(quint64 userId)
{
    const QUrl url(QString("https://www.goodreads.com/group/list/%1.xml?key=%2")
            .arg(userId)
            .arg(m_ConsumerKey));
    auto reply = m_NAM->get(QNetworkRequest(url));
    connect(reply,
            &QNetworkReply::finished,
            this,
            [this, userId]() {
                handleGetGroups(userId);
            });
}

void GoodReadsApi::GetFriends(quint64 userId)
{
    const auto& pair = m_OAuthWrapper->MakeSignedUrl(m_AccessToken, m_AccessTokenSecret,
            QUrl(QString("https://www.goodreads.com/friend/user/%1?format=xml").arg(userId)));
    auto reply = m_NAM->get(QNetworkRequest(pair.first));
    connect (reply,
             &QNetworkReply::finished,
             this,
             [this, userId]() {
                 handleGetFriends(userId);
             });
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


QByteArray GetReply(QObject *sender, bool& ok)
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

QDomDocument ParseDocument(const QByteArray& data, bool& ok)
{
    QDomDocument document;
    QString errorMsg;
    int errorLine = -1;
    int errorColumn = -1;
    if (!document.setContent(data, &errorMsg, &errorLine, &errorColumn))
    {
        qWarning() << Q_FUNC_INFO
                << errorMsg
                << "in line:"
                << errorLine
                << "column:"
                << errorColumn;
        ok = false;
    }
    else
    {
        ok = true;
    }
    return document;
}

QDomDocument GetDocumentFromReply(QObject *sender, bool& ok)
{
    QDomDocument doc;
    QByteArray data = GetReply(sender, ok);
    if (!ok || data.isEmpty()) {
        return doc;
    }

    ok = false;
    return ParseDocument(data, ok);
}
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

    emit gotAuthUserInfo(id.toLongLong(), name, link);
}

void GoodReadsApi::handleGetUserInfo()
{
    emit requestFinished();

    bool ok = false;
    auto doc = GetDocumentFromReply(sender(), ok);
    if (!ok) {
        return;
    }
    emit gotUserProfile(RpcUtils::Parser::ParseUserProfile(doc));
}

void GoodReadsApi::handleGetUpdates()
{
    emit requestFinished();

    bool ok = false;
    auto doc = GetDocumentFromReply(sender(), ok);
    if (!ok) {
        return;
    }

//    qDebug() << doc.toByteArray();
    //emit gotUserProfile(RpcUtils::Parser::ParseUserProfile(doc));
}

void GoodReadsApi::handleGetBookShelves(quint64 userId)
{
    emit requestFinished();

    bool ok = false;
    auto doc = GetDocumentFromReply(sender(), ok);
    if (!ok) {
        return;
    }

    emit gotUserBookShelves(userId, RpcUtils::Parser::ParseBookShelves(doc));
}

void GoodReadsApi::handleAddBookShelf()
{
    emit requestFinished();

    bool ok = false;
    auto doc = GetDocumentFromReply(sender(), ok);
    if (!ok) {
        return;
    }

    emit bookShelfAdded(RpcUtils::Parser::ParseBookShelf(doc.firstChildElement("user_shelf")));
}

void GoodReadsApi::handleEditBookShelf()
{
    emit requestFinished();

    bool ok = false;
    auto doc = GetDocumentFromReply(sender(), ok);
    if (!ok) {
        return;
    }

    emit bookShelfEdited(RpcUtils::Parser::ParseBookShelf(doc.firstChildElement("user_shelf")));
}

void GoodReadsApi::handleGetGroups(quint64 userId)
{
    emit requestFinished();

    bool ok = false;
    auto doc = GetDocumentFromReply(sender(), ok);
    if (!ok) {
        return;
    }

    emit gotUserGroups(userId, RpcUtils::Parser::ParseGroups(doc));
}

void GoodReadsApi::handleGetFriends(quint64 userId)
{
    emit requestFinished();

    bool ok = false;
    auto doc = GetDocumentFromReply(sender(), ok);
    if (!ok) {
        return;
    }

    emit gotUserFriends(userId, RpcUtils::Parser::ParseFriends(doc));
}
}
