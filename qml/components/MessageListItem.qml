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

ListItem {
    id: listItem

    property alias posterImage: posterHeader.posterAvatar
    property alias posterName: posterHeader.posterName
    property alias subject: subjectLabel.text
    property bool isRead: false
    property int posterId
    property var postDate

    signal linkActivated(var link)

    contentHeight: column.height + separator.height + Theme.paddingMedium
    clip: true

    Column {
        id: column
        anchors {
            left: parent.left
            leftMargin: Theme.horizontalPageMargin
            right: parent.right
            rightMargin: Theme.horizontalPageMargin
        }
        PosterHeaderItem {
            id: posterHeader
            width: parent.width
            postDate: Qt.formatDateTime(postDate)
            onClicked: pageStack.push(Qt.resolvedUrl("ProfilePage.qml"), { userId: posterId })
            highlighted: listItem.highlighted || down
        }
        Label {
            id: subjectLabel
            width: parent.width
            wrapMode: Text.WordWrap
            font.bold: !isRead
            textFormat: Text.StyledText
            linkColor: Theme.highlightColor
            color: highlighted ? Theme.highlightColor : Theme.primaryColor
            onLinkActivated: listItem.linkActivated(link)
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
