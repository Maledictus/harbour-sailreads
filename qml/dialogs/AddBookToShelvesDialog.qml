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

import QtQuick 2.0
import Sailfish.Silica 1.0
import harbour.sailreads 1.0

import "../components"
import "../utils/Utils.js" as Utils

Page {
    id: editBookshelvesPage

    property bool busy: sailreadsManager.busy && editBookshelvesPage.status === PageStatus.Active

    property var usedShelves: []

    BookShelvesModel {
        id: bookShelvesModel
        //"12934309" "51756959" sailreadsManager.authUser.id
        userId: sailreadsManager.authUser.id
        preloadAll: true
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
                title: qsTr("Edit bookshelves")
            }

            ComboBox {
                width: parent.width
                label: qsTr("Bookshelf")
                menu: ContextMenu {
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
                onTextChanged: tagsModel.filterRegExp = new RegExp(text)
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
                        filterRegExp: new RegExp("")
                        filterCaseSensitivity: Qt.CaseInsensitive
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
                        selected: usedShelves.indexOf(bookShelfName) > -1
                        onClicked: {
                            if (selected) {
                                console.log("remove from")
                            }
                            else {
                                console.log("add to")
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
