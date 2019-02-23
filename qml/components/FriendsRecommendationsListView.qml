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
    id: friendsRecommendationsView

    property bool busy: false
    property bool active: true
    clip: true
    ViewPlaceholder {
        enabled: !busy && friendsRecommendationsView.count === 0
        text: qsTr("There are no friends recommendations")
        hintText: qsTr("Pull down to refresh")
    }

    model: FriendsRecommendationsModel {
        id: friendsRecommendationsModel
    }

    function fetchMoreIfNeeded() {
        if (active && !busy &&
                friendsRecommendationsModel.hasMore &&
                indexAt(contentX, contentY + height) > friendsRecommendationsModel.rowCount() - 2) {
            friendsRecommendationsModel.fetchMoreContent()
        }
    }
    onActiveChanged: fetchMoreIfNeeded()
    onContentYChanged: fetchMoreIfNeeded()
    delegate: ListItem {
        id: listItem
        contentHeight: mainColumn.height + separator.height + Theme.paddingMedium
        clip: true
        Item {
            id: mainColumn
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
                source: friendRecommendationRecommendedUser ?
                        friendRecommendationRecommendedUser.avatarUrl :
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
                    right: acceptButton.left
                    rightMargin: Theme.paddingMedium
                }

                Label {
                    width: parent.width
                    wrapMode: Text.WordWrap
                    linkColor: Theme.highlightColor
                    text: friendRecommendationRecommendedUser ?
                            friendRecommendationRecommendedUser.userName :
                            ""
                    color: highlighted ? Theme.highlightColor : Theme.primaryColor
                }


                Label {
                    id: recLabel
                    text: {
                        if (friendRecommendationRecommenderUser) {
                            var style = "<style>a:link { color:" + Theme.primaryColor + "; }</style>"
                            var urlStr = "%1<a href=\"%2\" style=\"text-decoration:none;\">%3</a>"
                                    .arg(style)
                                    .arg(friendRecommendationRecommenderUser.id)
                                    .arg(friendRecommendationRecommenderUser.userName)
                            return qsTr("Recommended by %1").arg(urlStr)
                        }
                        return ""
                    }
                    textFormat: Label.RichText
                    font.pixelSize: Theme.fontSizeSmall
                    color: Theme.highlightColor
                    visible: text !== ""
                    width: parent.width
                    wrapMode: Text.WordWrap
                    onLinkActivated: pageStack.push(Qt.resolvedUrl("../pages/ProfilePage.qml"),
                            { userId: link })
                }

                Label {
                    anchors.left: parent.left
                    anchors.rightMargin: Theme.horizontalPageMargin
                    font.pixelSize: Theme.fontSizeTiny
                    text: Qt.formatDateTime(friendRecommendationCreateDate)
                    color: highlighted ? Theme.secondaryHighlightColor : Theme.secondaryColor
                }
            }

            IconButton {
                id: acceptButton
                anchors {
                    right: ignoreButton.left
                    rightMargin: Theme.paddingSmall
                }
                icon.source: "image://theme/icon-m-acknowledge"
                onClicked: sailreadsManager.confirmFriendRecommendationRequest(friendRecommendationId, true)
            }

            IconButton {
                id: ignoreButton
                anchors.right: parent.right
                icon.source: "image://theme/icon-m-clear"
                onClicked: sailreadsManager.confirmFriendRecommendationRequest(friendRecommendationId, false)
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

        }
        onClicked: pageStack.push(Qt.resolvedUrl("../pages/ProfilePage.qml"),
                { userId: friendRecommendationRecommendedUser ? friendRecommendationRecommendedUser.id : "" })
    }
    VerticalScrollDecorator{}
}
