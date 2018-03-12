﻿/*
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

#include "application.h"

#include "sailfishapp.h"

#include <QQuickView>
#include <QQmlContext>
#include <QtDebug>
#include <QtQml>

#include "models/bookshelvesmodel.h"
#include "settings/accountsettings.h"
#include "settings/applicationsettings.h"
#include "sailreadsmanager.h"
#include "userprofile.h"

namespace Sailreads
{
Application::Application(QObject *parent)
: QObject(parent)
, m_View(nullptr)
{
}

void Application::ShowUI()
{
    if (!m_View)
    {
        qDebug() << "Construct view";
        m_View = SailfishApp::createView();
        m_View->setTitle("Sailreads");
        m_View->rootContext()->setContextProperty("accountSettings",
                AccountSettings::Instance(this));
        m_View->rootContext()->setContextProperty("applicationSettings",
                ApplicationSettings::Instance(this));
        m_View->rootContext()->setContextProperty("sailreadsManager",
                SailreadsManager::Instance(this));

        m_View->setSource(SailfishApp::pathTo("qml/harbour-sailreads.qml"));
        m_View->showFullScreen();
    }
    else
    {
        qDebug() << "Activating view";
        m_View->raise();
        m_View->requestActivate();
    }
}

void Application::start()
{
    qmlRegisterUncreatableType<SailreadsManager>("harbour.sailreads", 1, 0,
            "SailreadsManager", "SailreadsManager static uncreatable type");
    qRegisterMetaType<UserProfile*>("UserProfile*");
    qRegisterMetaType<BookShelvesModel*>("BookShelvesModel*");
/*    qmlRegisterUncreatableType<LinksBag::EnumsProxy>("harbour.linksbag", 1, 0,
            "LinksBag", "This exports otherwise unavailable \
                    LinksBag datatypes to QML");
*/

   ShowUI();
}

void Application::handleAboutToQuit()
{
//    LinksBagManager::Instance(this)->saveBookmarks();
}
} // namespace Sailreads
