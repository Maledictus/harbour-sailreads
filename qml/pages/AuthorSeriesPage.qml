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
import "../components"

Page {
    id: authorSeriesPage

    property alias authorId: authorSeriesModel.authorId
    property string authorName
    property bool busy: sailreadsManager.busy && authorSeriesPage.status === PageStatus.Active

    function attachPage() {
        if (pageStack._currentContainer.attachedContainer === null
                && sailreadsManager.logged) {
            //pageStack.pushAttached(Qt.resolvedUrl("StatusPage.qml"))
        }
    }

    Component.onDestruction: {
        sailreadsManager.abortRequest()
    }

    AuthorSeriesModel {
        id: authorSeriesModel
    }

    SilicaListView {
        id: authorSeriesView
        anchors.fill: parent
        header: PageHeader {
            title: qsTr("%1: Series").arg(authorName)
        }

        PullDownMenu {
            MenuItem {
                text: qsTr("Refresh")
                onClicked: {
                    sailreadsManager.loadAuthorSeries(authorId)
                }
            }
        }

        ViewPlaceholder {
            enabled: !sailreadsManager.busy && authorSeriesView.count === 0
            text: qsTr ("There are no series.\nPull down to refresh")
        }

        cacheBuffer: authorSeriesPage.height
        model: authorSeriesModel
        clip: true

        function fetchMoreIfNeeded() {
            if (!authorSeriesPage.busy &&
                    authorSeriesModel.hasMore &&
                    indexAt(contentX, contentY + height) > authorSeriesModel.rowCount() - 2) {
                authorSeriesModel.fetchMoreContent()
            }
        }

        onContentYChanged: fetchMoreIfNeeded()

        delegate: ListItem{
            height: Theme.itemSizeMedium
        }

        VerticalScrollDecorator {}
    }

    BusyIndicator {
        size: BusyIndicatorSize.Large
        anchors.centerIn: parent
        running: authorSeriesPage.busy
        visible: running
    }
}
