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
#include "userprofile.h"

namespace Sailreads
{
SailreadsManager::SailreadsManager(QObject *parent)
: QObject(parent)
, m_Api(new GoodReadsApi(this))
, m_IsBusy(false)
, m_IsLogged(false)
, m_Profile(new UserProfile(this))
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

UserProfile* SailreadsManager::GetProfile() const
{
    return m_Profile;
}

void SailreadsManager::MakeConnections()
{
    connect(m_Api, &GoodReadsApi::requestFinished,
            this, [=](){ SetBusy(false); });

    connect(m_Api, &GoodReadsApi::requestTokenChanged,
            this, &SailreadsManager::requestTokenChanged);
    connect(m_Api, &GoodReadsApi::accessTokensChanged,
            this,
            [=](const QString& accessToken, const QString& accessTokenSecret) {
                AccountSettings::Instance(this)->setValue("access_token", accessToken);
                AccountSettings::Instance(this)->setValue("access_token_secret", accessTokenSecret);
                m_AccessToken = accessToken;
                m_AccessSecretToken = accessTokenSecret;
                if (!m_AccessToken.isEmpty() && !m_AccessSecretToken.isEmpty()) {
                    m_Api->UpdateCredentials(m_AccessToken, m_AccessSecretToken);
                }
                SetLogged(!accessToken.isEmpty() && !accessTokenSecret.isEmpty());
            });

    connect(m_Api, &GoodReadsApi::gotAuthUserInfo,
            this,
            [=](const quint64& id, const QString& name, const QString& link) {
                if (!m_Profile) {
                    qWarning() << Q_FUNC_INFO << "Invalid profile";
                    return;
                }
                m_Profile->SetUserID(id);
                m_Profile->SetUserName(name);
                m_Profile->SetWebUrl(link);
                getUserInfo(id);
            });
    connect(m_Api, &GoodReadsApi::gotUserProfile,
            this,
            [=](std::shared_ptr<UserProfile> profile) {
                if (profile && m_Profile && profile->GetUserID() == m_Profile->GetUserID()) {
                    qDebug() << "Authenticated profile got";
                    m_Profile->Update(profile);
                    emit profileChanged();
                    emit gotUserProfile();
                }
                else if (profile) {
                    qDebug() << "User profile got";
                    //TODO
                }
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
    if (m_IsLogged) {
        m_Api->UpdateCredentials(m_AccessToken, m_AccessSecretToken);
    }
    emit loggedChanged();
}

void SailreadsManager::obtainRequestToken()
{
    SetBusy(true);
    emit authProgressChanged(tr("Authorization user..."));
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
    emit authProgressChanged(tr("Authentication user..."));
    m_Api->AuthUser();
}

void SailreadsManager::getUserInfo(quint64 id)
{
    SetBusy(true);
    emit authProgressChanged(tr("Getting user profile..."));
    m_Api->GetUserInfo(id);
}

void SailreadsManager::getUpdates()
{
    SetBusy(true);
    m_Api->GetUpdates();
}

void SailreadsManager::loadBookShelves()
{
    SetBusy(true);
    m_Api->GetBookShelves();
}
}
