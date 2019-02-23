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
    id: authorQuotesPage

    property alias authorId: authorQuotesModel.authorId
    property string authorName
    property bool busy: sailreadsManager.busy && authorQuotesPage.status === PageStatus.Active

    function attachPage() {
        if (pageStack._currentContainer.attachedContainer === null
                && sailreadsManager.logged) {
            //pageStack.pushAttached(Qt.resolvedUrl("StatusPage.qml"))
        }
    }

    Component.onDestruction: {
        authorQuotesModel.cancelRequest()
    }

    SilicaListView {
        id: quotesView
        anchors.fill: parent
        cacheBuffer: authorQuotesPage.height
        header: PageHeader {
            title: qsTr("Quotes: %1").arg(authorName)
        }

        PullDownMenu {
            busy: authorQuotesPage.busy
            MenuItem {
                text: qsTr("Refresh")
                onClicked: authorQuotesModel.loadBookQuotes()
            }
        }

        ViewPlaceholder {
            enabled: !sailreadsManager.busy && quotesView.count === 0
            text: qsTr("There are no quotes")
            hintText: qsTr("Pull down to refresh")
        }

        model: AuthorQuotesModel {
            id: authorQuotesModel
        }

        function fetchMoreIfNeeded() {
            if (!authorQuotesPage.busy &&
                    authorQuotesModel.hasMore &&
                    indexAt(contentX, contentY + height) > authorQuotesModel.rowCount() - 2) {
                authorQuotesModel.fetchMoreContent()
            }
        }

        onContentYChanged: fetchMoreIfNeeded()

        delegate: ListItem {
            id: listItem
            width: parent.width
            contentHeight: column.height + separator.height + Theme.paddingMedium
            clip: true
            Column {
                id: column
                anchors {
                    left: parent.left
                    right: parent.right
                    margins: Theme.horizontalPageMargin
                }
                Label {
                    id: lbl
                    width: parent.width
                    wrapMode: Text.WordWrap
                    text: quoteBody
                    color: listItem.highlighted ? Theme.highlightColor : Theme.primaryColor
                    horizontalAlignment: Qt.AlignJustify
                }
                ClickableKeyValueLabel {
                    key: qsTr("Book")
                    visible: quoteBook !== ""
                    value: quoteBook
                    highlighted: listItem.highlighted
                    onClicked: mainWindow.openPageFromUrl(quoteBookLink)
                }
            }

            Separator {
                id: separator
                anchors {
                    top: column.bottom
                    topMargin: Theme.paddingMedium
                }

                width: parent.width
                color: Theme.primaryColor
                horizontalAlignment: Qt.AlignHCenter
            }

            menu: ContextMenu {
                MenuItem {
                    text: qsTr("Copy quote to clipboard")
                    onClicked: Clipboard.text = quoteBody
                }
            }
        }
        VerticalScrollDecorator {}
    }

    BusyIndicator {
        size: BusyIndicatorSize.Large
        anchors.centerIn: parent
        running: authorQuotesPage.busy
        visible: running
    }
}
