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
            MenuItem {
                text: qsTr("Refresh")
                onClicked: {
                    reviewItem.updateReview()
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

                userId: review && review.user ? review.user.id : 0
                avatarImage.source: review ? review.user.avatarUrl : "qrc:/images/gra_small.png"
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

            Row {
                width: parent.width
                spacing: Theme.paddingMedium
                BaseImage {
                    source: review && review.book ? review.book.imageUrl : ""
                    height: Theme.coverSizeSmall.height
                    width: Theme.coverSizeSmall.width
                    indicator.size: BusyIndicatorSize.Medium
                    onClicked: {

                    }
                }

                Column {
                    width: parent.width
                    Label {
                        font.family: Theme.fontFamilyHeading
                        font.pixelSize: Theme.fontSizeMedium
                        color: Theme.highlightColor
                        text: review && review.book ? review.book.titleWithoutSeries : ""
                    }

                    Label {
                        textFormat: Text.RichText
                        font.pixelSize: Theme.fontSizeSmall
                        color: Theme.highlightColor
                        text: review && review.book ?
                                Utils.getAuthorsString(review.book.authors, Theme.primaryColor) : ""
                    }
                }
            }

            Label {
                text: review ? review.body : ""
                width: parent.width
                wrapMode: Text.WordWrap
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
                    label.text: review ? review.votes : 0
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

                    label.text: review ? review.commentsCount : 0
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
        running: reviewPage.busy
        visible: running
    }
}
