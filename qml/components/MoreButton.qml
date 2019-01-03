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

import QtQuick 2.0
import Sailfish.Silica 1.0

BackgroundItem {
    id: morePanel

    property alias text: moreLabel.text
    property alias counter: moreLabelCount.text
    property int horizontalMargin: Theme.horizontalPageMargin
    property alias busy: busyIndicator.running

    height: Theme.itemSizeSmall

    Label {
        id: moreLabelCount
        anchors {
            right: moreLabel.left
            rightMargin: Theme.paddingMedium
            verticalCenter: parent.verticalCenter
        }
        color: Theme.secondaryColor
    }

    Label {
        id: moreLabel
        anchors {
            right: (morePanel.enabled || busy) ? moreImage.left : parent.right
            rightMargin: (morePanel.enabled || busy) ? Theme.paddingMedium : horizontalMargin
            verticalCenter: parent.verticalCenter
        }
        horizontalAlignment: Text.AlignRight
        truncationMode: TruncationMode.Fade
        color: (morePanel.highlighted || !morePanel.enabled)
               ? Theme.highlightColor
               : Theme.primaryColor
    }

    Image {
        id: moreImage
        anchors {
            right: parent.right
            rightMargin: horizontalMargin
            verticalCenter: parent.verticalCenter
        }
        visible: morePanel.enabled && !busy
        source: "image://theme/icon-m-right?"
                + (morePanel.highlighted ? Theme.highlightColor : Theme.primaryColor)
    }

    BusyIndicator {
        id: busyIndicator
        anchors.centerIn: moreImage
        size: BusyIndicatorSize.Small
    }

    BoxBackground {
        anchors.fill: parent
        z: -1
    }
}

