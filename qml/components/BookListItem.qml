/*
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

import QtQuick 2.0
import QtQuick.Layouts 1.1
import Sailfish.Silica 1.0
import harbour.sailreads 1.0

ListItem {
    id: listItem

    property alias imageUrl: bookImage.source
    property alias title: bookNameLabel.text
    property alias authors: authorsLabel.text
    property real averageRating: 0.0
    property real ratingsCount: 0.0

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
            }

            Label {
                id: authorsLabel
                width: parent.width
                truncationMode: TruncationMode.Fade
                font.pixelSize: Theme.fontSizeTiny
                text: listItem.bookAuthors
            }

            Row {
                spacing: Theme.paddingSmall
                RatingBox {
                    id: ratingBox
                    rating: listItem.averageRating
                }

                Label {
                    font.pixelSize: Theme.fontSizeExtraSmall
                    text: qsTr("%1/%2 ratings").arg(Number(listItem.averageRating).toLocaleString())
                            .arg(Number(listItem.ratingsCount).toLocaleString(Qt.locale(), 'f', 0))
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
}
