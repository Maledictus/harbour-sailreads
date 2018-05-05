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

#include "application.h"

#include "sailfishapp.h"

#include <QQuickView>
#include <QQmlContext>
#include <QtDebug>
#include <QtQml>

#include "models/reviewsmodel.h"
#include "models/bookshelvesmodel.h"
#include "models/bookshelfproxymodel.h"
#include "models/commentsmodel.h"
#include "models/friendsmodel.h"
#include "models/usergroupsmodel.h"
#include "models/foundgroupsmodel.h"
#include "models/groupfoldersmodel.h"
#include "models/groupmembersmodel.h"
#include "models/groupfoldertopicsmodel.h"
#include "objects/author.h"
#include "objects/book.h"
#include "objects/group.h"
#include "objects/user.h"

#include "objects/groupfolder.h"
#include "objects/topic.h"
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
    qmlRegisterUncreatableType<SailreadsManager>("harbour.sailreads", 1, 0, "SailreadsManager",
            "SailreadsManager static uncreatable type");
    qmlRegisterUncreatableType<Author>("harbour.sailreads", 1, 0, "Author",
            "Author provids attached properties and can't be instantiated");
    qmlRegisterUncreatableType<Book>("harbour.sailreads", 1, 0, "Book",
            "Book provids attached properties and can't be instantiated");
    qmlRegisterUncreatableType<Group>("harbour.sailreads", 1, 0, "Group",
            "Group provids attached properties and can't be instantiated");
    qmlRegisterUncreatableType<User>("harbour.sailreads", 1, 0, "User",
            "User provids attached properties and can't be instantiated");

    qRegisterMetaType<Author*>("Author*");
    qRegisterMetaType<Book*>("Book*");
    qRegisterMetaType<Group*>("Group*");
    qRegisterMetaType<User*>("User*");

    qRegisterMetaType<GroupFolder>("GroupFolder");
    qRegisterMetaType<Topic>("Topic");
    qRegisterMetaType<Review>("Review");
    qRegisterMetaType<SeriesWork>("SeriesWork");
    qRegisterMetaType<Series>("Series");

    qmlRegisterType<BaseProxyModel>("harbour.sailreads", 1, 0, "BaseProxyModel");
    qmlRegisterType<BookShelvesModel>("harbour.sailreads", 1, 0, "BookShelvesModel");
    qmlRegisterType<BookShelfProxyModel>("harbour.sailreads", 1, 0, "BookShelfProxyModel");
    qmlRegisterType<FriendsModel>("harbour.sailreads", 1, 0, "FriendsModel");
    qmlRegisterType<UserGroupsModel>("harbour.sailreads", 1, 0, "UserGroupsModel");
    qmlRegisterType<FoundGroupsModel>("harbour.sailreads", 1, 0, "FoundGroupsModel");
    qmlRegisterType<GroupFoldersModel>("harbour.sailreads", 1, 0, "GroupFoldersModel");
    qmlRegisterType<GroupMembersModel>("harbour.sailreads", 1, 0, "GroupMembersModel");
    qmlRegisterType<GroupFolderTopicsModel>("harbour.sailreads", 1, 0, "GroupFolderTopicsModel");
    qmlRegisterType<CommentsModel>("harbour.sailreads", 1, 0, "CommentsModel");
    qmlRegisterType<ReviewsModel>("harbour.sailreads", 1, 0, "ReviewsModel");
    qmlRegisterType<UserProfile>("harbour.sailreads", 1, 0, "UserProfile");

    ShowUI();
}

void Application::handleAboutToQuit()
{
//    LinksBagManager::Instance(this)->saveBookmarks();
}
} // namespace Sailreads
