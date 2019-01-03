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

#ifdef QT_QML_DEBUG
#include <QtQuick>
#endif

#include <QGuiApplication>
#include <QScopedPointer>
#include <QtDebug>
#include <QTimer>

#include <sailfishapp.h>

#include "src/application.h"
#include "src/debugmessagehandler.h"

int main(int argc, char *argv[])
{
    //qInstallMessageHandler(DebugHandler::Write);

    QScopedPointer<QGuiApplication> app(SailfishApp::application(argc, argv));
    app->setApplicationDisplayName("Sailreads");
    app->setApplicationName("harbour-sailreads");
    app->setApplicationVersion(QString(APP_VERSION));

    qDebug() << "====Application starting====" << app->applicationVersion();
    QScopedPointer<Sailreads::Application> application(new Sailreads::Application());
    QTimer::singleShot(1, application.data(), SLOT(start()));

    QObject::connect(app.data(),
            &QGuiApplication::aboutToQuit,
            application.data(),
            &Sailreads::Application::handleAboutToQuit);

    const int retVal = app->exec();

    qDebug() << "====Application closing====";

    return retVal;
}
