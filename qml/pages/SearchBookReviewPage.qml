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

Page {
    id: searchBookReviewPage

    property bool busy: sailreadsManager.busy && searchBookReviewPage.status === PageStatus.Active

    function attachPage() {
        if (pageStack._currentContainer.attachedContainer === null
                && sailreadsManager.logged) {
//            //pageStack.pushAttached(Qt.resolvedUrl("StatusPage.qml"))
        }
    }

    Component.onDestruction: {
        foundBookReviewsModel.cancelRequest()
    }

    SearchReviewsModel {
        id: foundBookReviewsModel
    }

    SilicaListView {
        id: foundReviewsView
        anchors.fill: parent

        header: Column {
            id: headerColumn
            width: foundReviewsView.width
            PageHeader {
                title: qsTr("Search review")
            }

            SearchField {
                id: search
                anchors.left: parent.left
                anchors.right: parent.right
                placeholderText: qsTr("Search")

                EnterKey.enabled: text.length > 0
                EnterKey.iconSource: "image://theme/icon-m-enter-accept"
                EnterKey.onClicked: sailreadsManager.searchReviews(sailreadsManager.authUser.id, text)
            }
        }

        cacheBuffer: searchBookReviewPage.height
        model: foundBookReviewsModel

        function fetchMoreIfNeeded() {
            if (!searchBookReviewPage.busy &&
                    foundBookReviewsModel.hasMore &&
                    indexAt(contentX, contentY + height) > foundBookReviewsModel.rowCount() - 2) {
                foundBookReviewsModel.fetchMoreContent()
            }
        }

        onContentYChanged: fetchMoreIfNeeded()

        delegate: BookListItem {
            width: foundReviewsView.width
            clip: true

            imageUrl: reviewBook.imageUrl
            title: reviewBook.title
            authors: reviewBook.authorsString
            averageRating: reviewBook.averageRating
            ratingsCount: reviewBook.ratingsCount

            onClicked: {
                pageStack.push(Qt.resolvedUrl("BookPage.qml"),
                        { bookId: reviewBook.id, book: reviewBook })
            }
        }
        VerticalScrollDecorator {}
    }

    BusyIndicator {
        size: BusyIndicatorSize.Large
        anchors.centerIn: parent
        running: searchBookReviewPage.busy
        visible: running
    }
}
