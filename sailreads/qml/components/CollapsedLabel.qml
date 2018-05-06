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

import QtQuick 2.2
import Sailfish.Silica 1.0
import harbour.sailreads 1.0

MouseArea {
    id: detailsBackground

    signal linkActivated(url link)

    property alias text: contentLabel.text

    property real _ellipsisDisplacement: detailsBox.opened ? 0 : Theme.paddingLarge
    Behavior on _ellipsisDisplacement {
        NumberAnimation { duration: 200; easing.type: Easing.InOutQuad }
    }

    width: parent.width
    height: detailsBox.height + detailsBox.y + Theme.paddingMedium + _ellipsisDisplacement
    property bool highlighted: pressed && containsMouse

    Item {
        id: detailsBox

        property bool opened
        property int expandedHeight: contentLabel.height + Theme.paddingMedium * 2

        y: Theme.paddingMedium
        anchors {
            left: parent.left
            right: parent.right
        }
        clip: true

        height: opened ? expandedHeight : Math.min(contentLabel.height, lblForContentClip.height)

        Behavior on height {
            NumberAnimation {
                duration: detailsBox.expandedHeight > Screen.height ? 400 : 200
                easing.type: Easing.InOutQuad
            }
        }

        Label {
            id: lblForContentClip
            width: parent.width
            font.pixelSize: Theme.fontSizeSmall
            text: "\n"
        }
        Label {
            id: contentLabel
            width: parent.width
            color: detailsBackground.highlighted ? Theme.highlightColor : Theme.primaryColor
            linkColor: Theme.highlightColor
            font.pixelSize: Theme.fontSizeSmall
            textFormat: Text.RichText
            wrapMode: Text.WordWrap
            onLinkActivated: {
                detailsBackground.linkActivated(link)
            }
        }
    }

    Item {
        x: detailsBox.x
        y: detailsBox.y
        OpacityRampEffect {
            sourceItem: detailsBox
            enabled: !detailsBox.opened
            direction: OpacityRamp.TopToBottom
            slope: 1.6
            offset: 4 / 7
            width: detailsBox.width
            height: detailsBox.height
            anchors.fill: null
        }
    }

    Image {
        anchors {
            right: parent.right
            bottom: parent.bottom
        }
        source: "image://theme/icon-lock-more" + (highlighted ? "?" + Theme.highlightColor : "")
    }

    onClicked: {
        detailsBox.opened = !detailsBox.opened
    }
}
