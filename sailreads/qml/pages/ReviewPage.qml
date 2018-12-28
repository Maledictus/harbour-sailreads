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
import QtQuick.Layouts 1.1
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
            pageStack.pushAttached(Qt.resolvedUrl("StatusPage.qml"))
        }
    }

    ReviewItem {
        id: reviewItem
        onReviewIdChanged: {
            console.log(reviewId)
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

        delegate: ListItem {
            width: parent.width
            contentHeight: column.height + separator.height + Theme.paddingMedium
            clip: true
            Column {
                id: column
                anchors {
                    left: parent.left
                    leftMargin: Theme.horizontalPageMargin
                    right: parent.right
                    rightMargin: Theme.horizontalPageMargin
                }

                PosterHeaderItem {
                    width: parent.width
                    posterName: commentAuthor.userName.toUpperCase()
                    postDate: Utils.generateDateString(commentUpdateDate, "dd MMM yyyy hh:mm")
                    posterAvatar: commentAuthor.avatarUrl

                    onClicked: {
                        pageStack.push(Qt.resolvedUrl("ProfilePage.qml"),
                                { userId: commentAuthor.id })
                    }
                }

                property string _style: "<style>" +
                        "a:link { color:" + Theme.highlightColor + "; }" +
                        "p { color:" + Theme.primaryColor + "; }" +
                        "</style>"

                Label {
                    id: label
                    width: parent.width
                    wrapMode: Text.WordWrap
                    textFormat: Text.RichText
                    font.pixelSize: Theme.fontSizeSmall
                    linkColor: Theme.highlightColor
                    text: column._style + commentBody
                    onLinkActivated: {
                        Qt.openUrlExternally(link)
                    }
                }
            }

            Separator {
                id: separator
                anchors {
                    top: column.bottom
                    topMargin: Theme.paddingMedium
                }

                width: parent.width
                color: Theme.primaryColor
                horizontalAlignment: Qt.AlignHCenter
            }
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
            }

            Label {
                text: review ? review.body : ""
                width: parent.width
                wrapMode: Text.WordWrap
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
