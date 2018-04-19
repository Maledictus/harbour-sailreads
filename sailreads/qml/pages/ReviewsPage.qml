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
import Sailfish.Silica 1.0
import harbour.sailreads 1.0

import "../components"

Page {
    id: reviewsPage

    property alias userId: reviewsModel.userId
    property alias bookShelfId: reviewsModel.bookShelfId
    property alias bookShelf: reviewsModel.bookShelf

    function attachPage() {
        if (pageStack._currentContainer.attachedContainer === null
                && sailreadsManager.logged) {
            pageStack.pushAttached(Qt.resolvedUrl("StatusPage.qml"))
        }
    }

    ReviewsModel {
        id: reviewsModel
    }

    SilicaListView {
        id: reviewsView
        anchors.fill: parent
        header: PageHeader {
            title: bookShelf
        }

        PullDownMenu {
            MenuItem {
                text: qsTr("Refresh")
                onClicked: {
                    sailreadsManager.loadReviews(userId, bookShelf)
                }
            }
        }

        ViewPlaceholder {
            enabled: !sailreadsManager.busy && reviewsView.count === 0
            text: qsTr("There are no books reviews. Pull down to refresh")
        }

        model: reviewsModel

        function fetchMoreIfNeeded() {
            if (!reviewsPage.busy &&
                    reviewsModel.hasMore &&
                    indexAt(contentX, contentY + height) > reviewsModel.rowCount() - 2) {
                reviewsModel.fetchMoreContent()
            }
        }

        onContentYChanged: fetchMoreIfNeeded()

        delegate: ListItem {
            width: reviewsView.width
            contentHeight: Math.max(bookImage.height, column.height) + 2* Theme.paddingSmall
            clip: true

            Image {
                id: bookImage
                anchors {
                    left: parent.left
                    leftMargin: Theme.horizontalPageMargin
                    top: column.top
                    topMargin: Theme.paddingSmall
                }
                source: reviewBook.imageUrl
                height: Theme.iconSizeLarge
                width: Theme.iconSizeMedium
                fillMode: Image.PreserveAspectFit
                horizontalAlignment: Image.AlignLeft
                verticalAlignment: Image.AlignTop

                BusyIndicator {
                    size: BusyIndicatorSize.Medium
                    anchors.centerIn: parent
                    running: bookImage.status == Image.Loading
                    visible: running
                }
            }

            Column {
                id: column

                anchors {
                    right: parent.right
                    rightMargin: Theme.horizontalPageMargin
                    left: bookImage.right
                    leftMargin: Theme.paddingMedium
                }

                Label {
                    id: bookNameLabel
                    width: parent.width
                    wrapMode: Text.WordWrap
                    maximumLineCount: 2
                    font.pixelSize: Theme.fontSizeSmall
                    text: reviewBook.title
                }

                Label {
                    id: authorsLabe
                    width: parent.width
                    truncationMode: TruncationMode.Fade
                    font.pixelSize: Theme.fontSizeTiny
                    text: qsTr("by %1").arg(reviewBook.authors)
                }

                Row {
                    spacing: Theme.paddingSmall
                    RatingBox {
                        rating: reviewBook.averageRating
                    }

                    Label {
                        font.pixelSize: Theme.fontSizeExtraSmall
                        text: qsTr("%1/%2 ratings").arg(reviewBook.averageRating)
                                .arg(Number(reviewBook.ratingsCount).toFixed())
                    }
                }
            }
        }

        VerticalScrollDecorator {}
    }

    BusyIndicator {
        size: BusyIndicatorSize.Large
        anchors.centerIn: parent
        running: sailreadsManager.busy
        visible: running
    }
}
