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
import QtQml.Models 2.3
import Sailfish.Silica 1.0
import harbour.sailreads 1.0

import "../components"

Page {
    id: friendsPage

    property int userId: 0
    property bool busy: sailreadsManager.busy && friendsPage.status === PageStatus.Active

    function attachPage() {
        if (pageStack._currentContainer.attachedContainer === null
                && sailreadsManager.logged) {
            //pageStack.pushAttached(Qt.resolvedUrl("StatusPage.qml"))
        }
    }

    Component.onDestruction: {
        sailreadsManager.abortRequest()
    }

    SilicaFlickable {
        id: flickable

        anchors.fill: parent

        PullDownMenu {
            MenuItem {
                text: qsTr("Refresh")
                onClicked: {
                    switch (slideshowView.currentIndex) {
                    case 0:
                        sailreadsManager.loadFriends(userId)
                        return
                    case 1:
                        sailreadsManager.loadUserFollowings(userId)
                        return
                    case 2:
                        sailreadsManager.loadUserFollowers(userId)
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

                SilicaListView {
                    id: friendsView
                    width: slideshowView.width
                    height: slideshowView.height
                    header: PageHeader { title: qsTr("Friends") }

                    ViewPlaceholder {
                        enabled: !sailreadsManager.busy && friendsView.count === 0
                        text: qsTr("There are no friends. Pull down to refresh")
                    }

                    cacheBuffer: friendsPage.height
                    model: FriendsModel {
                        id: friendsModel
                        userId: friendsPage.userId
                    }

                    function fetchMoreIfNeeded() {
                        if (!friendsPage.busy &&
                                friendsModel.hasMore &&
                                indexAt(contentX, contentY + height) > friendsModel.rowCount() - 2) {
                            friendsModel.fetchMoreContent()
                        }
                    }

                    onContentYChanged: fetchMoreIfNeeded()

                    delegate: FriendListItem {
                        id: rootDelegateItem
                        width: friendsView.width

//                        RemorseItem { id: remorse }

//                        menu: ContextMenu {
//                            hasContent: userId === sailreadsManager.authUser.id
//                            MenuItem {
//                                text: qsTr("Remove")
//                                onClicked: {
//                                    remorse.execute(rootDelegateItem, qsTr("Removing"), function() {
//                                        sailreadsManager.removeFriend(friendId)
//                                    })
//                                }
//                            }
//                        }

                        onClicked: {
                            pageStack.push(Qt.resolvedUrl("ProfilePage.qml"),
                                    { userId: friendId })
                        }
                    }
                }

                SilicaListView {
                    id: followingsView
                    width: slideshowView.width
                    height: slideshowView.height
                    header: PageHeader { title: qsTr("Followings") }

                    ViewPlaceholder {
                        enabled: !sailreadsManager.busy && followingsView.count === 0
                        text: qsTr("There are no followings. Pull down to refresh")
                    }

                    cacheBuffer: friendsPage.height
                    model: FollowingsModel {
                        id: followingsModel
                        userId: friendsPage.userId
                    }

                    function fetchMoreIfNeeded() {
                        if (!friendsPage.busy &&
                                followingsModel.hasMore &&
                                indexAt(contentX, contentY + height) > followingsModel.rowCount() - 2) {
                            followingsModel.fetchMoreContent()
                        }
                    }

                    onContentYChanged: fetchMoreIfNeeded()

                    delegate: FriendListItem {
                        id: rootDelegateItem2
                        width: followingsView.width

                        RemorseItem { id: remorse2 }

                        menu: ContextMenu {
                            hasContent: userId === sailreadsManager.authUser.id
                            MenuItem {
                                text: qsTr("Unfollow")
                                onClicked: {
                                    remorse2.execute(rootDelegateItem, qsTr("Unfollowing"), function() {
                                        sailreadsManager.unfollowUser(friendId)
                                    })
                                }
                            }
                        }

                        onClicked: {
                            pageStack.push(Qt.resolvedUrl("ProfilePage.qml"),
                                    { userId: friendId })
                        }
                    }
                }

                SilicaListView {
                    id: followersView
                    width: slideshowView.width
                    height: slideshowView.height
                    header: PageHeader { title: qsTr("Followers") }

                    ViewPlaceholder {
                        enabled: !sailreadsManager.busy && followersView.count === 0
                        text: qsTr("There are no followers. Pull down to refresh")
                    }

                    cacheBuffer: friendsPage.height
                    model: FollowersModel {
                        id: followersModel
                        userId: friendsPage.userId
                    }

                    function fetchMoreIfNeeded() {
                        if (!friendsPage.busy &&
                                followersModel.hasMore &&
                                indexAt(contentX, contentY + height) > followersModel.rowCount() - 2) {
                            followersModel.fetchMoreContent()
                        }
                    }

                    onContentYChanged: fetchMoreIfNeeded()

                    delegate: FriendListItem {
                        width: followersView.width

                        onClicked: {
                            pageStack.push(Qt.resolvedUrl("ProfilePage.qml"),
                                    { userId: friendId })
                        }
                    }
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
                width: parent.width / 3
                icon.source: "image://Theme/icon-m-data-traffic?"+ (pressed || selected ?
                        Theme.highlightColor : Theme.primaryColor)
                text: qsTr("Friends")
                selected: slideshowView.currentIndex == 0
                onClicked: {
                    slideshowView.currentIndex = 0
                }
            }

            NavigationButton {
                width: parent.width / 3
                icon.source: "image://Theme/icon-m-data-upload?"+ (pressed || selected ?
                        Theme.highlightColor : Theme.primaryColor)
                text: qsTr("Followings")
                selected: slideshowView.currentIndex == 1
                onClicked: {
                    slideshowView.currentIndex = 1
                }
            }

            NavigationButton {
                width: parent.width / 3
                icon.source: "image://Theme/icon-m-data-download?"+ (pressed || selected ?
                        Theme.highlightColor : Theme.primaryColor)
                text: qsTr("Followers")
                selected: slideshowView.currentIndex == 2
                onClicked: {
                    slideshowView.currentIndex = 2
                }
            }
        }
    }

    BusyIndicator {
        size: BusyIndicatorSize.Large
        anchors.centerIn: parent
        running: friendsPage.busy
        visible: running
    }
}
