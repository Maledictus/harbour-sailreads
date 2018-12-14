/*
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

#include "oauth1.h"

#include <random>

#include <QCryptographicHash>
#include <QMessageAuthenticationCode>
#include <QNetworkRequest>

namespace Sailreads
{
OAuth1::OAuth1(const QString& consumerKey, const QString& consumerSecretKey,
        QNetworkAccessManager *manager, QObject *parent)
: QObject(parent)
, m_NAM(manager)
, m_ConsumerKey(consumerKey)
, m_ConsumerSecretKey(consumerSecretKey)
{
}

QPair<QString, QString> OAuth1::GetClientCredentials() const
{
    return qMakePair(m_ConsumerKey, m_ConsumerSecretKey);
}

const QString OAuth1::OAuth1Keys::oauthCallback =           QStringLiteral("oauth_callback");
const QString OAuth1::OAuth1Keys::oauthCallbackConfirmed =  QStringLiteral("oauth_callback_confirmed");
const QString OAuth1::OAuth1Keys::oauthConsumerKey =        QStringLiteral("oauth_consumer_key");
const QString OAuth1::OAuth1Keys::oauthNonce =              QStringLiteral("oauth_nonce");
const QString OAuth1::OAuth1Keys::oauthSignature =          QStringLiteral("oauth_signature");
const QString OAuth1::OAuth1Keys::oauthSignatureMethod =    QStringLiteral("oauth_signature_method");
const QString OAuth1::OAuth1Keys::oauthTimestamp =          QStringLiteral("oauth_timestamp");
const QString OAuth1::OAuth1Keys::oauthToken =              QStringLiteral("oauth_token");
const QString OAuth1::OAuth1Keys::oauthTokenSecret =        QStringLiteral("oauth_token_secret");
const QString OAuth1::OAuth1Keys::oauthVerifier =           QStringLiteral("oauth_verifier");
const QString OAuth1::OAuth1Keys::oauthVersion =            QStringLiteral("oauth_version");

QNetworkReply* OAuth1::RequestTemporaryCredentials(QNetworkAccessManager::Operation operation,
        const QUrl& url)
{
    if (!m_NAM)
    {
        return nullptr;
    }

    if (url.isEmpty())
    {
        return nullptr;
    }

    if (operation != QNetworkAccessManager::GetOperation &&
            operation != QNetworkAccessManager::PostOperation)
    {
        return nullptr;
    }

    QNetworkRequest request(url);
    request.setAttribute(QNetworkRequest::FollowRedirectsAttribute, true);
    QVariantMap headers;
    QVariantMap remainingParameters;
    AppendCommonHeaders(headers);
    for (auto it = parameters.begin(), end = parameters.end(); it != end; ++it) {
        const auto key = it.key();
        const auto value = it.value();
        if (key.startsWith(QStringLiteral("oauth_"))) {
            headers.insert(key, value);
        }
        else {
            remainingParameters.insert(key, value);
        }
    }

    AppendSignature(stage, &headers, url, operation, remainingParameters);
    request.setRawHeader("Authorization", QOAuth1::generateAuthorizationHeader(headers));
    QNetworkReply *reply = nullptr;
    if (operation == QNetworkAccessManager::GetOperation) {
        if (parameters.size() > 0) {
            QUrl url = request.url();
            url.setQuery(QOAuth1Private::createQuery(remainingParameters));
            request.setUrl(url);
        }
        reply = networkAccessManager()->get(request);
    }
    else if (operation == QNetworkAccessManager::PostOperation) {
        QUrlQuery query = QOAuth1Private::createQuery(remainingParameters);
        const QByteArray data = query.toString(QUrl::FullyEncoded).toUtf8();
        request.setHeader(QNetworkRequest::ContentTypeHeader,
                          QStringLiteral("application/x-www-form-urlencoded"));
        reply = networkAccessManager()->post(request, data);
    }
    connect(reply, QOverload<QNetworkReply::NetworkError>::of(&QNetworkReply::error),
            this, &QOAuth1Private::_q_onTokenRequestError);
    QAbstractOAuthReplyHandler *handler = replyHandler ? replyHandler.data()
                                                       : defaultReplyHandler.data();
    QObject::connect(reply, &QNetworkReply::finished,
                     [handler, reply]() { handler->networkReplyFinished(reply); });
    connect(handler, &QAbstractOAuthReplyHandler::tokensReceived, this,
            &QOAuth1Private::_q_tokensReceived);
    return reply;
}

QNetworkReply* OAuth1::RequestTokenCredentials(QNetworkAccessManager::Operation operation,
        const QUrl& url)
{

}

void OAuth1::AppendCommonHeader(QVariantMap& headers)
{
    const auto currentDateTime = QDateTime::currentDateTimeUtc();
    headers.insert(OAuth1Keys::oauthNonce, OAuth1::GenerateNonce());
    headers.insert(OAuth1Keys::oauthConsumerKey, m_ConsumerKey);
    headers.insert(OAuth1Keys::oauthTimestamp, QString::number(currentDateTime.toTime_t()));
    headers.insert(OAuth1Keys::oauthVersion, m_OAuth1Version);
    headers.insert(OAuth1Keys::oauthSignatureMethod, m_OAuth1Signature);
}

void OAuth1::AppendOAuthSignature(QVariantMap& headers, const QUrl& url,
        QNetworkAccessManager::Operation operation, const QVariantMap& parameters)
{
    QByteArray signature;
    {
        QVariantMap allParameters = QVariantMap(headers).unite(parameters);
        signature = generateSignature(allParameters, url, operation);
    }
    headers.insert(OAuth1Keys::oauthSignature, signature);
}

QString OAuth1::GenerateNonce() const
{
    static const int nonceSize = 8;
    const char characters[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
    static std::mt19937 randomEngine(QDateTime::currentDateTime().toMSecsSinceEpoch());
    std::uniform_int_distribution<int> distribution(0, sizeof(characters) - 2);
    QByteArray data;
    data.reserve(nonceSize);
    for (quint8 i = 0; i < length; ++i) {
        data.append(characters[distribution(randomEngine)]);
    }
    return data;
}

namespace
{
QByteArray GetSecret(const QString& clientSharedKey, const QString& tokenSecret)
{
    QByteArray secret;
    secret.append(QUrl::toPercentEncoding(clientSharedKey));
    secret.append('&');
    secret.append(QUrl::toPercentEncoding(tokenSecret));
    return secret;
}
}

QByteArray OAuth1::GenerateSignature(const QVariantMap& parameters, const QUrl& url,
        QNetworkAccessManager::Operation operation) const
{
    QMessageAuthenticationCode code(QCryptographicHash::Sha1);
    code.setKey(d->secret());
    code.addData(d->signatureBaseString());
    return code.result();
}
} // namespace Sailreads
