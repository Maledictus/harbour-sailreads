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
import QtQuick.Layouts 1.1
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
            //pageStack.pushAttached(Qt.resolvedUrl("StatusPage.qml"))
        }
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

        header: Item {
            width: seriesView.width
            height: headerBox.height
        }

        delegate: ListItem {
            width: seriesView.width
            contentHeight: row.height + separator.height + Theme.paddingMedium
            clip: true

            Row {
                id: row
                spacing: Theme.paddingMedium
                height: Math.max(bookImage.height, column.height)
                anchors {
                    left: parent.left
                    leftMargin: Theme.horizontalPageMargin
                    right: parent.right
                    rightMargin: Theme.horizontalPageMargin
                }
                BaseImage {
                    id: bookImage
                    anchors {
                        top: column.top
                        topMargin: Theme.paddingSmall
                    }

                    source: modelData.work !== null && modelData.work.bestBook !== null ?
                            modelData.work.bestBook.imageUrl : ""
                    height: 1.5 * width
                    width: Theme.iconSizeLarge
                    horizontalAlignment: Image.AlignLeft
                    verticalAlignment: Image.AlignTop
                    indicator.size: BusyIndicatorSize.Medium
                }

                Column {
                    id: column
                    width: parent.width - bookImage.width - Theme.paddingMedium
                    Label {
                        id: bookNameLabel
                        width: parent.width
                        wrapMode: Text.WordWrap
                        maximumLineCount: 2
                        font.family: Theme.fontFamilyHeading
                        font.pixelSize: Theme.fontSizeSmall
                        text: modelData.work !== null && modelData.work.bestBook !== null ?
                                modelData.work.bestBook.title : ""
                    }

                    Label {
                        id: authorsLabe
                        width: parent.width
                        truncationMode: TruncationMode.Fade
                        font.pixelSize: Theme.fontSizeTiny
                        text: modelData.work !== null && modelData.work.bestBook !== null ?
                                modelData.work.bestBook.authorsString : ""
                    }

                    Row {
                        spacing: Theme.paddingSmall
                        RatingBox {
                            rating: modelData.work !== null ? modelData.work.averageRating : 0
                        }

                        Label {
                            font.pixelSize: Theme.fontSizeExtraSmall
                            text: qsTr("%1/%2 ratings").arg(modelData.work !== null ?
                                        Number(modelData.work.averageRating).toLocaleString()
                                        : 0)
                                    .arg(Number(modelData.work !== null ?
                                        modelData.work.ratingCount : 0)
                                            .toLocaleString(Qt.locale(), 'f', 0))
                        }
                    }
                }
            }

            Separator {
                id: separator
                anchors {
                    top: row.bottom
                    topMargin: Theme.paddingMedium
                }

                width: parent.width
                color: Theme.primaryColor
                horizontalAlignment: Qt.AlignHCenter
            }

            onClicked: {
                pageStack.push(Qt.resolvedUrl("BookPage.qml"),
                        { bookId: (modelData.work !== null && modelData.work.bestBook !== null ?
                                modelData.work.bestBook.id : 0) })
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
            text: {
                return seriesItem.series !== null ? seriesItem.series.title : ""
            }
        }

        Label {
            anchors {
                left: parent.left
                leftMargin: Theme.horizontalPageMargin
                right: parent.right
                rightMargin: Theme.horizontalPageMargin
            }
            font.pixelSize: Theme.fontSizeTiny
            width: parent.width
            wrapMode: Text.WordWrap
            color: Theme.highlightColor
            visible: seriesItem.series !== null
            text: qsTr("%1 works, %2 primary works")
                    .arg(Number(seriesItem.series !== null ? seriesItem.series.seriesWorksCount : 0).toFixed())
                    .arg(Number(seriesItem.series !== null ? seriesItem.series.primaryWorkCount : 0).toFixed())
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
            text: seriesItem.series !== null ? (headerBox._style + seriesItem.series.description) : ""
            onLinkActivated: {
                Qt.openUrlExternally(link)
            }
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
