/*
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

#pragma once

#include <QObject>
#include <QSettings>
#include <QVariant>

namespace Sailreads
{
class ApplicationSettings : public QObject
{
    Q_OBJECT

    QSettings m_Settings;
    bool m_UseEmbededBrowser;
    bool m_ShowYourRecentUpdates;
    bool m_ShowFriendsUpdates;

    ApplicationSettings(QObject *parent = 0);

    Q_PROPERTY(bool useEmbededBrowser MEMBER m_UseEmbededBrowser NOTIFY useEmbededBrowserChanged)
    Q_PROPERTY(bool showYourRecentUpdates MEMBER m_ShowYourRecentUpdates NOTIFY showYourRecentUpdatesChanged)
    Q_PROPERTY(bool showFriendsUpdates MEMBER m_ShowFriendsUpdates NOTIFY showFriendsUpdatesChanged)
public:
    ~ApplicationSettings();
    ApplicationSettings(const ApplicationSettings& as) = delete;
    void operator=(const ApplicationSettings& as) = delete;

    static ApplicationSettings* Instance(QObject *parent = 0);

    Q_INVOKABLE QVariant value(const QString& key,
            const QVariant& def = QVariant()) const;
    Q_INVOKABLE void setValue(const QString& key, const QVariant& value);
    Q_INVOKABLE void remove(const QString& key);

signals:
    void useEmbededBrowserChanged();
    void showYourRecentUpdatesChanged();
    void showFriendsUpdatesChanged();
};
} // namespace Sailreads
