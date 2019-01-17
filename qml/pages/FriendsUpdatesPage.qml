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
import harbour.sailreads 1.0

import "../components"
import "../utils/Utils.js" as Utils

Page {
    id: friendsUpdatesPage

    property bool busy: sailreadsManager.busy && friendsUpdatesPage.status === PageStatus.Active

    property string contentFilter: applicationSettings.value("friends/updatesContentFilter", "all")
    property string scopeFilter: applicationSettings.value("friends/updatesScopeFilter","friends")

    function attachPage() {
        if (pageStack._currentContainer.attachedContainer === null
                && sailreadsManager.logged) {
            //pageStack.pushAttached(Qt.resolvedUrl("StatusPage.qml"))
        }
    }

    Component.onDestruction: {
        sailreadsManager.abortRequest()
    }

    UpdatesModel {
        id: updatesModel
        updateItems: contentFilter
        updateScope: scopeFilter
    }

    SilicaListView {
        id: friendsUpdatesView
        anchors.fill: parent
        clip: true

        PullDownMenu {
            busy: friendsUpdatesPage.busy
            MenuItem {
                text: qsTr("Friends")
                onClicked: pageStack.replace(Qt.resolvedUrl("FriendsPage.qml"),
                        { userId: sailreadsManager.authUser ? sailreadsManager.authUser.id : 0 })
            }
            MenuItem {
                text: qsTr("Filter")
                onClicked: {
                    var dialog = pageStack.push("../dialogs/UpdatesFilterDialog.qml",
                           { items: contentFilter, scope: scopeFilter })
                    dialog.accepted.connect (function () {
                        friendsUpdatesPage.contentFilter = dialog.items
                        applicationSettings.setValue("friends/updatesContentFilter",
                                friendsUpdatesPage.contentFilter)
                        friendsUpdatesPage.scopeFilter = dialog.scope
                        applicationSettings.setValue("friends/updatesScopeFilter",
                                friendsUpdatesPage.scopeFilter)
                        updatesModel.refreshUpdates()
                    })
                }
            }
            MenuItem {
                text: qsTr("Refresh")
                onClicked: updatesModel.refreshUpdates()
            }
        }

        header: PageHeader {
            title: qsTr("Friends updates")
            description: qsTr("%1, %2")
                    .arg(Utils.humanReadableContentField(contentFilter))
                    .arg(Utils.humanReadableScopeField(scopeFilter))
        }

        cacheBuffer: friendsUpdatesPage.height
        model: updatesModel
        function fetchMoreIfNeeded() {
            if (!friendsUpdatesPage.busy &&
                    updatesModel.hasMore &&
                    indexAt(contentX, contentY + height) > updatesModel.rowCount() - 2) {
                updatesModel.fetchMoreContent()
            }
        }

        onContentYChanged: fetchMoreIfNeeded()


        delegate: UpdateListItem {
            imageUrl: updateImageUrl
            headerText: updateHeaderText
            date: Qt.formatDateTime(updateUpdatedDate)
            onLinkActivated: {
                console.log(link)
            }
            onClicked: {
            }
        }

        VerticalScrollDecorator{}
    }


    BusyIndicator {
        size: BusyIndicatorSize.Large
        anchors.centerIn: parent
        running: sailreadsManager.busy
        visible: running
    }
}
