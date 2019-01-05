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

#include "application.h"

#include "sailfishapp.h"

#include <QQuickView>
#include <QQmlContext>
#include <QtDebug>
#include <QtQml>


#include "models/authorbooksmodel.h"
#include "models/authorseriesmodel.h"
#include "models/baseproxymodel.h"
#include "models/bookshelvesmodel.h"
#include "models/bookeditionsmodel.h"
#include "models/topiccommentsmodel.h"
#include "models/reviewcommentsmodel.h"
#include "models/friendsmodel.h"
#include "models/followersmodel.h"
#include "models/followingsmodel.h"
#include "models/usergroupsmodel.h"
#include "models/foundgroupsmodel.h"
#include "models/groupfoldersmodel.h"
#include "models/groupmembersmodel.h"
#include "models/groupfoldertopicsmodel.h"
#include "models/reviewsmodel.h"
#include "objects/author.h"
#include "objects/book.h"
#include "objects/group.h"
#include "objects/review.h"
#include "objects/series.h"
#include "objects/serieswork.h"
#include "objects/user.h"
#include "objects/work.h"
#include "objects/groupfolder.h"
#include "objects/topic.h"
#include "qmltypes/authorprofileitem.h"
#include "qmltypes/bookitem.h"
#include "qmltypes/groupitem.h"
#include "qmltypes/reviewitem.h"
#include "qmltypes/seriesitem.h"
#include "qmltypes/topicitem.h"
#include "qmltypes/userprofile.h"
#include "settings/accountsettings.h"
#include "settings/applicationsettings.h"
#include "authserver.h"
#include "sailreadsmanager.h"

namespace Sailreads
{
Application::Application(QObject *parent)
: QObject(parent)
, m_View(nullptr)
, m_AuthServer(new AuthServer(this))
{
    connect(m_AuthServer, &AuthServer::gotAuthAnswer,
            this, &Application::handleAuthAnswerGot);
}

QString Application::GetPath(ApplicationDirectory subdirectory)
{
    switch (subdirectory) {
    case ApplicationDirectory::AppDataDirectory:
        return QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    case ApplicationDirectory::CacheDirectory:
        return QStandardPaths::writableLocation(QStandardPaths::CacheLocation);
    default:
        return "";
    }
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

        const bool result = m_AuthServer->StartListening(QHostAddress::LocalHost, PORT);
        m_View->rootContext()->setContextProperty("authServerRunning", result);

        m_View->setSource(SailfishApp::pathTo("qml/harbour-sailreads.qml"));
        m_View->showFullScreen();

        connect(SailreadsManager::Instance(this), &SailreadsManager::loggedChanged,
                this, &Application::handleLogged);
    }
    else
    {
        qDebug() << "Activating view";
        m_View->raise();
        m_View->requestActivate();
    }
}

void Application::handleAuthAnswerGot(const QString& data)
{
    SailreadsManager::Instance(this)->handleGotAuthAnswer(data);
}

void Application::handleLogged()
{
    if (m_AuthServer)
    {
        m_AuthServer->SendAnswer(SailreadsManager::Instance(this)->GetLogged() ?
                tr("Authorized") : tr("Not authorized"));
    }
}

