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

import QtQuick 2.2
import Sailfish.Silica 1.0
import Nemo.Notifications 1.0
import harbour.sailreads 1.0
import "cover"
import "pages"

ApplicationWindow {
    id: mainWindow

    cover: CoverPage { }

    _defaultPageOrientations: Orientation.All

    property bool alreadyLogged: accountSettings.value("access_token", "").length > 0 &&
            accountSettings.value("access_token_secret", "").length > 0

    initialPage: alreadyLogged ? splashScreenComponent : loginComponent

    Notification {
        id: notification
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
    }

    Component {
        id: loginComponent
        LoginPage {}
    }

    function openInBrowser(url) {
        if (applicationSettings.value("main/embeded_browser", true) === true) {
            pageStack.push(Qt.resolvedUrl("pages/WebViewPage.qml"), { url: url })
        }
        else {
            Qt.openUrlExternally(url)
        }
    }

    function openPageFromUrl(url) {
        var str = url.toString();
        var id = str.substr(str.lastIndexOf('/') + 1)
        if (str.indexOf("www.goodreads.com/topic/show/", 0) > 0) {
            console.log("TOPIC: ", id)
        }
        else if (str.indexOf("www.goodreads.com/read_statuses/", 0) > 0) {
            console.log("READ_STATUS: ", id)
        }
        else if (str.indexOf("www.goodreads.com/review/show/", 0) > 0) {
            console.log("REVIEW: ", id)
        }
        else if (str.indexOf("www.goodreads.com/user/show/", 0) > 0) {
            console.log("USER: ", id)
        }
        else if (str.indexOf("www.goodreads.com/book/show/", 0) > 0) {
            console.log("BOOK: ", id)
        }
        else if (str.indexOf("www.goodreads.com/author/show/", 0) > 0) {
            console.log("AUTHOR: ", id)
        }
        else if (str.indexOf("www.goodreads.com/user_status/show/", 0) > 0) {
            console.log("USER_STATUS: ", id)
        }
        else if (str.indexOf("www.goodreads.com/recommendations/", 0) > 0) {
            console.log("RECOMMENDATION: ", id)
        }
        else if (str.indexOf("www.goodreads.com/user_challenges/", 0) > 0) {
            console.log("USER_CHALLENGE: ", id)
        }
        else {
            console.log("OPEN IN BROWSER: ", url)
//            openInBrowser(url)
        }
    }
}
