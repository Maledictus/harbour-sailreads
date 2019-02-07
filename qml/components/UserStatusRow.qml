/*
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
import harbour.sailreads 1.0

import "../components"

Row {
    id: userStatusRow

    property alias userAvatar: avatarImageItem.source
    property alias userHeader: userStatusLabel.text
    property alias postBody: bodyLabel.text
    property alias postDate: dateLabel.text

    signal userClicked()
    signal linkActivated(var link)

    width: parent ? parent.width : Screen.width
    spacing: Theme.paddingMedium

    BaseImage {
        id: avatarImageItem
        anchors.top: column.top
        defaultImage: "qrc:/images/gra_small.png"
        indicator.size: BusyIndicatorSize.Medium
        onClicked: userClicked()
    }

    Column {
        id: column
        width: parent.width - avatarImageItem.width - Theme.paddingMedium
        Label {
            id: userStatusLabel
            width: parent.width
            wrapMode: Text.WordWrap
            color: Theme.highlightColor
            font.family: Theme.fontFamilyHeading
            linkColor: Theme.primaryColor
            onLinkActivated: userStatusRow.linkActivated(link)
        }

        Label {
            id: bodyLabel
            width: parent.width
            visible: text !== ""
            wrapMode: Text.WordWrap
            color: Theme.highlightColor
            linkColor: Theme.primaryColor
            onLinkActivated: userStatusRow.linkActivated(link)
        }

        Label {
            id: dateLabel
            visible: text !== ""
            color: Theme.secondaryColor
            anchors.right: parent.right
            font.pixelSize: Theme.fontSizeExtraSmall
        }
    }
}
