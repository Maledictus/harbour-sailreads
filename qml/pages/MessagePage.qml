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
import "../components"

Page {
    id: messagePage

    property bool busy: sailreadsManager.busy && messagePage.status === PageStatus.Active
    property alias messageId: messageItem.messageId
    property alias message: messageItem.message

    function attachPage() {
        if (pageStack._currentContainer.attachedContainer === null
                && sailreadsManager.logged) {
            //pageStack.pushAttached(Qt.resolvedUrl("StatusPage.qml"))
        }
    }

    Component.onDestruction: {
        messageItem.cancelRequest()
    }

    MessageItem {
        id: messageItem
    }

    SilicaListView {
        id: messageHistoryView
        anchors.fill: parent
        cacheBuffer: messagePage.height

        PullDownMenu {
            busy: messagePage.busy
            MenuItem {
                text: qsTr("Refresh")
                onClicked: {
                    messageItem.loadMessage()
                }
            }
        }

        header: Item {
            width: messageHistoryView.width
            height: messageBox.height
        }

        contentY: -headerItem.height
        model: message ? message.messageHistory : []

        delegate: MessageListItem {
            posterImage: modelData.fromUser.avatarUrl
            posterName: modelData.fromUser.userName
            postDate: modelData.createDate
            posterId: modelData.fromUser.id
            subject: modelData.subject
            isRead: modelData.isRead

            onClicked: pageStack.push(Qt.resolvedUrl("MessagePage.qml"),
                    { message: modelData, messageId: modelData.id })
        }

        VerticalScrollDecorator {}
    }

    Column {
        id: messageBox
        parent: messageHistoryView.headerItem ? messageHistoryView.headerItem : messagePage
        width: parent.width

        spacing: Theme.paddingMedium

        PageHeader {
            title: message ? message.subject : qsTr("Message")
        }

        Column {
            anchors {
                left: parent.left
                leftMargin: Theme.horizontalPageMargin
                right: parent.right
                rightMargin: Theme.horizontalPageMargin
            }

            PosterHeaderItem {
                width: parent.width
                height: Theme.itemSizeMedium
                posterAvatar: message ? message.fromUser.avatarUrl : ""
                posterName: message ?  message.fromUser.userName : ""
                postDate: message ? Qt.formatDateTime(message.createDate) : ""
                onClicked: {
                    if (!message) {
                        return
                    }

                    pageStack.push(Qt.resolvedUrl("ProfilePage.qml"),
                        { userId: message.fromUser.id })
                }
            }

            Label {
                text: message ? message.body : ""
                width: parent.width
                wrapMode: Text.WordWrap
                textFormat: Text.StyledText
                linkColor: Theme.highlightColor
            }

            SectionHeader {
                text: qsTr("Message History")
            }
        }
    }

    BusyIndicator {
        size: BusyIndicatorSize.Large
        anchors.centerIn: parent
        running: sailreadsManager.busy
        visible: running
    }
}
