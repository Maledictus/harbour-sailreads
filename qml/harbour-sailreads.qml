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

import QtQuick 2.6
import Sailfish.Silica 1.0
import Nemo.Notifications 1.0
import harbour.sailreads 1.0
import "cover"
import "pages"
import "dialogs"

ApplicationWindow {
    id: mainWindow

    cover: CoverPage { model: currentlyReadingModel }

    _defaultPageOrientations: Orientation.All

    property bool alreadyLogged: accountSettings.value("access_token", "").length > 0 &&
            accountSettings.value("access_token_secret", "").length > 0

    initialPage: alreadyLogged ? splashScreenComponent : loginComponent

    property alias currentlyReadingModel: currentlyReadingStatusModel

    Notification {
        id: notification
    }

    ReviewsModel {
        id: currentlyReadingStatusModel
        userId: sailreadsManager.authUser ? sailreadsManager.authUser.id : ""
        bookShelf: "currently-reading"
        sortField: "date_updated"
        sortOrder: Qt.DescendingOrder
        onBookShelfIdChanged: {
            if (sailreadsManager.authUser && sailreadsManager.authUser.id === userId) {
                loadReviews()
            }
        }
    }

    Connections {
        target: sailreadsManager
        onLoggedChanged: {
            if (!sailreadsManager.logged) {
                pageStack.completeAnimation()
                pageStack.clear()
                pageStack.push(Qt.resolvedUrl("pages/LoginPage.qml"))
            }
            else {
                pageStack.completeAnimation()
                pageStack.clear()
                pageStack.push(Qt.resolvedUrl("pages/SplashScreenPage.qml"))
            }
        }
    }

    Component {
        id: splashScreenComponent
        SplashScreenPage {}
        //12934309 171430
        //BookPage { bookId: "41045262" }
        //UpdateReadingProgressDialog{ bookId: "41045262" }
        //ProfilePage { userId: "171430" }
    }

    Component {
        id: loginComponent
        LoginPage {}
    }

    function openInBrowser(url) {
        if (applicationSettings.useEmbededBrowser) {
            pageStack.push(Qt.resolvedUrl("pages/WebViewPage.qml"), { url: url })
        }
        else {
            Qt.openUrlExternally(url)
        }
    }

    function openPageFromUrl(url) {
        var str = url.toString();
        var match = /(https?:\/\/)?www.goodreads.com\/(.*)\/(\d+)/.exec(url)
        if (match === null) {
            openInBrowser(url)
            return
        }

        var path = match[2]
        var id = match[3]
        switch (path) {
        case "topic/show":
            pageStack.push(Qt.resolvedUrl("pages/GroupFolderTopicPage.qml"),
                    { topicId: id })
            break
        case "read_statuses":
            pageStack.push(Qt.resolvedUrl("pages/ReadStatusPage.qml"),
                    { readStatusId: id })
            break
        case "review/show":
            pageStack.push(Qt.resolvedUrl("pages/ReviewPage.qml"),
                    { reviewId: id })
            break
        case "user/show":
            pageStack.push(Qt.resolvedUrl("pages/ProfilePage.qml"),
                    { userId : id })
            break
        case "book/show":
            pageStack.push(Qt.resolvedUrl("pages/BookPage.qml"),
                    { bookId: id })
            break
        case "author/show":
            pageStack.push(Qt.resolvedUrl("pages/AuthorPage.qml"),
                    { authorId: id })
            break
        case "user_status/show":
            pageStack.push(Qt.resolvedUrl("pages/UserStatusPage.qml"),
                    { userStatusId: id })
            break
        case "recommendations":
            pageStack.push(Qt.resolvedUrl("pages/RecommendationPage.qml"),
                    { recommendationId: id })
            break
        default:
            openInBrowser(url)
            break
        }
    }
}
