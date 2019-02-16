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

import QtQuick 2.6
import Sailfish.Silica 1.0
import harbour.sailreads 1.0

import "../components"
import "../utils/Utils.js" as Utils

MouseArea {
    id: tagDelegate

    property string name
    property int count
    property bool selected: false
    property var view

    property bool down: pressed && containsMouse
    property bool highlighted: down || selected

    implicitHeight: mainRect.height
    width: mainRect.width

    Rectangle {
        id: mainRect
        color: highlighted ?
                Theme.rgba(Theme.highlightBackgroundColor, Theme.highlightBackgroundOpacity) :
                Theme.rgba(Theme.primaryColor, 0.2)

        property int spacing: Theme.paddingSmall
        property real clipWidth: {
            var maxWidth = view ? view.width : Screen.width
            return maxWidth - height - spacing;
        }

        height: Math.max(Theme.itemSizeExtraSmall, textMetricsNumber.width) + spacing
        width: height + textLabel.width + 2 * spacing
        radius: height / 2

        TextMetrics {
            id: textMetricsLabel
            text: name
            font: textLabel.font
            elideWidth: mainRect.clipWidth
            elide: mainRect.clipWidth > 0 ? Qt.ElideRight : Qt.ElideNone
        }

        TextMetrics {
            id: textMetricsNumber
            text: count
            font: textNumber.font
        }

        Rectangle {
            id: rectCircle
            anchors {
                left: parent.left
                verticalCenter: parent.verticalCenter
            }
            height: parent.height
            width: height
            radius: parent.radius
            color: highlighted ?
                    Theme.rgba(Theme.highlightBackgroundColor, Theme.highlightBackgroundOpacity) :
                    Theme.rgba(Theme.primaryColor, 0.3)

            Label {
                id: textNumber
                anchors.centerIn: parent
                color: highlighted ? Theme.rgba(Theme.highlightColor, 0.6) :
                           Theme.rgba(Theme.primaryColor, 0.6)
                wrapMode: Text.NoWrap
                font.bold: true
                text: count
            }
        }

        Label {
            id: textLabel
            anchors {
                left: rectCircle.right
                leftMargin: mainRect.spacing / 2
                verticalCenter:parent.verticalCenter
            }
            color: highlighted ? Theme.highlightColor : Theme.primaryColor
            wrapMode: Text.NoWrap
            text: textMetricsLabel.elidedText
        }
    }
}
