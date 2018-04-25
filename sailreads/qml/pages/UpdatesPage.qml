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
    id: updatesPage

    function loadUpdates() {
        if (sailreadsManager.logged) {
            sailreadsManager.getUpdates()
        }
    }

    Component.onDestruction: {
        sailreadsManager.abortRequest()
    }

    onStatusChanged: {
        if (status == PageStatus.Active && sailreadsManager.logged) {
            loadUpdates()
        }
    }

    SilicaListView {
        id: updatesView
        anchors.fill: parent
        header: PageHeader {
            title: qsTr("Updates")
        }

        ViewPlaceholder {
            enabled: false //TODO
            text: qsTr("There are no updates. Pull down to refresh")
        }

        PullDownMenu {
            MenuItem {
                text: qsTr("View Profile")
                onClicked: {
                    pageStack.push(Qt.resolvedUrl("ProfilePage.qml"))
                }
            }

            MenuItem {
                text: qsTr("My Books")
                onClicked: {
                    pageStack.replace(Qt.resolvedUrl("BookShelvesPage.qml"),
                            {userId: sailreadsManager.userProfile.userId})
                }
            }

            MenuItem {
                text: qsTr("Refresh")
                onClicked: {
                    sailreadsManager.getUpdates()
                }
            }
        }

        VerticalScrollDecorator {}
    }

    BusyIndicator {
        size: BusyIndicatorSize.Large
        anchors.centerIn: parent
        running: sailreadsManager.busy
        visible: running
    }
}
