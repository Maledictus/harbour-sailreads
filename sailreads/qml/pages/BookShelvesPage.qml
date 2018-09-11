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

Page {
    id: page

    property int userId: 0
    property string userName
    property bool busy: sailreadsManager.busy && page.status === PageStatus.Active

    function attachPage() {
        if (pageStack._currentContainer.attachedContainer === null
                && sailreadsManager.logged) {
            pageStack.pushAttached(Qt.resolvedUrl("StatusPage.qml"))
        }
    }

    Component.onDestruction: {
        sailreadsManager.abortRequest()
    }

    BookShelfProxyModel {
        id: bookShelvesModel

        dynamicSortFilter: true
        sourceModel: BookShelvesModel {
            userId: page.userId
        }
    }

    SilicaListView {
        id: bookShelvesView
        anchors.fill: parent
        header: PageHeader {
            title: qsTr("Bookshelves")
        }

        PullDownMenu {
            MenuItem {
                text: qsTr("Add shelf")
                onClicked: {
                    var dialog = pageStack.push("../dialogs/AddEditShelfDialog.qml")
                    dialog.accepted.connect (function () {
                        sailreadsManager.addBookShelf(dialog.name, dialog.exclusive,
                                dialog.sortable, dialog.featured)
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

        model: bookShelvesModel

        delegate: ListItem {
            Label {
                id: shelfName
                anchors {
                    left: parent.left
                    leftMargin: Theme.horizontalPageMargin
                    right: shelfCount.left
                    rightMargin: Theme.paddingMedium
                    verticalCenter: parent.verticalCenter
                }

                truncationMode: TruncationMode.Fade
                text: bookShelfName
            }

            Label {
                id: shelfCount
                anchors {
                    right: parent.right
                    rightMargin: Theme.horizontalPageMargin
                    verticalCenter: parent.verticalCenter
                }
                color: Theme.secondaryColor
                text: bookShelfBooksCount
            }

            menu: ContextMenu {
                hasContent: userId === sailreadsManager.authUser.id
                MenuItem {
                    text: qsTr("Edit")
                    onClicked: {
                        var dialog = pageStack.push("../dialogs/AddEditShelfDialog.qml",
                            { mode: "edit", name: bookShelfName, exclusive: bookShelfExclusive,
                                sortable: bookShelfSortable, featured: bookShelfFeatured })
                        dialog.accepted.connect(function () {
                            sailreadsManager.editBookShelf(bookShelfId, dialog.name, dialog.exclusive,
                                    dialog.sortable, dialog.featured)
                            })
                     }
                }
            }

            onClicked: {
                pageStack.push(Qt.resolvedUrl("ReviewsPage.qml"),
                        { userId: userId, userName: userName,
                        bookShelfId: bookShelfId, bookShelf: bookShelfName })
            }
        }
        VerticalScrollDecorator {}
    }

    BusyIndicator {
        size: BusyIndicatorSize.Large
        anchors.centerIn: parent
        running: page.busy
        visible: running
    }
}
