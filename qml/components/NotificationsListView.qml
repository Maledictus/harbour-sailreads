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
    id: notificationsView

    property bool busy: false
    property bool active: true

    clip: true
    model: NotificationsModel {
        id: notificationsModel
    }

    ViewPlaceholder {
        enabled: !busy && notificationsView.count === 0
        text: qsTr("There are no notifications")
        hintText: qsTr("Pull down to refresh")
    }

    function fetchMoreIfNeeded() {
        if (active && !busy &&
                notificationsModel.hasMore &&
                indexAt(contentX, contentY + height) > notificationsModel.rowCount() - 2) {
            notificationsModel.fetchMoreContent()
        }
    }
    onActiveChanged: fetchMoreIfNeeded()
    onContentYChanged: fetchMoreIfNeeded()

    delegate: ListItem {
        id: listItem
        contentHeight: row.height + separator.height + Theme.paddingMedium
        clip: true
        Row {
            id: row
            height: Math.max(firstActorAvatar.height, column.height)
            spacing: Theme.paddingMedium
            anchors {
                left: parent.left
                leftMargin: Theme.horizontalPageMargin
                right: parent.right
                rightMargin: Theme.horizontalPageMargin
            }

            BaseImage {
                id: firstActorAvatar
                anchors {
                    top: parent.top
                    topMargin: Theme.paddingSmall
                }
                width: Theme.iconSizeMedium
                height: Theme.iconSizeMedium
                source: notificationActors.length > 0 ? notificationActors[0].avatarUrl : ""
                defaultImage: "qrc:/images/gra_small.png"
                indicator.size: BusyIndicatorSize.Small
                enabled: false
            }

            Column {
                id: column
                width: parent.width - firstActorAvatar.width
                Label {
                    width: parent.width
                    wrapMode: Text.WordWrap
                    font.bold: notificationIsNew
                    textFormat: Text.StyledText
                    linkColor: Theme.highlightColor
                    text: notificationPlainText
                    onLinkActivated: mainWindow.openPageFromUrl(link)
                    color: highlighted ? Theme.highlightColor : Theme.primaryColor
                }
                Label {
                    anchors.right: parent.right
                    anchors.rightMargin: Theme.horizontalPageMargin
                    font.pixelSize: Theme.fontSizeTiny
                    text: Qt.formatDateTime(notificationCreateDate)
                    color: highlighted ? Theme.secondaryHighlightColor : Theme.secondaryColor
                }
            }
        }

        Separator {
            id: separator
            anchors {
                top: row.bottom
                topMargin: Theme.paddingMedium
            }
            width: parent.width
            color: Theme.primaryColor
            horizontalAlignment: Qt.AlignHCenter
        }

        onClicked: {
            if (notificationResourceId === 0) {
                mainWindow.openPageFromUrl(notificationUrl)
            }
            else {
                console.log(notificationGroupResourceType)
                console.log(notificationUrl)
                switch (notificationGroupResourceType) {
                case "Topic":
                    pageStack.push(Qt.resolvedUrl("../pages/GroupFolderTopicPage.qml"),
                            { topicId: notificationResourceId })
                    break
                case "Review":
                    pageStack.push(Qt.resolvedUrl("../pages/ReviewPage.qml"),
                            { reviewId: notificationResourceId })
                    break
                case "Friend":
                    if (notificationResourceType === "Friend") {
                        pageStack.push(Qt.resolvedUrl("../pages/ProfilePage.qml"),
                                { userId: notificationResourceId })
                        break
                    }
                    else {
                        mainWindow.openPageFromUrl(notificationUrl)
                    }
                    break
                default:
                    mainWindow.openPageFromUrl(notificationUrl)
                    break
                }
            }
        }
    }
    VerticalScrollDecorator{}
}
