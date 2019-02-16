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
    id: reviewPage

    property alias reviewId : reviewItem.reviewId
    property alias review : reviewItem.review
    property bool busy: sailreadsManager.busy && reviewPage.status === PageStatus.Active

    function attachPage() {
        if (pageStack._currentContainer.attachedContainer === null
                && sailreadsManager.logged) {
            //pageStack.pushAttached(Qt.resolvedUrl("StatusPage.qml"))
        }
    }

    Component.onDestruction: {
        reviewItem.cancelRequest()
    }

    ReviewItem {
        id: reviewItem
        onReviewChanged: {
            commentsView.currentIndex = -1
        }
    }

    SilicaListView {
        id: commentsView
        anchors.fill: parent
        cacheBuffer: reviewPage.height

        function fetchMoreIfNeeded() {
            if (!reviewPage.busy &&
                    commentsModel.hasMore &&
                    commentsModel.rowCount() > 0 &&
                    indexAt(contentX, contentY + height) > commentsModel.rowCount() - 2) {
                commentsModel.fetchMoreContent()
            }
        }

        contentY: -headerItem.height

        onContentYChanged: fetchMoreIfNeeded()

        PullDownMenu {
            busy: reviewPage.busy
            MenuItem {
                text: qsTr("Add comment")
                onClicked: {
                    var dialog = pageStack.push(Qt.resolvedUrl("../dialogs/AddCommentDialog.qml"))
                    dialog.accepted.connect(function () {
                        sailreadsManager.addNewComment("review", reviewId, dialog.comment)
                    })
                }
            }
            MenuItem {
                text: qsTr("Refresh")
                onClicked: reviewItem.loadReview()
            }
        }

        PushUpMenu {
            busy: reviewPage.busy
            MenuItem {
                text: qsTr("Add comment")
                onClicked: {
                    var dialog = pageStack.push(Qt.resolvedUrl("../dialogs/AddCommentDialog.qml"))
                    dialog.accepted.connect(function () {
                        sailreadsManager.addNewComment("review", reviewId, dialog.comment)
                    })
                }
            }
        }

        header: Item {
            width: commentsView.width
            height: reviewBox.height
        }

        model: ReviewCommentsModel {
            id: commentsModel
            reviewId: reviewItem.reviewId
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
        id: reviewBox
        parent: commentsView.headerItem ? commentsView.headerItem : reviewPage
        width: parent.width

        spacing: Theme.paddingMedium

        PageHeader {
            title: qsTr("Review")
        }

        Column {
            anchors {
                left: parent.left
                leftMargin: Theme.horizontalPageMargin
                right: parent.right
                rightMargin: Theme.horizontalPageMargin
            }

            UserShortReview {
                width: parent.width
                userId: review && review.user ? review.user.id : ""
                avatarImage.source: review ? review.user.avatarUrl : "qrc:/images/gra_small.png"
                shortDescription: review ? review.shortDescription : ""
                nameLabel.label.text: review ? review.user.userName : ""
                ratingBox.rating: review ? review.rating : 0.0
                dateLabel.text: review ?
                        Utils.generateDateString(review.updatedDate, "dd MMM yyyy hh:mm") :
                        ""
                hasCommentImage.visible: false

                headerFontSize: Theme.fontSizeSmall
                ratingIconSize: Theme.iconSizeSmall
                highlighted: false
                onUserClicked: {
                    pageStack.push(Qt.resolvedUrl("ProfilePage.qml"),
                            { userId: userId })
                }
            }

            ShortBookRow {
                width: parent.width
                bookImage: review && review.book ? review.book.imageUrl : ""
                bookTitle: review && review.book ? review.book.title : ""
                bookAuthors: review && review.book ?
                        Utils.getAuthorsString(review.book.authors, Theme.primaryColor) : ""
                bookAverageRating: review && review.book ? review.book.averageRating : 0.0
                bookRatingsCount: review && review.book ? review.book.ratingsCount : 0

                onBookClicked: pageStack.push(Qt.resolvedUrl("BookPage.qml"),
                        { bookId: review && review.book ? review.book.id : "",
                            book : review ? review.book : null })
                onAuthorLinkActivated: pageStack.push(Qt.resolvedUrl("AuthorPage.qml"),
                        { authorId : link })
            }

            SectionHeader {
                text: qsTr("Review")
                visible: review && review.body !== ""
            }

            Label {
                text: review ? review.body : ""
                visible: review && review.body !== ""
                width: parent.width
                wrapMode: Text.WordWrap
                textFormat: Text.StyledText
                linkColor: Theme.highlightColor
                color: Theme.highlightColor
                onLinkActivated: mainWindow.openPageFromUrl(link)
            }

            BaseActionsItem {
                width: parent.width
                likesCount: review ? review.likesCount : 0
                commentsCount: review && review.commentsCount >= commentsView.count ?
                        review.commentsCount : commentsView.count
                editButton.visible: sailreadsManager.authUser &&
                        review && review.user && sailreadsManager.authUser.id === review.user.id
                isLiked: review && review.isLiked
                onLike: isLiked ?
                        sailreadsManager.unlikeResource(reviewId, review.ratingId) :
                        sailreadsManager.likeResource(reviewId, "Review")
                onEdit: {
                    var editDialog = pageStack.push("../dialogs/AddEditReviewDialog.qml",
                            { mode: "edit", book: review.book, rating: review.rating, reviewText: review.body })
                    editDialog.accepted.connect(function () {
                        sailreadsManager.editReview(review.id, editDialog.rating, editDialog.reviewText)
                    })
                }
                onOpenInBrowser: {
                    if (review) {
                        mainWindow.openInBrowser(review.url)
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
        running: reviewPage.busy
        visible: running
    }
}
