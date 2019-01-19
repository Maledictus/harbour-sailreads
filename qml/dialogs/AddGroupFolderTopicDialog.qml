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

Dialog {
    id: addGroupFolderTopicDialog

    property alias topic: topicField.text
    property alias question: questionSwitch.checked
    property alias updateFeed: updateFeedSwitch.checked
    property alias digest: digestSwitch.checked
    property alias topicComment: topicCommentArea.text


    SilicaFlickable {
        anchors.fill: parent
        contentHeight: column.height + Theme.paddingSmall
        clip: true

        Column {
            id: column
            width: parent.width
            clip: true
            spacing: Theme.paddingMedium

            DialogHeader {
                acceptText: qsTr("Add")
                cancelText: qsTr("Cancel")
            }

            TextField {
                id: topicField
                width: parent.width
                placeholderText: qsTr("Topic title")
            }

            TextArea {
                id: topicCommentArea
                width: parent.width
                placeholderText: qsTr("Comment")
            }

            TextSwitch {
                id: questionSwitch
                width: parent.width
                text: qsTr("Discussion question")
            }

            TextSwitch {
                id: updateFeedSwitch
                width: parent.width
                text: qsTr("Add to my update feed")
            }

            TextSwitch {
                id: digestSwitch
                width: parent.width
                text: qsTr("Notify me when people reply")
                description: qsTr("You will receive an email notification if someone replies to this thread." +
                        "You will only receive one email notification unit you revisit the thread")
            }

        }
        VerticalScrollDecorator{}
    }

    canAccept: topicField.text !== ""
}
