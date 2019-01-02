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

import QtQuick 2.2
import Sailfish.Silica 1.0

Page {
    id: splashScreenPage

    onStatusChanged: {
        if (status === PageStatus.Active && sailreadsManager.logged) {
            sailreadsManager.authenticateUser()
        }
    }

    Label {
        id: name

        anchors.bottom: logo.top
        anchors.bottomMargin: Theme.paddingLarge
        anchors.horizontalCenter: logo.horizontalCenter

        color: Theme.highlightColor
        font.pixelSize: Theme.fontSizeExtraLarge
        text: "Sailreads"
    }

    Image {
        id: logo

        anchors.centerIn: parent

        width: 256
        height: 256

        clip: true
        smooth: true
        asynchronous: true
        fillMode: Image.PreserveAspectFit

        sourceSize.width: width
        sourceSize.height: height
        source: "qrc:/images/sailreads256x256.png"
    }

    BusyIndicator {
        size: BusyIndicatorSize.Large
        anchors.top: logo.bottom
        anchors.topMargin: Theme.paddingLarge
        anchors.horizontalCenter: logo.horizontalCenter
        running: sailreadsManager.busy
        visible: running
    }

    Connections {
        target: sailreadsManager
        onAuthProgressChanged: {
            progress.text = progressMessage
        }
        onGotAuthUserId: {
            pageStack.replace(Qt.resolvedUrl("ProfilePage.qml"),
                    { userId: authUserId })
        }
    }

    Label {
        id: progress

        anchors.bottom: parent.bottom
        anchors.bottomMargin: Theme.paddingMedium
        anchors.horizontalCenter: logo.horizontalCenter

        color: Theme.highlightColor
        font.pixelSize: Theme.fontSizeSmall
        text: ""
    }
}
