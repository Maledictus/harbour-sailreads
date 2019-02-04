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
                text: qsTr("Add comment")
                onClicked: {
                    var dialog = pageStack.push(Qt.resolvedUrl("../dialogs/AddCommentDialog.qml"))
                    dialog.accepted.connect(function () {
                        sailreadsManager.addNewComment("read_status", readStatusId, dialog.comment)
                    })
                }
            }
            MenuItem {
                text: qsTr("Refresh")
                onClicked: {
                    readStatusItem.loadReadStatus()
                }
            }
        }

        PushUpMenu {
            busy: readStatusPage.busy
            MenuItem {
                text: qsTr("Add comment")
                onClicked: {
                    var dialog = pageStack.push(Qt.resolvedUrl("../dialogs/AddCommentDialog.qml"))
                    dialog.accepted.connect(function () {
                        sailreadsManager.addNewComment("read_status", readStatusId, dialog.comment)
                    })
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
            title: qsTr("Status update")
        }

        Column {
            anchors {
                left: parent.left
                leftMargin: Theme.horizontalPageMargin
                right: parent.right
                rightMargin: Theme.horizontalPageMargin
            }

            spacing: Theme.paddingMedium

            UserStatusRow {
                width: parent.width
                userAvatar: readStatus && readStatus.user ?
                        readStatus.user.avatarUrl :
                        "qrc:/images/gra_small.png"
                userHeader: readStatus ? readStatus.header : ""
                postDate: readStatus ? Qt.formatDateTime(readStatus.updateDate) : ""
                onUserClicked: pageStack.push(Qt.resolvedUrl("ProfilePage.qml"),
                        { userId: readStatus && readStatus.user ? readStatus.user.id : "" })
                onLinkActivated: mainWindow.openPageFromUrl(link)
            }

            ShortBookRow {
                width: parent.width
                bookImage: readStatus && readStatus.book ? readStatus.book.imageUrl : ""
                bookTitle: readStatus && readStatus.book ? readStatus.book.titleWithoutSeries : ""
                bookAuthors: readStatus && readStatus.book ?
                        Utils.getAuthorsString(readStatus.book.authors, Theme.primaryColor) : ""
                bookAverageRating: readStatus && readStatus.book ? readStatus.book.averageRating : 0.0
                bookRatingsCount: readStatus && readStatus.book ? readStatus.book.ratingsCount : 0

                onBookClicked: pageStack.push(Qt.resolvedUrl("BookPage.qml"),
                        { bookId: readStatus && readStatus.book ? readStatus.book.id : "",
                            book : readStatus ? readStatus.book : null })
                onAuthorLinkActivated: pageStack.push(Qt.resolvedUrl("AuthorPage.qml"),
                        { authorId : link })
            }

            BaseActionsItem {
                width: parent.width
                likesCount: readStatus ? readStatus.likesCount : 0
                commentsCount: readStatus && readStatus.commentsCount >= commentsView.count ?
                        readStatus.commentsCount : commentsView.count
                editButton.visible: false
                isLiked: readStatus && readStatus.isLiked
                onLike: isLiked ?
                            sailreadsManager.unlikeResource(readStatusId, readStatus.ratingId) :
                            sailreadsManager.likeResource(readStatusId, "ReadStatus")
                onOpenInBrowser: {
                    mainWindow.openInBrowser("https://www.goodreads.com/read_statuses/%1".arg(readStatusId))
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
