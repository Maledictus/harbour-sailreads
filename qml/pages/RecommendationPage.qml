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
    id: recommendationPage

    property alias recommendationId : recommendationItem.recommendationId
    property alias recommendation : recommendationItem.recommendation
    property bool busy: sailreadsManager.busy && recommendationPage.status === PageStatus.Active

    function attachPage() {
        if (pageStack._currentContainer.attachedContainer === null
                && sailreadsManager.logged) {
            //pageStack.pushAttached(Qt.resolvedUrl("StatusPage.qml"))
        }
    }

    Component.onDestruction: {
        recommendationItem.cancelRequest()
    }

    RecommendationItem {
        id: recommendationItem
    }

    SilicaListView {
        id: commentsView
        anchors.fill: parent
        cacheBuffer: recommendationPage.height

        ViewPlaceholder {
            y: 0
            enabled: !sailreadsManager.busy && commentsView.count === 0
            text: qsTr("There are no comments")
            hintText: qsTr("Pull down to refresh")
        }

        function fetchMoreIfNeeded() {
            if (!recommendationPage.busy &&
                    commentsModel.hasMore &&
                    commentsModel.rowCount() > 0 &&
                    indexAt(contentX, contentY + height) > commentsModel.rowCount() - 2) {
                commentsModel.fetchMoreContent()
            }
        }
        contentY: -headerItem.height
        onContentYChanged: fetchMoreIfNeeded()

        PullDownMenu {
            busy: recommendationPage.busy
            MenuItem {
                text: qsTr("Add comment")
                onClicked: {
                    var dialog = pageStack.push(Qt.resolvedUrl("../dialogs/AddCommentDialog.qml"))
                    dialog.accepted.connect(function () {
                        sailreadsManager.addNewComment("recommendation", recommendationId, dialog.comment)
                    })
                }
            }
             MenuItem {
                text: qsTr("Refresh")
                onClicked: {
                    recommendationItem.loadRecommendation()
                }
            }
        }

        PushUpMenu {
            busy: recommendationPage.busy
            MenuItem {
                text: qsTr("Add comment")
                onClicked: {
                    var dialog = pageStack.push(Qt.resolvedUrl("../dialogs/AddCommentDialog.qml"))
                    dialog.accepted.connect(function () {
                        sailreadsManager.addNewComment("recommendation", recommendationId, dialog.comment)
                    })
                }
            }
        }

        header: Item {
            width: commentsView.width
            height: recommendationBox.height
        }

        model: RecommendationCommentsModel {
            id: commentsModel
            recommendationId: recommendationPage.recommendationId
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
        id: recommendationBox
        parent: commentsView.headerItem ? commentsView.headerItem : recommendationPage
        width: parent.width

        spacing: Theme.paddingMedium

        PageHeader {
            title: qsTr("Recommendation")
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
                userAvatar: recommendation && recommendation.fromUser ?
                        recommendation.fromUser.avatarUrl :
                        "qrc:/images/gra_small.png"
                userHeader: {
                    var fromUrl = ""
                    var toUrl = ""
                    var bookUrl = ""
                    if (recommendation && recommendation.fromUser) {
                        fromUrl = "<a href=\"" + recommendation.fromUser.webUrl + "\" \">" +
                                recommendation.fromUser.userName + "</a>"
                    }

                    if (recommendation && recommendation.toUser) {
                        toUrl = "<a href=\"" + recommendation.toUser.webUrl + "\" \">" +
                                recommendation.toUser.userName + "</a>"
                    }

                    if (recommendation && recommendation.book) {
                        bookUrl = "<a href=\"" + recommendation.book.link + "\" \">" +
                                recommendation.book.title + "</a>"
                    }
                    return qsTr ("%1 recommended the book %2 to %3")
                        .arg(fromUrl)
                        .arg(bookUrl)
                        .arg(toUrl)
                }
                postDate: recommendation ? Qt.formatDateTime(recommendation.createDate) : ""
                onUserClicked: pageStack.push(Qt.resolvedUrl("ProfilePage.qml"),
                        { userId: recommendation && recommendation.fromUser ? recommendation.fromUser.id : "" })
                onLinkActivated: mainWindow.openPageFromUrl(link)
            }

            ShortBookRow {
                width: parent.width
                bookImage: recommendation && recommendation.book ? recommendation.book.imageUrl : ""
                bookTitle: recommendation && recommendation.book ? recommendation.book.title : ""
                bookAuthors: recommendation && recommendation.book ?
                        Utils.getAuthorsString(recommendation.book.authors, Theme.primaryColor) : ""
                bookAverageRating: recommendation && recommendation.book ? recommendation.book.averageRating : 0.0
                bookRatingsCount: recommendation && recommendation.book ? recommendation.book.ratingsCount : 0

                onBookClicked: pageStack.push(Qt.resolvedUrl("BookPage.qml"),
                        { bookId: recommendation && recommendation.book ? recommendation.book.id : "",
                            book : recommendation ? recommendation.book : null })
                onAuthorLinkActivated: pageStack.push(Qt.resolvedUrl("AuthorPage.qml"),
                        { authorId : link })
            }

            BaseActionsItem {
                width: parent.width
                likesCount: recommendation ? recommendation.likesCount : 0
                commentsCount: recommendation && recommendation.commentsCount >= commentsView.count ?
                        recommendation.commentsCount : commentsView.count
                editButton.visible: false
                isLiked: recommendation && recommendation.isLiked
                busy: recommendationPage.busy
                onLike: isLiked ?
                        sailreadsManager.unlikeResource(recommendationId, recommendation.ratingId) :
                        sailreadsManager.likeResource(recommendationId, "Recommendation")
                onOpenInBrowser: {
                    mainWindow.openInBrowser("https://www.goodreads.com/recommendations/%1".arg(recommendationId))
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
        running: recommendationPage.busy
        visible: running
    }
}
