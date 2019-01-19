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
    id: authorBooksPage

    property alias authorId: authorBooksModel.authorId
    property string authorName
    property bool busy: sailreadsManager.busy && authorBooksPage.status === PageStatus.Active

    function attachPage() {
        if (pageStack._currentContainer.attachedContainer === null
                && sailreadsManager.logged) {
            //pageStack.pushAttached(Qt.resolvedUrl("StatusPage.qml"))
        }
    }

    Component.onDestruction: {
        authorBooksModel.cancelRequest()
    }

    AuthorBooksModel {
        id: authorBooksModel
    }

    SilicaListView {
        id: authorBooksView
        anchors.fill: parent
        header: PageHeader {
            title: qsTr("%1: books").arg(authorName)
        }

        PullDownMenu {
            busy: authorBooksPage.busy
            MenuItem {
                text: qsTr("Refresh")
                onClicked: {
                    authorBooksModel.loadAuthorBooks()
                }
            }
        }

        ViewPlaceholder {
            enabled: !sailreadsManager.busy && authorBooksView.count === 0
            text: qsTr ("There are no books.\nPull down to refresh")
        }

        cacheBuffer: authorBooksPage.height
        model: authorBooksModel
        clip: true

        function fetchMoreIfNeeded() {
            if (!authorBooksPage.busy &&
                    authorBooksModel.hasMore &&
                    indexAt(contentX, contentY + height) > authorBooksModel.rowCount() - 2) {
                authorBooksModel.fetchMoreContent()
            }
        }

        onContentYChanged: fetchMoreIfNeeded()

        delegate: BookListItem {
            width: authorBooksView.width
            clip: true

            imageUrl: bookImageUrl
            title: bookTitle
            authors: bookAuthors
            averageRating: bookAverageRating
            ratingsCount: bookRatingsCount

            onClicked: {
                pageStack.push(Qt.resolvedUrl("BookPage.qml"),
                        { book: bookBook, bookId: bookId })
            }
        }

        VerticalScrollDecorator {}
    }

    BusyIndicator {
        size: BusyIndicatorSize.Large
        anchors.centerIn: parent
        running: authorBooksPage.busy
        visible: running
    }
}
