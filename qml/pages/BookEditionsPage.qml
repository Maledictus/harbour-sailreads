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
    id: bookEditionsPage

    property bool busy: sailreadsManager.busy && bookEditionsPage.status === PageStatus.Active
    property string bookId
    property string reviewId
    property alias workId: bookEditionsModel.workId

    property var parentPage

    function attachPage() {
        if (pageStack._currentContainer.attachedContainer === null
                && sailreadsManager.logged) {
            //pageStack.pushAttached(Qt.resolvedUrl("StatusPage.qml"))
        }
    }

    Component.onDestruction: {
        bookEditionsModel.cancelRequest()
    }

    SilicaListView {
        id: bookEditionsView
        anchors.fill: parent
        header: PageHeader {
            title: qsTr("Book Editions")
        }

        PullDownMenu {
            busy: bookEditionsPage.busy
            MenuItem {
                text: qsTr("Refresh")
                onClicked: {
                    bookEditionsModel.loadBookEditions()
                }
            }
        }

        ViewPlaceholder {
            enabled: !sailreadsManager.busy && bookEditionsView.count === 0
            text: qsTr("There are no book editions")
            hintText: qsTr("Pull down to refresh")
        }
        model: BookEditionsModel {
            id: bookEditionsModel
        }

        function fetchMoreIfNeeded() {
            if (!bookEditionsPage.busy &&
                    bookEditionsModel.hasMore &&
                    indexAt(contentX, contentY + height) > bookEditionsModel.rowCount() - 2) {
                bookEditionsModel.fetchMoreContent()
            }
        }

        onContentYChanged: fetchMoreIfNeeded()

        delegate: BookListItem {
            width: bookEditionsView.width
            clip: true

            imageUrl: bookImageUrl
            title: bookTitle
            authors: bookAuthors
            averageRating: bookAverageRating
            ratingsCount: bookRatingsCount

            onClicked: {
                pageStack.push(Qt.resolvedUrl("BookPage.qml"),
                        { bookId: bookId, book: bookBook })
            }

            menu: ContextMenu {
                hasContent: bookBook.review
                MenuItem {
                    text: bookEditionsPage.bookId !== bookId ?
                            qsTr("Switch to This Edition") :
                            qsTr("Selected Edition")
                    enabled: bookEditionsPage.bookId !== bookId
                    onClicked: sailreadsManager.switchToBookEdition(bookBook.review.id, bookId)

                }
            }
        }
        VerticalScrollDecorator {}
    }

    Connections {
        target: sailreadsManager
        onBookEditionSwitched: {
            if (bookEditionsPage.reviewId === reviewId) {
                bookEditionsPage.bookId = bookId
                parentPage.bookId = bookId;
            }
        }
    }

    BusyIndicator {
        size: BusyIndicatorSize.Large
        anchors.centerIn: parent
        running: bookEditionsPage.busy
        visible: running
    }
}
