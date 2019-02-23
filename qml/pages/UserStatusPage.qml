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
import "../utils/Utils.js" as Utils

Page {
    id: userStatusPage

    property alias userStatusId : userStatusItem.userStatusId
    property alias userStatus : userStatusItem.userStatus
    property bool busy: sailreadsManager.busy && userStatusPage.status === PageStatus.Active

    function attachPage() {
        if (pageStack._currentContainer.attachedContainer === null
                && sailreadsManager.logged) {
            //pageStack.pushAttached(Qt.resolvedUrl("StatusPage.qml"))
        }
    }

    Component.onDestruction: {
        userStatusItem.cancelRequest()
    }

    UserStatusItem {
        id: userStatusItem
    }

    SilicaListView {
        id: commentsView
        anchors.fill: parent
        cacheBuffer: userStatusPage.height

        function fetchMoreIfNeeded() {
            if (!userStatusPage.busy &&
                    commentsModel.hasMore &&
                    commentsModel.rowCount() > 0 &&
                    indexAt(contentX, contentY + height) > commentsModel.rowCount() - 2) {
                commentsModel.fetchMoreContent()
            }
        }

        contentY: -headerItem.height

        onContentYChanged: fetchMoreIfNeeded()

        PullDownMenu {
            busy: userStatusPage.busy
            MenuItem {
                text: qsTr("Add comment")
                onClicked: {
                    var dialog = pageStack.push(Qt.resolvedUrl("../dialogs/AddCommentDialog.qml"))
                    dialog.accepted.connect(function () {
                        sailreadsManager.addNewComment("user_status", userStatusId, dialog.comment)
                    })
                }
            }
             MenuItem {
                text: qsTr("Refresh")
                onClicked: userStatusItem.loadUserStatus()
            }
        }

        PushUpMenu {
            busy: userStatusPage.busy
            MenuItem {
                text: qsTr("Add comment")
                onClicked: {
                    var dialog = pageStack.push(Qt.resolvedUrl("../dialogs/AddCommentDialog.qml"))
                    dialog.accepted.connect(function () {
                        sailreadsManager.addNewComment("user_status", userStatusId, dialog.comment)
                    })
                }
            }
        }

        header: Item {
            width: commentsView.width
            height: userStatusBox.height
        }

        model: UserStatusCommentsModel {
            id: commentsModel
            userStatusId: userStatusItem.userStatusId
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
        id: userStatusBox
        parent: commentsView.headerItem ? commentsView.headerItem : userStatusPage
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
                userAvatar: userStatus && userStatus.user ?
                        userStatus.user.avatarUrl :
                        "qrc:/images/gra_small.png"
                userHeader: userStatus ? userStatus.header : ""
                postBody: userStatus ? userStatus.body : ""
                postDate: userStatus ? Qt.formatDateTime(userStatus.updateDate) : ""
                onUserClicked: pageStack.push(Qt.resolvedUrl("ProfilePage.qml"),
                        { userId: userStatus && userStatus.user ? userStatus.user.id : "" })
                onLinkActivated: mainWindow.openPageFromUrl(link)
            }

            ShortBookRow {
                width: parent.width
                bookImage: userStatus && userStatus.book ? userStatus.book.imageUrl : ""
                bookTitle: userStatus && userStatus.book ? userStatus.book.title : ""
                bookAuthors: userStatus && userStatus.book ?
                                 Utils.getAuthorsString(userStatus.book.authors, Theme.primaryColor) : ""
                bookAverageRating: userStatus && userStatus.book ? userStatus.book.averageRating : 0.0
                bookRatingsCount: userStatus && userStatus.book ? userStatus.book.ratingsCount : 0

                onBookClicked: pageStack.push(Qt.resolvedUrl("BookPage.qml"),
                        { bookId: userStatus && userStatus.book ? userStatus.book.id : "",
                            book : userStatus ? userStatus.book : null })
                onAuthorLinkActivated: pageStack.push(Qt.resolvedUrl("AuthorPage.qml"),
                        { authorId : link })
            }

            SectionHeader {
                text: qsTr("Status update")
                visible: userStatus && userStatus.body !== ""
            }

            Label {
                text: userStatus ? userStatus.body : ""
                visible: userStatus && userStatus.body !== ""
                width: parent.width
                wrapMode: Text.WordWrap
                textFormat: Text.StyledText
                linkColor: Theme.primaryColor
                color: Theme.highlightColor
                onLinkActivated: mainWindow.openPageFromUrl(link)
            }

            BaseActionsItem {
                width: parent.width
                likesCount: userStatus ? userStatus.likesCount : 0
                commentsCount: userStatus && userStatus.commentsCount >= commentsView.count ?
                        userStatus.commentsCount : commentsView.count
                editButton.visible: false
                isLiked: userStatus && userStatus.isLiked
                onLike: isLiked ?
                        sailreadsManager.unlikeResource(userStatusId, userStatus.ratingId) :
                        sailreadsManager.likeResource(userStatusId, "UserStatus")
                onOpenInBrowser: {
                    mainWindow.openInBrowser("https://www.goodreads.com/user_status/show/%1".arg(userStatusId))
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
        running: userStatusPage.busy
        visible: running
    }
}
