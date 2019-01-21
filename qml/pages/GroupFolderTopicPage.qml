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
import "../utils/Utils.js" as Utils

Page {
    id: groupFodlerTopicPage

    property alias topicId: topicItem.topicId
    property bool busy: sailreadsManager.busy && groupFodlerTopicPage.status === PageStatus.Active

    function attachPage() {
        if (pageStack._currentContainer.attachedContainer === null
                && sailreadsManager.logged) {
            //pageStack.pushAttached(Qt.resolvedUrl("StatusPage.qml"))
        }
    }

    Component.onDestruction: {
        topicItem.cancelRequest()
        topicThreadModel.cancelRequest()
    }

    TopicItem {
        id: topicItem
    }

    CommentsModel {
        id: topicThreadModel
        resourceType: "topic"
        resourceId: topicItem.topicId
    }

    SilicaFlickable {
        id: flickable
        anchors.fill: parent
        pressDelay: 0
        contentHeight: height

        PageHeader {
            id: header
            clip: true
            title: topicItem.topic ? topicItem.topic.title : ""
        }

        PullDownMenu {
            busy: groupFodlerTopicPage.busy
            MenuItem {
                text: qsTr("Refresh")
                onClicked: {
                    topicThreadModel.loadComments()
                }
            }
        }

        SilicaListView {
            id: topicThreadView
            anchors {
                top: header.bottom
                bottom: parent.bottom
            }
            width: parent.width
            clip: true
            cacheBuffer: groupFodlerTopicPage.height * 2
            pressDelay: 0
            verticalLayoutDirection: ListView.BottomToTop
            model: topicThreadModel

            header: NewMessageItem {
                anchors {
                    left: parent.left
                    right: parent.right
                }
                onSendMessage: {
                    sailreadsManager.addNewComment("topic", groupFodlerTopicPage.topicId, message)
                }
            }

            function fetchMoreIfNeeded() {
                if (!groupFodlerTopicPage.busy &&
                        topicThreadModel.hasMore &&
                        indexAt(contentX, contentY) > topicThreadModel.rowCount() - 2) {
                    topicThreadModel.fetchMoreContent()
                }
            }

            onContentYChanged: fetchMoreIfNeeded()

            delegate: CommentListItem {
                width: parent.width
                author: commentAuthor
                body: commentBody
                updateDate: commentUpdateDate
                onUserClicked: pageStack.push(Qt.resolvedUrl("ProfilePage.qml"), { userId: userId })
                onLinkActivated: mainWindow.openPageFromUrl(link)
            }
            VerticalScrollDecorator{}
        }
    }

    BusyIndicator {
        size: BusyIndicatorSize.Large
        anchors.centerIn: parent
        running: groupFodlerTopicPage.busy
        visible: running
    }

}
