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
import harbour.sailreads 1.0
import "../components"

SilicaListView {
    id: friendsRequestsView

    property bool busy: false
    property bool active: true
    clip: true
    ViewPlaceholder {
        enabled: !busy && friendsRequestsView.count === 0
        text: qsTr("There are no friends requests. Pull down to refresh")
    }

    model: FriendsRequestsModel {
        id: friendsRequestsModel
    }

    function fetchMoreIfNeeded() {
        if (active && !busy &&
                friendsRequestsModel.hasMore &&
                indexAt(contentX, contentY + height) > friendsRequestsModel.rowCount() - 2) {
            friendsRequestsModel.fetchMoreContent()
        }
    }
    onActiveChanged: fetchMoreIfNeeded()
    onContentYChanged: fetchMoreIfNeeded()

    delegate: ListItem {
        id: listItem
        contentHeight: mainColumn.height + separator.height + Theme.paddingMedium
        clip: true
        Column {
            id: mainColumn
            width: parent.width
            Item {
                height: Math.max(userAvatar.height, column.height)
                anchors {
                    left: parent.left
                    leftMargin: Theme.horizontalPageMargin
                    right: parent.right
                    rightMargin: Theme.horizontalPageMargin
                }

                BaseImage {
                    id: userAvatar
                    anchors {
                        top: parent.top
                        topMargin: Theme.paddingSmall
                    }
                    width: Theme.iconSizeMedium
                    height: Theme.iconSizeMedium
                    source: friendRequestFromUser ?
                            friendRequestFromUser.avatarUrl :
                            ""
                    defaultImage: "qrc:/images/gra_small.png"
                    indicator.size: BusyIndicatorSize.Small
                    enabled: false
                }

                Column {
                    id: column
                    anchors {
                        left: userAvatar.right
                        leftMargin: Theme.paddingMedium
                        right: messageButton.visible ? messageButton.left : acceptButton.left
                        rightMargin: Theme.paddingMedium
                    }

                    Label {
                        width: parent.width
                        wrapMode: Text.WordWrap
                        linkColor: Theme.highlightColor
                        text: friendRequestFromUser ? friendRequestFromUser.userName : ""
                        color: highlighted ? Theme.highlightColor : Theme.primaryColor
                    }

                    Label {
                        anchors.left: parent.left
                        anchors.rightMargin: Theme.horizontalPageMargin
                        font.pixelSize: Theme.fontSizeTiny
                        text: Qt.formatDateTime(friendRequestCreateDate)
                        color: highlighted ? Theme.secondaryHighlightColor : Theme.secondaryColor
                    }
                }

                IconButton {
                    id: messageButton
                    anchors {
                        right: acceptButton.left
                        rightMargin: Theme.paddingSmall
                    }
                    visible: friendRequestMessage !== ""
                    icon.source: "image://theme/icon-m-file-document"
                    onClicked: messageLabel.visible = !messageLabel.visible
                }

                IconButton {
                    id: acceptButton
                    anchors {
                        right: ignoreButton.left
                        rightMargin: Theme.paddingSmall
                    }
                    icon.source: "image://theme/icon-m-acknowledge"
                    onClicked: sailreadsManager.confirmFriendRequest(friendRequestId, true)
                }

                IconButton {
                    id: ignoreButton
                    anchors.right: parent.right
                    icon.source: "image://theme/icon-m-clear"
                    onClicked: sailreadsManager.confirmFriendRequest(friendRequestId, false)
                }
            }
            Label {
                id: messageLabel
                anchors {
                    left: parent.left
                    leftMargin: Theme.horizontalPageMargin
                    right: parent.right
                    rightMargin: Theme.horizontalPageMargin
                }
                wrapMode: Text.WordWrap
                visible: false
                text: friendRequestMessage
            }
        }

        Separator {
            id: separator
            anchors {
                top: mainColumn.bottom
                topMargin: Theme.paddingMedium
            }
            width: parent.width
            color: Theme.primaryColor
            horizontalAlignment: Qt.AlignHCenter
        }

        onClicked: pageStack.push(Qt.resolvedUrl("../pages/ProfilePage.qml"),
                { userId: friendRequestFromUser ? friendRequestFromUser.id : "" })
    }

    VerticalScrollDecorator{}
}
