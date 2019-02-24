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

import QtQuick 2.6
import Sailfish.Silica 1.0
import harbour.sailreads 1.0

import "../utils/Utils.js" as Utils

ListItem {
    id: listItem

    property alias imageUrl: groupIconImage.source
    property alias title: groupNameLabel.text
    property var lastActivity
    property int usersCount: 0

    contentHeight: Math.max(groupIconImage.height, column.height) + separator.height +
            Theme.paddingMedium
    clip: true

    BaseImage {
        id: groupIconImage
        anchors {
            left: parent.left
            leftMargin: Theme.horizontalPageMargin
            top: column.top
            topMargin: Theme.paddingSmall
        }
        height: Theme.iconSizeLarge
        width: Theme.iconSizeMedium
        defaultImage: "qrc:/images/grga.jpg"
        horizontalAlignment: Image.AlignLeft
        verticalAlignment: Image.AlignTop
        indicator.size: BusyIndicatorSize.Medium
    }

    Column {
        id: column

        anchors {
            right: parent.right
            rightMargin: Theme.horizontalPageMargin
            left: groupIconImage.right
            leftMargin: Theme.paddingMedium
        }

        Label {
            id: groupNameLabel
            width: parent.width
            font.family: Theme.fontFamilyHeading
            truncationMode: TruncationMode.Fade
            color: highlighted ? Theme.highlightColor : Theme.primaryColor
        }
        KeyValueLabel {
            id: groupLastActivityLabel
            key: qsTr("Last activity")
            value: Utils.generateDateString(lastActivity, "dd.MM.yy hh:mm")
        }
        KeyValueLabel {
            id: groupMembersLabel
            key: qsTr("Members")
            value: "%L1".arg(usersCount)
        }
    }

    Separator {
        id: separator
        anchors {
            top: column.bottom
            topMargin: Theme.paddingMedium
        }

        width: parent.width
        color: Theme.primaryColor
        horizontalAlignment: Qt.AlignHCenter
    }
}
