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

Page {
    SilicaFlickable {
        id: settingsContent
        anchors.fill: parent
        contentWidth: width
        contentHeight: column.height

        RemorsePopup { id: remorse }

        Column {
            id: column
            spacing: Theme.paddingSmall
            width: parent.width
            PageHeader {
                title: qsTr("Settings")
            }

            PullDownMenu {
                MenuItem {
                    text: qsTr("About")
                    onClicked: pageStack.push(Qt.resolvedUrl("AboutPage.qml"))
                }
                MenuItem {
                    text: qsTr("Logout")
                    onClicked: remorse.execute(qsTr("Logout"), function() {
                        sailreadsManager.logout()
                    } )
                }
            }

            SectionHeader {
                text: qsTr("User interface")
            }

            TextSwitch {
                text: qsTr("Show friends updates on friends page")
                checked: applicationSettings.showFriendsUpdates
                onCheckedChanged: applicationSettings.showFriendsUpdates = checked
            }

            TextSwitch {
                text: qsTr("Use embeded webview to show web content")
                checked: applicationSettings.useEmbededBrowser
                onCheckedChanged: applicationSettings.useEmbededBrowser = checked
            }

            TextSwitch {
                text: qsTr("Show your recent updates on profile page")
                checked: applicationSettings.showYourRecentUpdates
                onCheckedChanged:applicationSettings.showYourRecentUpdates = checked
            }
        }
    }
}
