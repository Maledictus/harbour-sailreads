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
import QtQuick.Layouts 1.1
import Sailfish.Silica 1.0

import "../components"
import "../utils/Utils.js" as Utils

Item {
    anchors.fill: parent

    property alias model: listView.model
    visible: listView.count > 0

    SilicaListView {
        id: listView
        anchors {
            fill: parent
            margins: Theme.paddingSmall
        }
        clip: true

        orientation: Qt.Horizontal
        highlightRangeMode: ListView.StrictlyEnforceRange
        snapMode: ListView.SnapOneItem

        delegate: BackgroundItem {
            id: delegate
            width: listView.width
            height: listView.height
            BaseImage {
                source: reviewBook.imageUrl
                anchors {
                    left: parent.left
                    right: parent.right
                    top: parent.top
                    bottom: column.top
                }
            }
            Column {
                id: column
                anchors {
                    left: parent.left
                    right: parent.right
                    bottom: parent.bottom
                }
                Label {
                    id: bookNameLabel
                    width: parent.width
                    wrapMode: Text.WordWrap
                    maximumLineCount: 2
                    font.pixelSize: Theme.fontSizeExtraSmall
                    horizontalAlignment: Qt.AlignHCenter
                    text: reviewBook.titleWithoutSeries
                }
                Label {
                    id: authorLabel
                    width: parent.width
                    wrapMode: Text.WordWrap
                    maximumLineCount: 2
                    text: reviewBook.authorsString
                    font.pixelSize: Theme.fontSizeExtraSmall
                    color: Theme.secondaryColor
                    horizontalAlignment: Qt.AlignHCenter
                }
            }
        }
    }
}
