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

Page {
    id: searchPage

    property bool busy: sailreadsManager.busy && searchPage.status === PageStatus.Active

    function attachPage() {
        if (pageStack._currentContainer.attachedContainer === null
                && sailreadsManager.logged) {
//            //pageStack.pushAttached(Qt.resolvedUrl("StatusPage.qml"))
        }
    }

    Component.onDestruction: {
        booksModel.cancelRequest()
    }

    FoundBooksModel {
        id: booksModel
    }

    SilicaListView {
        id: foundView
        anchors.fill: parent

        cacheBuffer: searchPage.height
        model: booksModel

        function fetchMoreIfNeeded() {
            if (!searchPage.busy &&
                    booksModel.hasMore &&
                    indexAt(contentX, contentY + height) > booksModel.rowCount() - 2) {
                booksModel.fetchMoreContent()
            }
        }
        onContentYChanged: fetchMoreIfNeeded()

        header: Item {
            width: foundView.width
            height: searchSettingsBox.height
        }

        delegate: BookListItem {
            width: foundView.width
            clip: true

            imageUrl: bookImageUrl
            title: bookTitle
            authors: bookAuthors
            averageRating: bookAverageRating
            ratingsCount: bookRatingsCount

            bookShelfButton.visible: true
            selected: bookBook.review
            bookShelfButton.label.text: !selected ? qsTr("Want to Read") :
                    (bookBook.review ? bookBook.review.exclusiveShelf : "")
            bookShelfButton.onClicked: {
                if (!selected) {
                    sailreadsManager.addBookToShelves(bookId, ["to-read"])
                }
                else {
                    pageStack.push("AddBookToShelvesPage.qml",
                            { bookId: bookBook.id, book: bookBook, review: bookBook.review })
                }
            }

            onClicked: {
                pageStack.push(Qt.resolvedUrl("BookPage.qml"),
                        { book: bookBook, bookId: bookId })
            }

            menu: ContextMenu {
                MenuItem {
                    text: qsTr("Add to My Books")
                    onClicked: {
                        pageStack.push("AddBookToShelvesPage.qml",
                                { bookId: bookBook.id, book: bookBook, review: bookBook.review })
                    }
                }
            }
        }

        VerticalScrollDecorator {}
    }

    Column {
        id: searchSettingsBox
        parent: foundView.headerItem ? foundView.headerItem : userStatusPage
        width: parent.width
        spacing: Theme.paddingMedium

        PageHeader {
            title: qsTr("Search")
        }

        Column {
            width: parent.width
            SearchField {
                id: searchField
                width: parent.width

                EnterKey.enabled: text.length > 0
                EnterKey.iconSource: "image://theme/icon-m-enter-accept"
                EnterKey.onClicked: {
                    booksModel.search = text
                    booksModel.field = subSearchType.field
                    booksModel.searchBooks()
                }
            }

            ComboBox {
                id: subSearchType
                property string field: "all"
                property var fields: ["all", "title", "author"]
                label: qsTr("Field")
                function update() {
                    field = fields[currentIndex]
                }
                menu: ContextMenu {
                    onClosed: subSearchType.update()
                    MenuItem { text: qsTr("Title and Author") }
                    MenuItem { text: qsTr("Title") }
                    MenuItem { text: qsTr("Author") }
                }
            }
        }
    }

    BusyIndicator {
        size: BusyIndicatorSize.Large
        anchors.centerIn: parent
        running: searchPage.busy
        visible: running
    }
}