void Application::start()
{
    qmlRegisterUncreatableType<SailreadsManager>("harbour.sailreads", 1, 0, "SailreadsManager",
            "SailreadsManager static uncreatable type");
    qmlRegisterUncreatableType<Author>("harbour.sailreads", 1, 0, "Author",
            "Author provides attached properties and can't be instantiated");
    qmlRegisterUncreatableType<Book>("harbour.sailreads", 1, 0, "Book",
            "Book provides attached properties and can't be instantiated");
    qmlRegisterUncreatableType<Group>("harbour.sailreads", 1, 0, "Group",
            "Group provides attached properties and can't be instantiated");
    qmlRegisterUncreatableType<Review>("harbour.sailreads", 1, 0, "Review",
            "Review provides attached properties and can't be instantiated");
    qmlRegisterUncreatableType<Series>("harbour.sailreads", 1, 0, "Series",
            "Series provides attached properties and can't be instantiated");
    qmlRegisterUncreatableType<SeriesWork>("harbour.sailreads", 1, 0, "SeriesWork",
            "SeriesWork provides attached properties and can't be instantiated");
    qmlRegisterUncreatableType<Topic>("harbour.sailreads", 1, 0, "Topic",
            "Topic provids attached properties and can't be instantiated");
    qmlRegisterUncreatableType<User>("harbour.sailreads", 1, 0, "User",
            "User provids attached properties and can't be instantiated");
    qmlRegisterUncreatableType<Work>("harbour.sailreads", 1, 0, "Work",
            "Work provides attached properties and can't be instantiated");

    qRegisterMetaType<QObjectList>("QObjectList");
    qRegisterMetaType<Author*>("Author*");
    qRegisterMetaType<Book*>("Book*");
    qRegisterMetaType<Group*>("Group*");
    qRegisterMetaType<Review*>("Review*");
    qRegisterMetaType<Series*>("Series*");
    qRegisterMetaType<SeriesWork*>("SeriesWork*");
    qRegisterMetaType<Topic*>("Topic*");
    qRegisterMetaType<User*>("User*");
    qRegisterMetaType<Work*>("Work*");
    qRegisterMetaType<GroupFolder>("GroupFolder");

    qmlRegisterType<AuthorBooksModel>("harbour.sailreads", 1, 0, "AuthorBooksModel");
    qmlRegisterType<AuthorSeriesModel>("harbour.sailreads", 1, 0, "AuthorSeriesModel");
    qmlRegisterType<BaseProxyModel>("harbour.sailreads", 1, 0, "BaseProxyModel");
    qmlRegisterType<BookShelvesModel>("harbour.sailreads", 1, 0, "BookShelvesModel");
    qmlRegisterType<BookEditionsModel>("harbour.sailreads", 1, 0, "BookEditionsModel");
    qmlRegisterType<FriendsModel>("harbour.sailreads", 1, 0, "FriendsModel");
    qmlRegisterType<FollowersModel>("harbour.sailreads", 1, 0, "FollowersModel");
    qmlRegisterType<FollowingsModel>("harbour.sailreads", 1, 0, "FollowingsModel");
    qmlRegisterType<UserGroupsModel>("harbour.sailreads", 1, 0, "UserGroupsModel");
    qmlRegisterType<FoundGroupsModel>("harbour.sailreads", 1, 0, "FoundGroupsModel");
    qmlRegisterType<GroupFoldersModel>("harbour.sailreads", 1, 0, "GroupFoldersModel");
    qmlRegisterType<GroupMembersModel>("harbour.sailreads", 1, 0, "GroupMembersModel");
    qmlRegisterType<GroupFolderTopicsModel>("harbour.sailreads", 1, 0, "GroupFolderTopicsModel");
    qmlRegisterType<TopicCommentsModel>("harbour.sailreads", 1, 0, "TopicCommentsModel");
    qmlRegisterType<ReviewCommentsModel>("harbour.sailreads", 1, 0, "ReviewCommentsModel");
    qmlRegisterType<ReviewsModel>("harbour.sailreads", 1, 0, "ReviewsModel");
    qmlRegisterType<UserProfile>("harbour.sailreads", 1, 0, "UserProfile");
    qmlRegisterType<GroupItem>("harbour.sailreads", 1, 0, "GroupItem");
    qmlRegisterType<TopicItem>("harbour.sailreads", 1, 0, "TopicItem");
    qmlRegisterType<BookItem>("harbour.sailreads", 1, 0, "BookItem");
    qmlRegisterType<SeriesItem>("harbour.sailreads", 1, 0, "SeriesItem");
    qmlRegisterType<AuthorProfileItem>("harbour.sailreads", 1, 0, "AuthorProfileItem");
    qmlRegisterType<User>("harbour.sailreads", 1, 0, "User");
    qmlRegisterType<ReviewItem>("harbour.sailreads", 1, 0, "ReviewItem");

    ShowUI();
}

void Application::handleAboutToQuit()
{
//    LinksBagManager::Instance(this)->saveBookmarks();
}
} // namespace Sailreads

