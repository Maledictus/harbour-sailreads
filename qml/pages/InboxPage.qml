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
import QtQml.Models 2.3
import Sailfish.Silica 1.0
import harbour.sailreads 1.0

import "../components"

Page {
    id: inboxPage

    property string userId: ""
    property bool busy: sailreadsManager.busy && inboxPage.status === PageStatus.Active
    property MessagesFolder messagesFolder: inboxMessagesFolder

    property MessagesFolder inboxMessagesFolder: MessagesFolder {
        name: qsTr("Inbox")
        key: "inbox"
    }
    property MessagesFolder savedBookmarksFilter: MessagesFolder {
        name: qsTr("Saved")
        key: "saved"
    }
    property MessagesFolder sentBookmarksFilter: MessagesFolder {
        name: qsTr("Sent")
        key: "sent"
    }
    property MessagesFolder trashBookmarksFilter: MessagesFolder {
        name: qsTr("Trash")
        key: "trash"
    }
    property variant messagesFolders: [
        inboxMessagesFolder,
        savedBookmarksFilter,
        sentBookmarksFilter,
        trashBookmarksFilter
    ]

    Component.onDestruction: {
        switch (slideshowView.currentIndex) {
        case 0:
            notificationsView.model.cancelRequest()
            return
        case 1:
            messagesView.model.cancelRequest()
            return
        case 2:
            friendsRequestsView.model.cancelRequest()
            return
        case 3:
            friendsRecommendationsView.model.cancelRequest()
            return
        default:
            return
        }
    }

    SilicaFlickable {
        id: flickable
        anchors.fill: parent

        PullDownMenu {
            busy: inboxPage.busy
            MenuItem {
                visible: slideshowView.currentIndex === 1
                text: qsTr("Select folder")
                onClicked: {
                    pageStack.push(Qt.resolvedUrl("MessagesFolderSelectorPage.qml"),
                            { parentPage: inboxPage, model: messagesFolders })
                }
            }

            MenuItem {
                text: qsTr("Refresh")
                onClicked: {
                    switch (slideshowView.currentIndex) {
                    case 0:
                        notificationsView.model.loadNotifications()
                        return
                    case 1:
                        messagesView.model.loadMessages()
                        return
                    case 2:
                        friendsRequestsView.model.loadFriendsRequests()
                        return
                    case 3:
                        friendsRecommendationsView.model.loadFriendsRecommendations()
                        return
                    default:
                        return
                    }
                }
            }
        }

        SilicaListView {
            id: slideshowView

            orientation: Qt.Horizontal
            snapMode: ListView.SnapOneItem
            clip: true

            anchors {
                left: parent.left
                right: parent.right
                top: parent.top
                bottom: navigationBar.top
            }

            highlightRangeMode: ListView.StrictlyEnforceRange

            model: ObjectModel {
                id: objectModel
                NotificationsListView {
                    id: notificationsView
                    busy: sailreadsManager.busy
                    width: slideshowView.width
                    height: slideshowView.height
                    header: PageHeader { title: qsTr("Notifications") }
                    cacheBuffer: inboxPage.height
                    active: slideshowView.currentIndex === 0
                }

                MessagesListView {
                    id: messagesView
                    busy: sailreadsManager.busy
                    width: slideshowView.width
                    height: slideshowView.height
                    header: PageHeader { title: qsTr("Messages: %1".arg(messagesFolder.name)) }
                    cacheBuffer: inboxPage.height
                    property string prevKey: ""
                    model: MessagesModel {
                        id: messagesModel
                        folder: {
                            if (messagesView.active) {
                                messagesView.prevKey = messagesFolder.key
                                return messagesFolder.key
                            }
                            else {
                                return messagesView.prevKey
                            }
                        }
                    }
                    active: slideshowView.currentIndex === 1
                }

                FriendsRequestsListView {
                    id: friendsRequestsView
                    busy: sailreadsManager.busy
                    width: slideshowView.width
                    height: slideshowView.height
                    header: PageHeader { title: qsTr("Friends Requests") }
                    cacheBuffer: inboxPage.height
                    active: slideshowView.currentIndex === 2
                }

                FriendsRecommendationsListView {
                    id: friendsRecommendationsView
                    busy: sailreadsManager.busy
                    width: slideshowView.width
                    height: slideshowView.height
                    header: PageHeader { title: qsTr("Friends Recommendations") }
                    cacheBuffer: inboxPage.height
                    active: slideshowView.currentIndex === 3
                }
            }
        }

        NavigationBar {
            id: navigationBar
            anchors {
                left: parent.left
                right: parent.right
                bottom: parent.bottom
                bottomMargin: Theme.paddingSmall
            }

            NavigationButton {
                width: parent.width / 4
                icon.source: "image://Theme/icon-m-region"
                text: qsTr("Notifications")
                fontSize: Theme.fontSizeExtraSmall
                selected: slideshowView.currentIndex == 0
                onClicked: slideshowView.currentIndex = 0
            }

            NavigationButton {
                width: parent.width / 4
                icon.source: "image://Theme/icon-m-mail"
                text: qsTr("Messages")
                fontSize: Theme.fontSizeExtraSmall
                selected: slideshowView.currentIndex == 1
                onClicked: slideshowView.currentIndex = 1
            }

            NavigationButton {
                width: parent.width / 4
                icon.source: "image://Theme/icon-m-add"
                text: qsTr("Requests")
                fontSize: Theme.fontSizeExtraSmall
                selected: slideshowView.currentIndex == 2
                onClicked: slideshowView.currentIndex = 2
            }

            NavigationButton {
                width: parent.width / 4
                icon.source: "image://Theme/icon-m-like"
                text: qsTr("Recommendations")
                fontSize: Theme.fontSizeExtraSmall
                selected: slideshowView.currentIndex == 3
                onClicked: slideshowView.currentIndex = 3
            }
        }
    }

    BusyIndicator {
        size: BusyIndicatorSize.Large
        anchors.centerIn: parent
        running: inboxPage.busy
        visible: running
    }
}
