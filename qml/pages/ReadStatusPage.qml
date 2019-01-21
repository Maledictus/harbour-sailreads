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
    id: readStatusPage

    property alias readStatusId : readStatusItem.readStatusId
    property alias readStatus : readStatusItem.readStatus
    property bool busy: sailreadsManager.busy && readStatusPage.status === PageStatus.Active

    function attachPage() {
        if (pageStack._currentContainer.attachedContainer === null
                && sailreadsManager.logged) {
            //pageStack.pushAttached(Qt.resolvedUrl("StatusPage.qml"))
        }
    }

    Component.onDestruction: {
        readStatusItem.cancelRequest()
    }

    ReadStatusItem {
        id: readStatusItem
        onReadStatusChanged: {
            commentsModel.resourceId = readStatusId
        }
    }

    SilicaListView {
        id: commentsView
        anchors.fill: parent
        cacheBuffer: readStatusPage.height

        function fetchMoreIfNeeded() {
            if (!readStatusPage.busy &&
                    commentsModel.hasMore &&
                    commentsModel.rowCount() > 0 &&
                    indexAt(contentX, contentY + height) > commentsModel.rowCount() - 2) {
                commentsModel.fetchMoreContent()
            }
        }


        contentY: -headerItem.height

        onContentYChanged: fetchMoreIfNeeded()

        PullDownMenu {
            busy: readStatusPage.busy
             MenuItem {
                text: qsTr("Refresh")
                onClicked: {
                    readStatusItem.loadReadStatus()
                }
            }
        }

        header: Item {
            width: commentsView.width
            height: readStatusBox.height
        }

        model: ReadStatusCommentsModel {
            id: commentsModel
            readStatusId: readStatusItem.readStatusId
        }

        delegate: CommentListItem {
            width: parent.width
            author: commentAuthor
            body: commentBody
            updateDate: commentUpdateDate
            onUserClicked: pageStack.push(Qt.resolvedUrl("ProfilePage.qml"), { userId: userId })
            onLinkActivated: mainWindow.openPageFromUrl(link)
        }
        VerticalScrollDecorator{}
    }

    Column {
        id: readStatusBox
        parent: commentsView.headerItem ? commentsView.headerItem : readStatusPage
        width: parent.width

        spacing: Theme.paddingMedium

        PageHeader {
            title: qsTr("Read Status")
        }

        Column {
            anchors {
                left: parent.left
                leftMargin: Theme.horizontalPageMargin
                right: parent.right
                rightMargin: Theme.horizontalPageMargin
            }

            spacing: Theme.paddingMedium

            Row {
                width: parent.width
                spacing: Theme.paddingMedium
                BaseImage {
                    id: avatarImageItem
                    anchors.top: column.top
                    indicator.size: BusyIndicatorSize.Medium
                    source: readStatus && readStatus.user ?
                            readStatus.user.avatarUrl :
                            "qrc:/images/gra_small.png"
                    onClicked: pageStack.push(Qt.resolvedUrl("ProfilePage.qml"), { userId: userId })
                }

                Column {
                    id: column
                    width: parent.width - avatarImageItem.width - Theme.paddingMedium
                    Label {
                        width: parent.width
                        wrapMode: Text.WordWrap
                        color: Theme.highlightColor
                        text: readStatus ? readStatus.header : ""
                        font.family: Theme.fontFamilyHeading
                        linkColor: Theme.primaryColor
                        onLinkActivated: mainWindow.openPageFromUrl(link)
                    }

                    Label {
                        id: dateLabelItem
                        visible: text !== ""
                        color: Theme.secondaryColor
                        text: readStatus ? Qt.formatDateTime(readStatus.updateDate) : ""
                        anchors.right: parent.right
                        font.pixelSize: Theme.fontSizeExtraSmall
                    }
                }
            }

            Row {
                width: parent.width
                spacing: Theme.paddingMedium
                BaseImage {
                    id: bookImage
                    source: readStatus && readStatus.book ? readStatus.book.imageUrl : ""
                    height: Theme.coverSizeSmall.height
                    width: Theme.coverSizeSmall.width
                    indicator.size: BusyIndicatorSize.Medium
                    onClicked: {

                    }
                }

                Column {
                    width: parent.width - bookImage.width - Theme.paddingMedium
                    Label {
                        font.family: Theme.fontFamilyHeading
                        font.pixelSize: Theme.fontSizeMedium
                        color: Theme.highlightColor
                        text: readStatus && readStatus.book ? readStatus.book.titleWithoutSeries : ""
                        wrapMode: Text.WordWrap
                        width: parent.width
                    }

                    Label {
                        textFormat: Text.RichText
                        font.pixelSize: Theme.fontSizeSmall
                        color: Theme.highlightColor
                        text: readStatus && readStatus.book ?
                                Utils.getAuthorsString(readStatus.book.authors, Theme.primaryColor) : ""
                    }
                }
            }

            Item {
                width: parent.width
                height: likeButton.height
                IconText {
                    id: voteIcon

                    anchors {
                        left: parent.left
                        baseline: parent.verticalCenter
                    }
                    label.text: readStatus ? readStatus.likesCount : 0
                    label.color: Theme.highlightColor
                    label.font.pixelSize: Theme.fontSizeExtraSmall
                    icon.source: "image://theme/icon-s-like?" + Theme.highlightColor
                }

                IconText {
                    id: commentsIcon

                    anchors {
                        left: voteIcon.right
                        leftMargin: Theme.paddingLarge
                        baseline: parent.verticalCenter
                    }

                    label.text: readStatus ? readStatus.commentsCount : 0
                    label.color: Theme.highlightColor
                    label.font.pixelSize: Theme.fontSizeExtraSmall
                    icon.source: "image://theme/icon-s-chat?" + Theme.highlightColor
                }

                IconButton {
                    id: likeButton
                    anchors {
                        right: parent.right
                        bottom: parent.bottom
                    }

                    icon.source: "image://theme/icon-m-like?" + (pressed ?
                            Theme.highlightColor :
                            Theme.primaryColor)
                    onClicked: {
                        //TODO like review
                    }
                }
            }

            SectionHeader {
                text: qsTr("Comments")
            }
        }
    }

    BusyIndicator {
        size: BusyIndicatorSize.Large
        anchors.centerIn: parent
        running: readStatusPage.busy
        visible: running
    }
}
