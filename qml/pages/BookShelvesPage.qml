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
    id: bookShelvesPage

    property int userId: 0
    property string userName
    property bool busy: sailreadsManager.busy && bookShelvesPage.status === PageStatus.Active
    property bool showSearchField: applicationSettings.value("bookshelves/showSearchField", true) === true

    function attachPage() {
        if (pageStack._currentContainer.attachedContainer === null
                && sailreadsManager.logged) {
//            //pageStack.pushAttached(Qt.resolvedUrl("StatusPage.qml"))
        }
    }

    Component.onDestruction: {
        sailreadsManager.abortRequest()
    }

    BaseProxyModel {
        id: bookShelvesProxyModel
        dynamicSortFilter: true
        filterRole: BookShelvesModel.Name
        sourceModel: BookShelvesModel {
            id: bookShelvesModel
            userId: bookShelvesPage.userId
        }
    }

    SilicaListView {
        id: bookShelvesView
        anchors.fill: parent

        header: Column {
            id: headerColumn
            width: bookShelvesView.width
            property alias description: pageHeader.description
            property alias searchField: search
            PageHeader {
                id: pageHeader
                title: qsTr("Bookshelves")
            }

            SearchField {
                id: search
                visible: showSearchField
                anchors.left: parent.left
                anchors.right: parent.right
                placeholderText: qsTr("Search")
                onTextChanged: {
                    bookShelvesProxyModel.filterRegExp = new RegExp(text)
                    search.forceActiveFocus()
                    bookShelvesView.currentIndex = -1
                }
            }
        }

        PullDownMenu {
            MenuItem {
                text: showSearchField ? qsTr("Hide search field") : qsTr("Show search field")
                onClicked: {
                    showSearchField = !showSearchField
                    applicationSettings.setValue("bookshelves/showSearchField", showSearchField)
                }
            }
            MenuItem {
                text: qsTr("Add shelf")
                onClicked: {
                    var dialog = pageStack.push("../dialogs/AddEditShelfDialog.qml")
                    dialog.accepted.connect (function () {
                        sailreadsManager.addBookShelf(dialog.name, dialog.exclusive,
                                dialog.sortable, dialog.featured, dialog.recommendFor)
                    })
                }
            }
            MenuItem {
                text: qsTr("Refresh")
                onClicked: {
                    sailreadsManager.loadBookShelves(userId)
                }
            }
        }

        ViewPlaceholder {
            enabled: !sailreadsManager.busy && bookShelvesView.count === 0
            text: qsTr("There are no bookshelves. Pull down to refresh")
        }

        cacheBuffer: bookShelvesPage.height
        model: bookShelvesProxyModel

        function fetchMoreIfNeeded() {
            if (headerItem.searchField.text === "" &&
                !bookShelvesPage.busy &&
                    bookShelvesModel.hasMore &&
                    indexAt(contentX, contentY + height) > bookShelvesModel.rowCount() - 2) {
                bookShelvesModel.fetchMoreContent()
            }
        }

        onContentYChanged: fetchMoreIfNeeded()

        delegate: BookShelfListItem {
            shelfName: bookShelfName
            shelfBooksCount: bookShelfBooksCount

            menu: ContextMenu {
                hasContent: userId === sailreadsManager.authUser.id
                MenuItem {
                    text: qsTr("Edit")
                    onClicked: {
                        var dialog = pageStack.push("../dialogs/AddEditShelfDialog.qml",
                            { mode: "edit", name: bookShelfName, exclusive: bookShelfExclusive,
                                sortable: bookShelfSortable, featured: bookShelfFeatured,
                                recommendFor: bookShelfRecommendFor })
                        dialog.accepted.connect(function () {
                            sailreadsManager.editBookShelf(bookShelfId, dialog.name, dialog.exclusive,
                                    dialog.sortable, dialog.featured, dialog.recommendFor)
                            })
                     }
                }
            }

            onClicked: {
                pageStack.push(Qt.resolvedUrl("BookReviewsPage.qml"),
                        { userId: userId, userName: userName,
                        bookShelfId: bookShelfId, bookShelf: bookShelfName })
            }
        }
        VerticalScrollDecorator {}
    }

    BusyIndicator {
        size: BusyIndicatorSize.Large
        anchors.centerIn: parent
        running: bookShelvesPage.busy
        visible: running
    }
}
