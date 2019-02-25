/*
The MIT License(MIT)

Copyright (c) 2018-2019 Oleg Linkin <maledictusdemagog@gmail.com>

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files(the "Software"), to deal
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
    id: editBookshelvesPage

    property bool busy: sailreadsManager.busy && editBookshelvesPage.status === PageStatus.Active

    property string bookId
    property var book
    property var review

    Connections {
        target: sailreadsManager
        onReviewUpdated: {
            if (editBookshelvesPage.bookId != bookId) {
                return
            }
            if (editBookshelvesPage.review && editBookshelvesPage.review.id === review.id) {
                editBookshelvesPage.review.updateShelves(review)
            }
            else {
                editBookshelvesPage.review = review
            }

            for (var i = 0; i < contextMenu.children.length; ++i) {
                if (review.shelvesList.indexOf(contextMenu.children[i].text) > -1) {
                    bookShelvesComboBox.currentIndex = i
                    bookShelvesComboBox.prevIndex = i
                    return
                }
            }
            bookShelvesComboBox.currentIndex = -1
        }
    }

    function setSelectedShelf() {
        for (var i = 0; i < contextMenu.children.length; ++i) {
            if (review.shelvesList.indexOf(contextMenu.children[i].text) > -1) {
                bookShelvesComboBox.currentIndex = i
                bookShelvesComboBox.prevIndex = i
                return
            }
        }
        bookShelvesComboBox.currentIndex = -1
    }

    BookShelvesModel {
        id: bookShelvesModel
        userId: sailreadsManager.authUser.id
        preloadAll: true

        onHasMoreChanged: {
            if (!hasMore && review) {
                setSelectedShelf()
            }
            else {
                bookShelvesComboBox.currentIndex = -1
            }
        }
    }

    Component.onCompleted: {
        if (bookShelvesModel.preloadAll) {
            bookShelvesModel.loadAllBookShelves()
        }
    }


    SilicaFlickable {
        anchors.fill: parent
        contentHeight: column.height + Theme.paddingMedium
        Column {
            id: column
            width: parent.width
            spacing: Theme.paddingMedium

            PageHeader {
                title: qsTr("Add to My Books")
            }

            PullDownMenu {
                MenuItem {
                    text: qsTr("Add new shelf or tag")
                    onClicked: {
                        var dialog = pageStack.push("../dialogs/AddEditShelfDialog.qml")
                        dialog.accepted.connect (function () {
                            sailreadsManager.addBookShelf(dialog.name, dialog.exclusive,
                                    dialog.sortable, dialog.featured, dialog.recommendFor)
                        })
                    }
                }
            }

            ShortBookRow {
                anchors {
                    left: parent.left
                    leftMargin: Theme.horizontalPageMargin
                    right: parent.right
                    rightMargin: Theme.horizontalPageMargin
                }

                width: parent.width
                bookImage: book ? book.imageUrl : ""
                bookTitle: book ? book.title: ""
                bookAuthors: book ? Utils.getAuthorsString(book.authors, Theme.primaryColor) : ""
                bookAverageRating: book ? book.averageRating : 0
                bookRatingsCount: book ? book.ratingsCount : 0
                onBookClicked: pageStack.push(Qt.resolvedUrl("../pages/BookPage.qml"),
                        { bookId: book ? book.id : "",
                            book : book })
                onAuthorLinkActivated: pageStack.push(Qt.resolvedUrl("../pages/AuthorPage.qml"),
                        { authorId : link })
            }

            ComboBox {
                id: bookShelvesComboBox
                width: parent.width
                label: qsTr("Bookshelf")

                property int prevIndex: -1
                function update(index) {
                    if (index === prevIndex) {
                        return
                    }
                    sailreadsManager.addBookToShelves(bookId, [ contextMenu.children[index].text ],
                            review ? review.shelvesList : [])

                    prevIndex = index
                }

                menu: ContextMenu {
                    id: contextMenu
                    onActivated: bookShelvesComboBox.update(index)
                    Repeater {
                        width: parent.width
                        delegate: MenuItem {
                            text: bookShelfName
                        }
                        model: BaseProxyModel {
                            filterRole: BookShelvesModel.Exclusive
                            filterRegExp: new RegExp("true")
                            dynamicSortFilter: true
                            sourceModel: bookShelvesModel
                        }
                    }
                }
            }

            SectionHeader {
                text: qsTr("Tags")
            }

            SearchField {
                anchors.left: parent.left
                anchors.right: parent.right
                placeholderText: qsTr("Search")
                onTextChanged: tagsModel.filterRegExp = new RegExp(text, "i")
            }

            Flow {
                id: tagsFlow
                spacing: Theme.paddingMedium
                anchors {
                    left: parent.left
                    leftMargin: Theme.horizontalPageMargin
                    right: parent.right
                    rightMargin: Theme.horizontalPageMargin
                }

                Repeater {
                    model: BaseProxyModel {
                        id: tagsModel
                        filterRole: BookShelvesModel.Name
                        dynamicSortFilter: true
                        sourceModel: BaseProxyModel {
                            filterRole: BookShelvesModel.Exclusive
                            filterRegExp: new RegExp("false")
                            dynamicSortFilter: true
                            sourceModel: bookShelvesModel
                        }
                    }

                    delegate: TagDelegate {
                        view: tagsFlow
                        name: bookShelfName
                        count: bookShelfBooksCount
                        selected: review ? review.shelvesList.indexOf(bookShelfName) > -1 : false
                        onClicked: {
                            if (selected) {
                                sailreadsManager.removeBookFromShelf(bookId, bookShelfName)
                            }
                            else {
                                sailreadsManager.addBookToShelves(bookId, [bookShelfName],
                                        review ? review.shelvesList : [])
                            }

                            selected = !selected
                        }
                    }
                }
            }
        }
    }

    BusyIndicator {
        size: BusyIndicatorSize.Large
        anchors.centerIn: parent
        running: editBookshelvesPage.busy
        visible: running
    }
}
