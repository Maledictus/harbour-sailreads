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

#include "sailreadsmanager.h"

#include <QtDebug>

#include "settings/accountsettings.h"
#include "goodreadsapi.h"

namespace Sailreads
{
SailreadsManager::SailreadsManager(QObject *parent)
: QObject(parent)
, m_Api(new GoodReadsApi(this))
, m_IsBusy(false)
, m_IsLogged(false)
{
    MakeConnections();

    m_AccessToken = AccountSettings::Instance(this)->value("access_token").toString();
    m_AccessSecretToken = AccountSettings::Instance(this)->value("access_token_secret").toString();
    SetLogged(!m_AccessToken.isEmpty() && !m_AccessSecretToken.isEmpty());
}

SailreadsManager *SailreadsManager::Instance(QObject *parent)
{
    static SailreadsManager *sailreadsManager = nullptr;
    if (!sailreadsManager)
    {
        sailreadsManager = new SailreadsManager(parent);
    }
    return sailreadsManager;
}

bool SailreadsManager::GetBusy() const
{
    return m_IsBusy;
}

bool SailreadsManager::GetLogged() const
{
    return m_IsLogged;
}

void SailreadsManager::MakeConnections()
{
    connect(m_Api, &GoodReadsApi::requestFinished,
            this, [=](){ SetBusy(false); });

    connect(m_Api, &GoodReadsApi::requestTokenChanged,
            this, &SailreadsManager::requestTokenChanged);
    connect(m_Api, &GoodReadsApi::accessTokensChanged,
            this,
            [=](const QString& accessToken, const QString& accessTokenSecret)
            {
                AccountSettings::Instance(this)->setValue("access_token", accessToken);
                AccountSettings::Instance(this)->setValue("access_token_secret", accessTokenSecret);
                m_AccessToken = accessToken;
                m_AccessSecretToken = accessTokenSecret;
                SetLogged(!accessToken.isEmpty() && !accessTokenSecret.isEmpty());
            });

    connect(m_Api, &GoodReadsApi::gotAuthUserInfo,
            this,
            [=](const QString& id, const QString& name, const QString& link)
            {
                qDebug() << id << name << link;
            });
}

void SailreadsManager::SetBusy(bool busy)
{
    m_IsBusy = busy;
    emit busyChanged();
}

void SailreadsManager::SetLogged(bool logged)
{
    m_IsLogged = logged;
    emit loggedChanged();
}

void SailreadsManager::obtainRequestToken()
{
    SetBusy(true);
    m_Api->ObtainRequestToken();
}

void SailreadsManager::requestAccessToken()
{
    SetBusy(true);
    m_Api->RequestAccessToken();
}

void SailreadsManager::authUser()
{
    SetBusy(true);
    m_Api->AuthUser(m_AccessToken, m_AccessSecretToken);
}
}
