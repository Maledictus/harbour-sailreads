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

import QtQuick 2.6
import Sailfish.Silica 1.0
import harbour.sailreads 1.0
import "../components"

SilicaListView {
    id: messagesView

    property bool busy: false
    property bool active: true

    clip: true
    ViewPlaceholder {
        enabled: !busy && messagesView.count === 0
        text: qsTr("There are no messages in this folder. Pull down to refresh")
    }

    function fetchMoreIfNeeded() {
        if (active && !busy &&
                model.hasMore &&
                indexAt(contentX, contentY + height) > model.rowCount() - 2) {
            model.fetchMoreContent()
        }
    }
    onActiveChanged: fetchMoreIfNeeded()
    onContentYChanged: fetchMoreIfNeeded()

    delegate: MessageListItem {
        posterImage: messageFromUser.avatarUrl
        posterName: messageFromUser.userName
        postDate: messageCreateDate
        posterId: messageFromUser.id
        subject: messageSubject
        isRead: messageRead

        menu: ContextMenu {
            hasContent: !messageRead
            MenuItem {
                text: qsTr("Mark as read")
                onClicked: sailreadsManager.markMessageAsRead(messageId)
            }
        }

        onClicked: pageStack.push(Qt.resolvedUrl("../pages/MessagePage.qml"),
                { message: messageMessage, messageId: messageId })
        onLinkActivated: mainWindow.openPageFromUrl(link)
    }

    VerticalScrollDecorator{}
}
