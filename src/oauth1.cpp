/*
Copyright (c) 2018-2019 Oleg Linkin <maledictusdemagog@gmail.com>

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
#include <QUrlQuery>

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
    return RequestToken(operation, url, QString(), QString());
}

QNetworkReply* OAuth1::RequestTokenCredentials(QNetworkAccessManager::Operation operation,
        const QUrl& url, const QString& requestToken, const QString& requestTokenSecret)
{
    return RequestToken(operation, url, requestToken, requestTokenSecret);
}

namespace
{
QByteArray generateAuthorizationHeader(const QVariantMap& oauthParams)
{
    // TODO Add realm parameter support
    bool first = true;
    QString ret(QStringLiteral("OAuth "));
    QVariantMap headers(oauthParams);
    for (auto it = headers.begin(), end = headers.end(); it != end; ++it) {
        if (first)
            first = false;
        else
            ret += QLatin1String(",");
        ret += it.key() +
               QLatin1String("=\"") +
               QString::fromUtf8(QUrl::toPercentEncoding(it.value().toString())) +
               QLatin1Char('\"');
    }
    return ret.toUtf8();
}

QUrlQuery createQuery(const QVariantMap &parameters)
{
    QUrlQuery query;
    for (auto it = parameters.begin(), end = parameters.end(); it != end; ++it)
        query.addQueryItem(it.key(), it.value().toString());
    return query;
}

QByteArray convertParameters(const QVariantMap &parameters)
{
    QByteArray data;
    QUrlQuery query;
    for (auto it = parameters.begin(), end = parameters.end(); it != end; ++it)
        query.addQueryItem(it.key(), it->toString());
    data = query.toString(QUrl::FullyEncoded).toUtf8();
    return data;
}

QByteArray parameterString(const QVariantMap &parameters)
{
    QByteArray ret;
    auto previous = parameters.end();
    for (auto it = parameters.begin(), end = parameters.end(); it != end; previous = it++) {
        if (previous != parameters.end()) {
            ret.append("&");
        }
        ret.append(QUrl::toPercentEncoding(it.key()));
        ret.append("=");
        ret.append(QUrl::toPercentEncoding(it.value().toString()));
    }
    return ret;
}

QByteArray encodeHeaders(const QVariantMap &headers)
{
    return QUrl::toPercentEncoding(QString::fromLatin1(parameterString(headers)));
}

QByteArray GetSignatureBaseString(QNetworkAccessManager::Operation method, const QUrl& url,
        const QVariantMap& parameters)
{
    // https://tools.ietf.org/html/rfc5849#section-3.4.1
    QByteArray base;

    switch (method) {
    case QNetworkAccessManager::HeadOperation:
        base.append("HEAD");
        break;
    case QNetworkAccessManager::GetOperation:
        base.append("GET");
        break;
    case QNetworkAccessManager::PutOperation:
        base.append("PUT");
        break;
    case QNetworkAccessManager::PostOperation:
        base.append("POST");
        break;
    case QNetworkAccessManager::DeleteOperation:
        base.append("DELETE");
        break;
    default:
        break;
    }
    base.append('&');
    base.append(QUrl::toPercentEncoding(url.toString(QUrl::RemoveQuery)) + "&");

    QVariantMap p = parameters;
    {
        // replace '+' with spaces now before decoding so that '%2B' gets left as '+'
        const QString query = url.query().replace(QLatin1Char('+'), QLatin1Char(' '));
        const auto queryItems = QUrlQuery(query).queryItems(QUrl::FullyDecoded);
        for (auto it = queryItems.begin(), end = queryItems.end(); it != end; ++it) {
            p.insert(it->first, it->second);
        }
    }
    base.append(encodeHeaders(p));
    return base;
}

QByteArray GetSecret(const QString& clientSharedKey, const QString& tokenSecret)
{
    QByteArray secret;
    secret.append(QUrl::toPercentEncoding(clientSharedKey));
    secret.append('&');
    secret.append(QUrl::toPercentEncoding(tokenSecret));
    return secret;
}

QByteArray GenerateSignature(const QVariantMap& parameters, const QUrl& url,
        QNetworkAccessManager::Operation operation, const QString& customerSecretKey,
        const QString& accessTokenSecret)
{
    QMessageAuthenticationCode code(QCryptographicHash::Sha1);
    code.setKey(GetSecret(customerSecretKey, accessTokenSecret));
    code.addData(GetSignatureBaseString(operation, url, parameters));
    return code.result().toBase64();
}

void AppendOAuthSignature(QVariantMap& headers, const QUrl& url,
        QNetworkAccessManager::Operation operation, const QVariantMap& parameters,
        const QString& customerSecretKey, const QString& accessTokenSecret)
{
    QByteArray signature;
    {
        QVariantMap allParameters = QVariantMap(headers).unite(parameters);
        signature = GenerateSignature(allParameters, url, operation, customerSecretKey, accessTokenSecret);
    }
    headers.insert(OAuth1::OAuth1Keys::oauthSignature, signature);
}

QString GenerateNonce()
{
    static const int nonceSize = 8;
    const char characters[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
    static std::mt19937 randomEngine(QDateTime::currentDateTime().toMSecsSinceEpoch());
    std::uniform_int_distribution<int> distribution(0, sizeof(characters) - 2);
    QByteArray data;
    data.reserve(nonceSize);
    for (quint8 i = 0; i < nonceSize; ++i) {
        data.append(characters[distribution(randomEngine)]);
    }
    return data;
}
}

QNetworkReply* OAuth1::Get(const QString& accessToken, const QString& accessTokenSecret,
        const QUrl& url, const QVariantMap& parameters)
{
    QNetworkRequest request(url);
    SignRequest(request, parameters, QNetworkAccessManager::GetOperation,
            accessToken, accessTokenSecret);
    return m_NAM->get(request);
}

QNetworkReply* OAuth1::Post(const QString& accessToken, const QString& accessTokenSecret,
        const QUrl& url, const QVariantMap& parameters)
{
    QNetworkRequest request(url);
    SignRequest(request, parameters, QNetworkAccessManager::PostOperation,
            accessToken, accessTokenSecret);
    request.setHeader(QNetworkRequest::ContentTypeHeader,
            QStringLiteral("application/x-www-form-urlencoded"));

    const QByteArray data = convertParameters(parameters);
    return m_NAM->post(request, data);
}

QNetworkReply* OAuth1::Put(const QString& accessToken, const QString& accessTokenSecret,
        const QUrl& url, const QVariantMap& parameters)
{
    QNetworkRequest request(url);
    SignRequest(request, parameters, QNetworkAccessManager::PutOperation,
            accessToken, accessTokenSecret);
    request.setHeader(QNetworkRequest::ContentTypeHeader,
            QStringLiteral("application/x-www-form-urlencoded"));

    const QByteArray data = convertParameters(parameters);
    return m_NAM->put(request, data);
}

QNetworkReply* OAuth1::DeleteResource(const QString& accessToken, const QString& accessTokenSecret,
        const QUrl& url, const QVariantMap& parameters)
{
    QNetworkRequest request(url);
    SignRequest(request, parameters, QNetworkAccessManager::DeleteOperation,
            accessToken, accessTokenSecret);
    return m_NAM->deleteResource(request);
}

void OAuth1::AppendCommonHeaders(QVariantMap& headers)
{
    const auto currentDateTime = QDateTime::currentDateTimeUtc();
    headers.insert(OAuth1Keys::oauthNonce, GenerateNonce());
    headers.insert(OAuth1Keys::oauthConsumerKey, m_ConsumerKey);
    headers.insert(OAuth1Keys::oauthTimestamp, QString::number(currentDateTime.toTime_t()));
    headers.insert(OAuth1Keys::oauthVersion, m_OAuth1Version);
    headers.insert(OAuth1Keys::oauthSignatureMethod, m_OAuth1Signature);
}

QNetworkReply *OAuth1::RequestToken(QNetworkAccessManager::Operation operation,
        const QUrl& url, const QString& token, const QString& tokenSecret,
        const QVariantMap& parameters)
{
    if (!m_NAM)
    {
        qWarning() << "NatworkAccessManager is null in OAuth1";
        return nullptr;
    }

    if (url.isEmpty())
    {
        qWarning() << "Url for request token method is empty";
        return nullptr;
    }

    if (operation != QNetworkAccessManager::GetOperation &&
            operation != QNetworkAccessManager::PostOperation)
    {
        qWarning() << "RequstingToken only support GET and POST operations";
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

    if (!token.isEmpty()) {
        headers.insert(OAuth1Keys::oauthToken, token);
    }

    AppendOAuthSignature(headers, url, operation, remainingParameters,
            m_ConsumerSecretKey, tokenSecret);
    request.setRawHeader("Authorization", generateAuthorizationHeader(headers));
    QNetworkReply *reply = nullptr;
    if (operation == QNetworkAccessManager::GetOperation) {
        if (parameters.size() > 0) {
            QUrl url = request.url();
            url.setQuery(createQuery(remainingParameters));
            request.setUrl(url);
        }
        reply = m_NAM->get(request);
    }
    else if (operation == QNetworkAccessManager::PostOperation) {
        QUrlQuery query = createQuery(remainingParameters);
        const QByteArray data = query.toString(QUrl::FullyEncoded).toUtf8();
        request.setHeader(QNetworkRequest::ContentTypeHeader,
                QStringLiteral("application/x-www-form-urlencoded"));
        reply = m_NAM->post(request, data);
    }
    return reply;
}

void OAuth1::SignRequest(QNetworkRequest& request, const QVariantMap& signingParameters,
        QNetworkAccessManager::Operation operation, const QString& accessToken,
        const QString& accessTokenSecret)
{
    QVariantMap oauthParams;
    const auto currentDateTime = QDateTime::currentDateTimeUtc();

    oauthParams.insert(OAuth1Keys::oauthConsumerKey, m_ConsumerKey);
    oauthParams.insert(OAuth1Keys::oauthVersion, m_OAuth1Version);
    oauthParams.insert(OAuth1Keys::oauthToken, accessToken);
    oauthParams.insert(OAuth1Keys::oauthSignatureMethod, m_OAuth1Signature);
    oauthParams.insert(OAuth1Keys::oauthNonce, GenerateNonce());
    oauthParams.insert(OAuth1Keys::oauthTimestamp, QString::number(currentDateTime.toTime_t()));

    // Add signature parameter
    {
        const auto parameters = QVariantMap(oauthParams).unite(signingParameters);
        const auto signature = GenerateSignature(parameters, request.url(), operation,
                m_ConsumerSecretKey, accessTokenSecret);
        oauthParams.insert(OAuth1Keys::oauthSignature, signature);
    }

    if (operation == QNetworkAccessManager::GetOperation) {
        if (signingParameters.size()) {
            QUrl url = request.url();
            QUrlQuery query = QUrlQuery(url.query());
            for (auto it = signingParameters.begin(), end = signingParameters.end(); it != end;
                 ++it)
                query.addQueryItem(it.key(), it.value().toString());
            url.setQuery(query);
            request.setUrl(url);
        }
    }

    request.setRawHeader("Authorization", generateAuthorizationHeader(oauthParams));

    if (operation == QNetworkAccessManager::PostOperation ||
            operation == QNetworkAccessManager::PutOperation) {
        request.setHeader(QNetworkRequest::ContentTypeHeader,
                QStringLiteral("application/x-www-form-urlencoded"));
    }
}
} // namespace Sailreads
