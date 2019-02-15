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
    width: childrenRect.width

    Rectangle {
        id: mainRect
        width: Math.min(row.width, view.width)
        height: Theme.itemSizeExtraSmall

        color: highlighted ? Theme.rgba(Theme.highlightBackgroundColor, Theme.highlightBackgroundOpacity)
                          : Theme.rgba(Theme.primaryColor, 0.2)
        radius: height / 2

        Row {
            id: row
            height: parent.height
            width: countRect.width + nameLabel.implicitWidth + 2 * Theme.paddingMedium
            spacing: Theme.paddingSmall
            Rectangle {
                id: countRect
                width: fakeLabel.width + 2 * Theme.paddingMedium
                height: parent.height
                color: highlighted ? Theme.rgba(Theme.highlightBackgroundColor, Theme.highlightBackgroundOpacity)
                                   : Theme.rgba(Theme.primaryColor, 0.3)
                radius: height / 2

                Label {
                    id: fakeLabel
                    visible: false
                    anchors.left: parent.left
                    anchors.leftMargin: Theme.paddingMedium
                    anchors.verticalCenter: parent.verticalCenter
                    text: "999"
                }

                Label {
                    anchors {
                        left: parent.left
                        leftMargin: Theme.paddingSmall
                        right: parent.right
                        rightMargin: Theme.paddingSmall
                        verticalCenter: parent.verticalCenter
                    }
                    horizontalAlignment: Text.AlignHCenter
                    text: count
                    truncationMode: TruncationMode.Fade
                    wrapMode: Text.NoWrap
                    font.bold: true
                    color: highlighted ? Theme.rgba(Theme.highlightColor, 0.6) :
                               Theme.rgba(Theme.primaryColor, 0.6)
                }
            }

            Label {
                id: nameLabel
                text: name
                width: mainRect.width - countRect.width
                wrapMode: Text.NoWrap
                truncationMode: TruncationMode.Fade
                color: highlighted ? Theme.highlightColor : Theme.primaryColor
                anchors.verticalCenter: parent.verticalCenter
            }
        }
    }
}
