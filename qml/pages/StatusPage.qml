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
TH SOFTWARE.
*/

import QtQuick 2.6
import Sailfish.Silica 1.0
import harbour.sailreads 1.0

import "../pages"
import "../components"
import "../utils/Utils.js" as Utils

Page {
    id: statusPage

    property bool busy: sailreadsManager.busy && statusPage.status === PageStatus.Active
    property var reviewsModel: mainWindow.currentlyReadingModel

    SilicaListView {
        id: reviewsView
        anchors.fill: parent
        header: PageHeader {
            title: qsTr("Currently-Reading books")
        }

        PullDownMenu {
            busy: statusPage.busy
            MenuItem {
                text: qsTr("Refresh")
                onClicked: reviewsModel.loadReviews()
            }
        }

        ViewPlaceholder {
            enabled: !sailreadsManager.busy && reviewsView.count === 0
            text: qsTr("There are no currently-reading books")
            hintText: qsTr("Pull down to refresh")
        }

        model: reviewsModel

        function fetchMoreIfNeeded() {
            if (!statusPage.busy &&
                    reviewsModel.hasMore &&
                    indexAt(contentX, contentY + height) > reviewsModel.rowCount() - 2) {
                reviewsModel.fetchMoreContent()
            }
        }

        onContentYChanged: fetchMoreIfNeeded()

        delegate: BookListItem {
            id: rootDelegateItem
            width: reviewsView.width
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
        running: statusPage.busy
        visible: running
    }
}
