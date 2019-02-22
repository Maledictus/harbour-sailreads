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
import QtWebKit 3.0

Page {
    id: communitiesReviewsPage

    property string content: ""
    property alias url : webView.url

    onContentChanged: {
        webView.loadHtml(content)
    }

    SilicaWebView {
        id: webView
        anchors.fill: parent

        header: PageHeader{
            title: qsTr("Community Reviews")
        }

        experimental.userAgent: "Mozilla/5.0 (Maemo; Linux; U; Sailfish; Mobile; rv:38.0) Gecko/38.0 Firefox/38.0"

        experimental.preferences.webGLEnabled: true
        experimental.preferences.notificationsEnabled: true
        experimental.preferences.javascriptEnabled: true
        experimental.preferences.localStorageEnabled: true
        experimental.overview: true

        experimental.userScripts: [ Qt.resolvedUrl("../helper/userscript.js") ]
        experimental.preferences.navigatorQtObjectEnabled: true

        experimental.onMessageReceived: {
            console.log("AAAA", message.data)
            var data = null
            try {
                data = JSON.parse(message.data)
            } catch (error) {
                console.log('onMessageReceived: ' + message.data );
                return
            }
            if (data.type === 'link') {
                if (data.target === '_blank') {
                    mainWindow.openPageFromUrl(data.href)
                }
                else if (data.target && data.target !== "_parent") {
                    mainWindow.openPageFromUrl(data.href)
                }
            }
        }
    }

    BusyIndicator {
        id: webviewBusyIndicator
        anchors.centerIn: parent
        visible: running;
        running: webView.loading
        size: BusyIndicatorSize.Large
    }
}
