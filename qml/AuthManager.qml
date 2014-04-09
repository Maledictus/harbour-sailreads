/*
    Copyright (c) 2014 Oleg Linkin <MaledictusDeMagog@gmail.com>

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

import QtQuick 2.0
import QtWebKit 3.0
import Sailfish.Silica 1.0

Item {
    id: authManager

    property int countLoading: 0

    signal authorized (bool success);

    function authorizeApplication (url) {
        webview.visible = true
        webview.url = url
    }

    WebView {
        id: webview
        anchors.fill: parent

        visible: false

        onLoadingChanged: {

            console.log("webview "  + loading)
            if (loading) {
                countLoading++;
            } else {
                countLoading--;

                var str = loadRequest.url.toString ();

                var success = str.indexOf ("authorize=1", 0)
                var fail = str.indexOf ("authorize=0", 0)
                if (success > 0) {
                    authorized (true)
                    webview.visible = false;
                } else if (fail > 0) {
                    authorized (false)
                    webview.visible = false;
                }
            }
        }

        BusyIndicator {
            id: webviewBusyIndicator
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.bottom: parent.bottom
            visible: webview.loading;
            running: true;
        }

    }
}
