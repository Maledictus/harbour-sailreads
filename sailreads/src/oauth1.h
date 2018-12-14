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

#pragma once

#include <QNetworkAccessManager>
#include <QObject>
#include <QUrl>

class QNetworkReply;

namespace Sailreads
{
class OAuth1 : public QObject
{
    Q_OBJECT

    QNetworkAccessManager *m_NAM;
    const QString m_ConsumerKey;
    const QString m_ConsumerSecretKey;

    const QString m_OAuth1Version = QStringLiteral("1.0");
    const QString m_OAuth1Signature = QStringLiteral("HMAC-SHA1");
public:
    struct OAuth1Keys
    {
        static const QString oauthCallback;
        static const QString oauthCallbackConfirmed;
        static const QString oauthConsumerKey;
        static const QString oauthNonce;
        static const QString oauthSignature;
        static const QString oauthSignatureMethod;
        static const QString oauthTimestamp;
        static const QString oauthToken;
        static const QString oauthTokenSecret;
        static const QString oauthVerifier;
        static const QString oauthVersion;
    };

    OAuth1(const QString& consumerKey, const QString& consumerSecretKey,
            QNetworkAccessManager *manager, QObject *parent = nullptr);

    QPair<QString, QString> GetClientCredentials() const;

    QNetworkReply* RequestTemporaryCredentials(QNetworkAccessManager::Operation operation,
            const QUrl& url);
    QNetworkReply* RequestTokenCredentials(QNetworkAccessManager::Operation operation,
            const QUrl& url);

private:
    void AppendCommonHeader(QVariantMap& headers);
    void AppendOAuthSignature(QVariantMap& headers, const QUrl& url,
            QNetworkAccessManager::Operation operation, const QVariantMap& parameters);
    QString GenerateNonce() const;
    QByteArray GenerateSignature(const QVariantMap&parameters, const QUrl& url,
            QNetworkAccessManager::Operation operation) const;
};
} // namespace Sailreads
