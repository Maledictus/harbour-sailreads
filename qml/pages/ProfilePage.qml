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
import QtQml.Models 2.2
import Sailfish.Silica 1.0
import harbour.sailreads 1.0

import "../components"
import "../utils/Utils.js" as Utils

Page {
    id: profilePage

    BookShelfProxyModel {
        id: bookShelvesModel

        limit: 3
        filterRole: BookShelvesModel.Exclusive
        filterRegExp: new RegExp("true")
        dynamicSortFilter: true
        sourceModel: BookShelvesModel {
            userId: sailreadsManager.userProfile.userId
        }
    }

    SilicaFlickable {
        id: profileView

        anchors.fill: parent

        contentHeight: content.height + header.height

        PageHeader{
            id: header
            title: sailreadsManager.userProfile.userName
            description: sailreadsManager.userProfile.nickName
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
                        sailreadsManager.getUserInfo(sailreadsManager.userProfile.userId)
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
                    source: sailreadsManager.userProfile.avatarUrl
                }

                Column {
                    width: parent.width - Theme.iconSizeExtraLarge - Theme.paddingMedium
                    Label {
                        font.pixelSize: Theme.fontSizeSmall
                        width: parent.width
                        elide: Text.ElideRight
                        text: qsTr("Details: %1")
                                .arg(Utils.getDetailsInfoString(sailreadsManager.userProfile.age,
                                        sailreadsManager.userProfile.gender,
                                        sailreadsManager.userProfile.location))
                    }
                    Label {
                        font.pixelSize: Theme.fontSizeSmall
                        width: parent.width
                        elide: Text.ElideRight
                        text: qsTr("Joined: %1").arg(sailreadsManager.userProfile.joinedDate)
                    }
                    Label {
                        font.pixelSize: Theme.fontSizeSmall
                        width: parent.width
                        elide: Text.ElideRight
                        text: qsTr("Last active: %1")
                                .arg(sailreadsManager.userProfile.lastUpdateDate)
                    }
                }
            }

            Label {
                text: qsTr("Interests: %1").arg(sailreadsManager.userProfile.interests)
                maximumLineCount: 3
                wrapMode: Text.WordWrap
                font.pixelSize: Theme.fontSizeSmall
                anchors {
                    left: parent.left
                    leftMargin: Theme.horizontalPageMargin
                    right: parent.right
                    rightMargin: Theme.horizontalPageMargin
                }
            }
            Label {
                text: qsTr("Favorite books: %1").arg(sailreadsManager.userProfile.favoriteBooksDesc)
                maximumLineCount: 3
                wrapMode: Text.WordWrap
                font.pixelSize: Theme.fontSizeSmall
                anchors {
                    left: parent.left
                    leftMargin: Theme.horizontalPageMargin
                    right: parent.right
                    rightMargin: Theme.horizontalPageMargin
                }
            }
            Label {
                text: qsTr("About me: %1").arg(sailreadsManager.userProfile.about)
                maximumLineCount: 3
                wrapMode: Text.WordWrap
                font.pixelSize: Theme.fontSizeSmall
                anchors {
                    left: parent.left
                    leftMargin: Theme.horizontalPageMargin
                    right: parent.right
                    rightMargin: Theme.horizontalPageMargin
                }
            }

            MoreButton {
                id: friendsButton
                width: parent.width
                height: Theme.itemSizeMedium
                text: qsTr("Friends")
                counter: sailreadsManager.userProfile.friendsCount
                busy: sailreadsManager.busy
            }

            MoreButton {
                id: groupsButton
                width: parent.width
                height: Theme.itemSizeMedium
                text: qsTr("Groups")
                counter: sailreadsManager.userProfile.groupsCount
                busy: sailreadsManager.busy
            }

            MoreButton {
                id: bookShelvesButton
                width: parent.width
                height: Theme.itemSizeMedium
                text: qsTr("Bookshelves")
                counter: sailreadsManager.userProfile.bookShelvesCount
                busy: sailreadsManager.busy
                onClicked: {
                    pageStack.push(Qt.resolvedUrl("BOokShelvesPage.qml"),
                            { userId: sailreadsManager.userProfile.userId })
                }
            }

            SilicaListView {
                id: bookshelvesView

                width: parent.width
                height: contentHeight
                delegate:  ListItem {
                    width: parent.width
                    Label {
                        anchors {
                            left: parent.left
                            leftMargin: Theme.horizontalPageMargin
                            right: parent.right
                            rightMargin: Theme.horizontalPageMargin
                            verticalCenter: parent.verticalCenter
                        }

                        text: bookShelfName + " (" + bookShelfBooksCount + ")"
                    }
                }

                model: bookShelvesModel

                BusyIndicator {
                    size: BusyIndicatorSize.Large
                    anchors.centerIn: parent
                    running: sailreadsManager.busy
                    visible: running
                }
            }
        }
    }
}
