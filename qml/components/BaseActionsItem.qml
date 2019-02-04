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
import Sailfish.Silica 1.0
import harbour.sailreads 1.0

Item {
    width: parent.width
    height: likeButton.height

    property int likesCount: 0
    property int commentsCount: 0
    property bool isLiked: false
    property alias editButton: editButtonItem
    property bool busy: false

    signal like()
    signal edit()
    signal openInBrowser()

    IconText {
        id: voteIcon
        anchors {
            left: parent.left
            verticalCenter: parent.verticalCenter
        }
        label.text: likesCount
        label.color: Theme.highlightColor
        label.font.pixelSize: Theme.fontSizeExtraSmall
        icon.source: "image://theme/icon-s-like?" + Theme.highlightColor
    }

    IconText {
        id: commentsIcon
        anchors {
            left: voteIcon.right
            leftMargin: Theme.paddingLarge
            verticalCenter: parent.verticalCenter
        }
        label.text: commentsCount
        label.color: Theme.highlightColor
        label.font.pixelSize: Theme.fontSizeExtraSmall
        icon.source: "image://theme/icon-s-chat?" + Theme.highlightColor
    }

    IconButton {
        id: likeButton
        anchors {
            right: editButton.visible ? editButton.left : webButton.left
            bottom: parent.bottom
        }
        enabled: !busy
        icon.source: "image://theme/icon-m-like?" + (isLiked ?
                Theme.highlightColor :
                Theme.primaryColor)
        onClicked: like()
    }

    IconButton {
        id: editButtonItem
        anchors {
            right: webButton.left
            bottom: parent.bottom
        }
        icon.source: "image://theme/icon-m-edit"
        onClicked: edit()
    }

    IconButton {
        id: webButton
        anchors {
            right: parent.right
            bottom: parent.bottom
        }
        icon.source: "image://theme/icon-m-link"
        onClicked: openInBrowser()
    }
}

