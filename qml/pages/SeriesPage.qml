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
    id: seriesPage

    property alias seriesId : seriesItem.seriesId
    property bool busy: sailreadsManager.busy && seriesPage.status === PageStatus.Active

    function attachPage() {
        if (pageStack._currentContainer.attachedContainer === null
                && sailreadsManager.logged) {
            pageStack.pushAttached(Qt.resolvedUrl("StatusPage.qml"))
        }
    }

    onStatusChanged: {
        if (status === PageStatus.Active) {
            attachPage()
        }
    }

    Component.onDestruction: {
        seriesItem.cancelRequest()
    }

    SeriesItem {
        id: seriesItem
    }

    SilicaListView {
        id: seriesView
        anchors.fill: parent
        cacheBuffer: seriesPage.height

        model: seriesItem.series !== null ? seriesItem.series.seriesWorks : null
        clip: true

        PullDownMenu {
            MenuItem {
                text: qsTr("Refresh")
                onClicked: seriesItem.loadSeries()
            }
        }

        ViewPlaceholder {
            enabled: !sailreadsManager.busy && seriesView.count === 0
            text: qsTr("There are no books in this series")
            hintText: qsTr("Pull down to refresh")
        }

        header: Item {
            width: seriesView.width
            height: headerBox.height
        }

        contentY: -headerItem.height

        delegate: BookListItem {
            width: seriesView.width
            clip: true

            imageUrl: modelData.work !== null && modelData.work.bestBook !== null ?
                    modelData.work.bestBook.imageUrl : ""
            title: modelData.work !== null && modelData.work.bestBook !== null ?
                    modelData.work.bestBook.title : ""
            authors: modelData.work !== null && modelData.work.bestBook !== null ?
                    modelData.work.bestBook.authorsString : ""
            averageRating: modelData.work !== null ? modelData.work.averageRating : 0.0
            ratingsCount: modelData.work !== null ? modelData.work.ratingsCount : 0

            onClicked: {
                pageStack.push(Qt.resolvedUrl("BookPage.qml"),
                        { bookId: (modelData.work !== null && modelData.work.bestBook !== null ?
                                modelData.work.bestBook.id : "") })
            }
        }

        VerticalScrollDecorator {}
    }

    Column {
        id: headerBox
        parent:seriesView.headerItem ? seriesView.headerItem : seriesPage
        width: parent.width

        spacing: Theme.paddingSmall

        PageHeader {
            id: pageHeader
            title: seriesItem.series !== null ? seriesItem.series.title : qsTr("Series")
        }

        Label {
            anchors {
                left: parent.left
                leftMargin: Theme.horizontalPageMargin
                right: parent.right
                rightMargin: Theme.horizontalPageMargin
            }
            wrapMode: Text.WordWrap
            color: Theme.highlightColor
            font.family: Theme.fontFamilyHeading
            font.pixelSize: Theme.fontSizeMedium
            text: seriesItem.series !== null ? seriesItem.series.title : ""
        }

        Row {
            anchors {
                left: parent.left
                leftMargin: Theme.horizontalPageMargin
                right: parent.right
                rightMargin: Theme.horizontalPageMargin
            }
            spacing: Theme.paddingMedium
            Label {
                font.pixelSize: Theme.fontSizeTiny
                wrapMode: Text.WordWrap
                color: Theme.highlightColor
                visible: seriesItem.series !== null
                text: qsTr("%Ln works", "", seriesItem.series ? seriesItem.series.seriesWorksCount : 0)
            }
            Label {
                font.pixelSize: Theme.fontSizeTiny
                wrapMode: Text.WordWrap
                color: Theme.highlightColor
                visible: seriesItem.series !== null
                text: qsTr("%Ln primary works", "", seriesItem.series ? seriesItem.series.primaryWorkCount : 0)
            }
        }

        Item {
            width: parent.width
            height: Theme.paddingLarge
        }

        property string _style: "<style>" +
                "a:link { color:" + Theme.highlightColor + "; }" +
                "p { color:" + Theme.primaryColor + "; }" +
                "</style>"

        CollapsedLabel {
            anchors {
                left: parent.left
                leftMargin: Theme.horizontalPageMargin
                right: parent.right
                rightMargin: Theme.horizontalPageMargin
            }
            visible: seriesItem.series && seriesItem.series.description !== ""
            text: seriesItem.series !== null ? (headerBox._style + seriesItem.series.description) : ""
            onLinkActivated: mainWindow.openPageFromUrl(link)
            onHeightChanged: seriesView.contentY = -headerBox.height
        }

        Item {
            width: parent.width
            height: Theme.paddingLarge
        }
    }

    BusyIndicator {
        size: BusyIndicatorSize.Large
        anchors.centerIn: parent
        running: seriesPage.busy
        visible: running
    }
}
