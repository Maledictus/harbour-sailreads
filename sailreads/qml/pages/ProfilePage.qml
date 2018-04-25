﻿/*
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
import QtQml.Models 2.2
import Sailfish.Silica 1.0
import harbour.sailreads 1.0

import "../components"
import "../utils/Utils.js" as Utils

Page {
    id: profilePage

    property alias userId: userProfile.userId
    property bool busy: sailreadsManager.busy && profilePage.status == PageStatus.Active

    function attachPage() {
        if (pageStack._currentContainer.attachedContainer === null
                && sailreadsManager.logged) {
            pageStack.pushAttached(Qt.resolvedUrl("StatusPage.qml"))
        }
    }

    Component.onDestruction: {
        sailreadsManager.abortRequest()
    }

    UserProfile {
        id: userProfile
    }

    BookShelfProxyModel {
        id: bookShelvesModel

        limit: 3
        filterRole: BookShelvesModel.Exclusive
        filterRegExp: new RegExp("true")
        dynamicSortFilter: true
        sourceModel: BookShelvesModel {
            userId: userProfile.user.id
        }
    }

    SilicaFlickable {
        id: profileView

        anchors.fill: parent

        contentHeight: content.height + header.height

        PageHeader {
            id: header
            title: userProfile.user.userName
            description: userProfile.user.nickName
        }

        Column {
            id: content

            anchors {
                left: parent.left
                right: parent.right
                top: header.bottom
            }

            PullDownMenu {
                MenuItem {
                    text: qsTr("Refresh")
                    onClicked: {
                        userProfile.updateProfile()
                    }
                }
            }

            Row {
                anchors {
                    left: parent.left
                    leftMargin: Theme.horizontalPageMargin
                    right: parent.right
                    rightMargin: Theme.horizontalPageMargin
                }

                spacing: Theme.paddingMedium

                Image {
                    id: avatarImage
                    width: Theme.iconSizeExtraLarge
                    height: Theme.iconSizeExtraLarge
                    source: userProfile.user.avatarUrl
                    fillMode: Image.PreserveAspectFit
                    BusyIndicator {
                        anchors.centerIn: parent
                        size: BusyIndicatorSize.Medium
                        running: profilePage.busy
                        visible: running
                    }
                }

                Column {
                    width: parent.width - Theme.iconSizeExtraLarge - Theme.paddingMedium
                    KeyValueLabel {
                        font.pixelSize: Theme.fontSizeSmall
                        width: parent.width
                        visible: value !== ""
                        key: qsTr("Details")
                        value: Utils.getDetailsInfoString(
                                   userProfile.user.age,
                                   userProfile.user.gender,
                                   userProfile.user.location)
                    }
                    KeyValueLabel {
                        font.pixelSize: Theme.fontSizeSmall
                        width: parent.width
                        visible: value !== ""
                        key: qsTr("Joined")
                        value: userProfile.user.joinedDate
                    }
                    KeyValueLabel {
                        font.pixelSize: Theme.fontSizeSmall
                        width: parent.width
                        visible: value !== ""
                        key: qsTr("Last active")
                        value: userProfile.user.lastUpdateDate
                    }
                }
            }

            Column {
                anchors {
                    left: parent.left
                    leftMargin: Theme.horizontalPageMargin
                    right: parent.right
                    rightMargin: Theme.horizontalPageMargin
                }
                KeyValueLabel {
                    maximumLineCount: 3
                    wrapMode: Text.WordWrap
                    font.pixelSize: Theme.fontSizeSmall
                    width: parent.width
                    visible: value !== ""
                    key: qsTr("Interests")
                    value: userProfile.user.interests
                }
                KeyValueLabel {
                    maximumLineCount: 3
                    wrapMode: Text.WordWrap
                    font.pixelSize: Theme.fontSizeSmall
                    width: parent.width
                    visible: value !== ""
                    key: qsTr("Favorite books")
                    value: userProfile.user.favoriteBooksDesc
                }
                KeyValueLabel {
                    maximumLineCount: 3
                    wrapMode: Text.WordWrap
                    font.pixelSize: Theme.fontSizeSmall
                    width: parent.width
                    visible: value !== ""
                    key: qsTr("About me")
                    value: userProfile.user.about
                }
            }

            MoreButton {
                id: friendsButton
                width: parent.width
                height: Theme.itemSizeMedium
                text: qsTr("Friends")
                counter: userProfile.user.friendsCount
                busy: profilePage.busy
                enabled: !busy
                visible: userProfile.user.friendsCount > 0
                onClicked: {
                    pageStack.push(Qt.resolvedUrl("FriendsPage.qml"), {
                        userId: userProfile.user.id
                   })
                }
            }

            MoreButton {
                id: groupsButton
                width: parent.width
                height: Theme.itemSizeMedium
                text: qsTr("Groups")
                counter: userProfile.user.groupsCount
                busy: profilePage.busy
                enabled: !busy
                visible: userProfile.user.groupsCount > 0
                onClicked: {
                    pageStack.push(Qt.resolvedUrl("GroupsPage.qml"), {
                        userId: userProfile.user.id
                    })
                }
            }

            MoreButton {
                id: bookShelvesButton
                width: parent.width
                height: Theme.itemSizeMedium
                text: qsTr("Bookshelves")
                counter: userProfile.user.bookShelvesCount
                busy: profilePage.busy
                enabled: !busy
                onClicked: {
                    pageStack.push(Qt.resolvedUrl("BookShelvesPage.qml"), {
                        userId: userProfile.user.id
                    })
                }
            }

            SilicaListView {
                id: bookshelvesView

                width: parent.width
                height: contentHeight
                delegate: ListItem {
                    Label {
                        id: shelfName
                        anchors {
                            left: parent.left
                            leftMargin: Theme.horizontalPageMargin
                            right: shelfCount.left
                            rightMargin: Theme.paddingMedium
                            verticalCenter: parent.verticalCenter
                        }

                        truncationMode: TruncationMode.Fade
                        text: bookShelfName
                    }

                    Label {
                        id: shelfCount
                        anchors {
                            right: parent.right
                            rightMargin: Theme.horizontalPageMargin
                            verticalCenter: parent.verticalCenter
                        }
                        color: Theme.secondaryColor
                        text: bookShelfBooksCount
                    }

                    onClicked: {
                        pageStack.push(Qt.resolvedUrl("ReviewsPage.qml"),
                                { userId: userProfile.user.id, bookShelfId: bookShelfId,
                                    bookShelf: bookShelfName })
                    }
                }

                model: bookShelvesModel

                BusyIndicator {
                    size: BusyIndicatorSize.Large
                    anchors.centerIn: parent
                    running: profilePage.busy
                    visible: running
                }
            }
        }
    }
}
