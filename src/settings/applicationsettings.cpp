﻿/*
The MIT License (MIT)

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

#include "applicationsettings.h"

#include <QtDebug>
#include <QCoreApplication>
#include <QDir>
#include <QStandardPaths>

namespace Sailreads
{
ApplicationSettings::ApplicationSettings(QObject *parent)
: QObject(parent)
, m_Settings(QDir(QStandardPaths::writableLocation(QStandardPaths::ConfigLocation))
             .filePath(QCoreApplication::applicationName()) + "/sailreads.conf",
        QSettings::IniFormat)
, m_UseEmbededBrowser(value("main/useEmbededBrowser", true).toBool())
, m_ShowYourRecentUpdates(value("main/showRecentUpdates", true).toBool())
, m_ShowFriendsUpdates(value("friends/showFriendsUpdates", false).toBool())
{
#ifdef QT_DEBUG
    qDebug() << this << "CONSTRUCTED";
#endif
}

ApplicationSettings::~ApplicationSettings()
{
#ifdef QT_DEBUG
    qDebug() << this << "DESTRUCTED";
#endif

    m_Settings.setValue("main/useEmbededBrowser", m_UseEmbededBrowser);
    m_Settings.setValue("main/showRecentUpdates", m_ShowYourRecentUpdates);
    m_Settings.setValue("friends/showFriendsUpdates", m_ShowFriendsUpdates);
    m_Settings.sync();
}

ApplicationSettings* ApplicationSettings::Instance(QObject *parent)
{
    static ApplicationSettings *appSettings = nullptr;
    if (!appSettings)
    {
        appSettings = new ApplicationSettings(parent);
    }
    return appSettings;
}

QVariant ApplicationSettings::value(const QString& key, const QVariant& def) const
{
    return m_Settings.value(key, def);
}

void ApplicationSettings::setValue(const QString& key, const QVariant& value)
{
    m_Settings.setValue(key, value);
    m_Settings.sync();
}

void ApplicationSettings::remove(const QString& key)
{
    m_Settings.remove(key);
    m_Settings.sync();
}
} // namespace Sailreads

