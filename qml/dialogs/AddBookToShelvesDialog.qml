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

Dialog {
    id: addBookToShelvesDialog

    property bool busy: sailreadsManager.busy && addBookToShelvesDialog.status === PageStatus.Active

    property var usedShelves: []
    property var newShelves: []

    BookShelvesModel {
        id: bookShelvesModel
        userId: {
            return sailreadsManager.authUser.id
        }
    }

    SilicaListView {
        id: bookshelvesView
        anchors.fill: parent

        header: DialogHeader {
            acceptText: qsTr("Add")
            cancelText: qsTr("Cancel")
        }

        ViewPlaceholder {
            enabled: !busy && !bookshelvesView.count
            text: qsTr("There are no bookshelves")
        }

        spacing: Theme.paddingSmall

        cacheBuffer: addBookToShelvesDialog.height
        model: bookShelvesModel

        function fetchMoreIfNeeded() {
            if (!addBookToShelvesDialog.busy &&
                    bookShelvesModel.hasMore &&
                    indexAt(contentX, contentY + height) > bookShelvesModel.rowCount() - 2) {
                bookShelvesModel.fetchMoreContent()
            }
        }

        onContentYChanged: fetchMoreIfNeeded()

        delegate: ListItem {
            id: rootDelegateItem

            width: bookshelvesView.width

            TextSwitch {
                id: bookshelfSwitch
                text: bookShelfName
                checked: usedShelves.indexOf(bookShelfName) !== -1
                onCheckedChanged: {
                    if (checked) {
                        newShelves.push(bookShelfName)
                    }
                    else {
                        var index = newShelves.indexOf(bookShelfName);
                        if (index > -1) {
                            newShelves.splice(index, 1);
                        }
                    }
                }
            }
        }

        VerticalScrollDecorator {}
    }

    BusyIndicator {
        size: BusyIndicatorSize.Large
        anchors.centerIn: parent
        running: addBookToShelvesDialog.busy
        visible: running
    }

    canAccept: bookshelvesView.count > 0
}
