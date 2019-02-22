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
    id: userQuotesPage

    property alias userId: userQuotesModel.userId
    property string userName
    property bool busy: sailreadsManager.busy && userQuotesPage.status === PageStatus.Active

    function attachPage() {
        if (pageStack._currentContainer.attachedContainer === null
                && sailreadsManager.logged) {
            //pageStack.pushAttached(Qt.resolvedUrl("StatusPage.qml"))
        }
    }

    Component.onDestruction: {
        userQuotesModel.cancelRequest()
    }

    SilicaListView {
        id: quotesView
        anchors.fill: parent
        cacheBuffer: userQuotesPage.height
        header: PageHeader {
            title: qsTr("%1: Quotes").arg(userName)
        }

        PullDownMenu {
            busy: userQuotesPage.busy
            MenuItem {
                text: qsTr("Refresh")
                onClicked: userQuotesModel.loadUserQuotes()
            }
        }

        ViewPlaceholder {
            enabled: !sailreadsManager.busy && quotesView.count === 0
            text: qsTr("There are no quotes. Pull down to refresh")
        }
        model: UserQuotesModel {
            id: userQuotesModel
        }

        function fetchMoreIfNeeded() {
            if (!userQuotesPage.busy &&
                    userQuotesModel.hasMore &&
                    indexAt(contentX, contentY + height) > userQuotesModel.rowCount() - 2) {
                userQuotesModel.fetchMoreContent()
            }
        }

        onContentYChanged: fetchMoreIfNeeded()

        delegate: ListItem {
            width: parent.width
            contentHeight: lbl.height + separator.height + Theme.paddingMedium
            clip: true
            Label {
                id: lbl
                anchors {
                    left: parent.left
                    right: parent.right
                    margins: Theme.horizontalPageMargin
                }
                wrapMode: Text.WordWrap
                text: quoteBody
                horizontalAlignment: Qt.AlignJustify
            }

            Separator {
                id: separator
                anchors {
                    top: lbl.bottom
                    topMargin: Theme.paddingMedium
                }

                width: parent.width
                color: Theme.primaryColor
                horizontalAlignment: Qt.AlignHCenter
            }

            menu: ContextMenu {
                MenuItem {
                    text: qsTr("Copy to clipboard")
                    onClicked: Clipboard.text = quoteBody
                }
            }
        }
        VerticalScrollDecorator {}
    }

    BusyIndicator {
        size: BusyIndicatorSize.Large
        anchors.centerIn: parent
        running: userQuotesPage.busy
        visible: running
    }
}
