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
import harbour.sailreads 1.0

Page {
    id: friendsPage

    property int userId: 0
    property bool busy: sailreadsManager.busy && friendsPage.status == PageStatus.Active

    function attachPage() {
        if (pageStack._currentContainer.attachedContainer === null
                && sailreadsManager.logged) {
            pageStack.pushAttached(Qt.resolvedUrl("StatusPage.qml"))
        }
    }

    function load() {
        sailreadsManager.loadFriends(userId)
    }

    FriendsModel {
        id: friendsModel
        userId: friendsPage.userId
    }

    SilicaListView {
        id: friendsView
        anchors.fill: parent
        header: PageHeader {
            title: qsTr("Friends")
        }

        PullDownMenu {
            MenuItem {
                text: qsTr("Refresh")
                onClicked: {
                    sailreadsManager.loadFriends(userId)
                }
            }
        }

        ViewPlaceholder {
            enabled: !sailreadsManager.busy && friendsView.count === 0
            text: qsTr("There are no friends. Pull down to refresh")
        }

        model: friendsModel

        delegate: ListItem {
            width: friendsView.width
            contentHeight: Theme.itemSizeLarge
            clip: true

            Image {
                id: friendIconImage
                anchors {
                    left: parent.left
                    leftMargin: Theme.horizontalPageMargin
                    verticalCenter: parent.verticalCenter
                }
                source: friendAvatarUrl
                height: Theme.iconSizeLarge
                width: Theme.iconSizeLarge
                fillMode: Image.PreserveAspectFit
            }

            Column {
                id: column

                anchors {
                    right: parent.right
                    rightMargin: Theme.horizontalPageMargin
                    left: friendIconImage.right
                    leftMargin: Theme.paddingMedium
                }

                Label {
                    id: friendNameLabel
                    font.family: Theme.fontFamilyHeading
                    truncationMode: TruncationMode.Fade
                    text: friendName
                }
                Label {
                    id: friendBooksCountLabel
                    truncationMode: TruncationMode.Fade
                    font.pixelSize: Theme.fontSizeExtraSmall
                    text: qsTr("%1 books").arg(friendBooksCount)
                }
                Label {
                    id: friendFriendsCountLabel
                    truncationMode: TruncationMode.Fade
                    font.pixelSize: Theme.fontSizeExtraSmall
                    text: qsTr("%1 friends").arg(friendFriendsCount)
                }
            }

            onClicked: {
                //TODO go to profile page
            }
        }

        VerticalScrollDecorator {}
    }

    BusyIndicator {
        size: BusyIndicatorSize.Large
        anchors.centerIn: parent
        running: friendsPage.busy
        visible: running
    }
}
