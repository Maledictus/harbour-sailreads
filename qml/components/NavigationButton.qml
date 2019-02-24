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

MouseArea {
    id: mouseArea
    property alias icon: image
    property alias text: lbl.text
    property alias fontSize: lbl.font.pixelSize
    property bool selected: false

    property bool down: pressed && containsMouse
    property bool highlighted: down

    height: childrenRect.height
    Column {
        width: parent.width
        HighlightImage {
            id: image
            anchors.horizontalCenter: parent.horizontalCenter
            horizontalAlignment: Qt.AlignHCenter
            highlighted: mouseArea.highlighted || mouseArea.selected
        }

        TextMetrics {
            id: textMetricsLabel
            font.pixelSize: lbl.font.pixelSize
            text: lbl.text
            elide: Qt.ElideNone
        }

        Label {
            id: lbl
            anchors.horizontalCenter: parent.horizontalCenter
            horizontalAlignment: width < textMetricsLabel.width ? Qt.AlignLeft : Qt.AlignHCenter
            font.pixelSize: Theme.fontSizeSmall
            width: parent.width
            truncationMode: TruncationMode.Fade
            color: mouseArea.highlighted || mouseArea.selected ?
                    Theme.highlightColor : Theme.primaryColor
        }
    }
}
