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
    id: authPage

    SilicaWebView {
        id: webview
        anchors.fill: parent

        experimental.preferences.navigatorQtObjectEnabled: true
        experimental.userAgent: "Mozilla/5.0 (Maemo; Linux; U; Sailfish; Mobile; rv:38.0) Gecko/38.0 Firefox/38.0"

        onLoadingChanged: {
            if (loadRequest.status !== WebView.LoadSucceededStatus) {
                return;
            }

            webviewBusyIndicator.visible = false
            var str = loadRequest.url.toString();
            var success = str.indexOf ("authorize=1", 0)
            var fail = str.indexOf ("authorize=0", 0)
            if (success > 0) {
                webview.stop()
                sailreadsManager.requestAccessToken();
            } else if (fail > 0) {
                webview.stop()
                webview.visible = false;
                messageLabel.text = qsTr("Authorization failed")
                processIndicator.visible = false
            }
        }
    }

    BusyIndicator {
        id: webviewBusyIndicator
        anchors.centerIn: parent
        visible: true;
        running: true
        size: BusyIndicatorSize.Large
    }

    Connections {
        target: sailreadsManager
        onRequestTokenChanged: {
            webview.url = "https://www.goodreads.com/oauth/authorize?mobile=1&oauth_token=" + requestToken
        }
    }

    Component.onCompleted: {
        sailreadsManager.obtainRequestToken()
    }

    Column {
        anchors.centerIn: parent
        spacing: Theme.paddingMedium
        visible: !webview.visible

        Label {
            id: messageLabel
            text: qsTr("Authorization process...")
            color: Theme.highlightColor
            font.pixelSize: Theme.fontSizeLarge
        }

        BusyIndicator {
            id: processIndicator
            anchors.horizontalCenter: parent.horizontalCenter
            size: BusyIndicatorSize.Large
            running: true
            visible: running
        }
    }
}
