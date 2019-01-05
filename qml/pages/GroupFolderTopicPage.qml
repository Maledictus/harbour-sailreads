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

    property int groupId: 0
    property int groupFolderId: 0
    property alias topicId: topicItem.topicId
    property bool busy: sailreadsManager.busy && groupFodlerTopicPage.status === PageStatus.Active

    function attachPage() {
        if (pageStack._currentContainer.attachedContainer === null
                && sailreadsManager.logged) {
            //pageStack.pushAttached(Qt.resolvedUrl("StatusPage.qml"))
        }
    }

    TopicItem {
        id: topicItem
    }

    Component.onDestruction: {
        sailreadsManager.abortRequest()
    }

    TopicCommentsModel {
        id: topicThreadModel
        topicId: groupFodlerTopicPage.topicId
    }

    Column {
        id: headerBox
        parent: topicThreadView.headerItem ? topicThreadView.headerItem : groupFodlerTopicPage
        width: parent.width

        PageHeader {
            id: pageHeader
            title: topicItem.topic ? topicItem.topic.title : qsTr("Topic")
        }

        Column {
            anchors {
                left: parent.left
                leftMargin: Theme.horizontalPageMargin
                right: parent.right
                rightMargin: Theme.horizontalPageMargin
            }

            KeyValueLabel {
                key: qsTr("Author")
                value: topicItem.topic ? topicItem.topic.author.userName : ""
                truncationMode: TruncationMode.Fade
                font.pixelSize: Theme.fontSizeSmall
            }
            KeyValueLabel {
                key: qsTr("Group")
                value: topicItem.topic ? topicItem.topic.group.name : ""
                truncationMode: TruncationMode.Fade
            }
            KeyValueLabel {
                key: qsTr("Folder")
                value: topicItem.topic ? topicItem.topic.folder.name : ""
                truncationMode: TruncationMode.Fade
            }
        }
    }

    SilicaListView {
        id: topicThreadView

        header: Item {
            width: topicThreadView.width
            height: headerBox.height
        }

        PullDownMenu {
            MenuItem {
                text: qsTr("Refresh")
                onClicked: {
                    topicItem.updateTopic()
                }
            }
            MenuItem {
                text: qsTr("Add comment")
                onClicked: {
                    var dialog = pageStack.push("../dialogs/AddCommentDialog.qml")
                    dialog.accepted.connect (function () {
                        sailreadsManager.addNewComment("topic", groupFodlerTopicPage.topicId,
                                dialog.comment)
                    })
                }
            }
        }

        anchors.fill: parent
        cacheBuffer: groupFodlerTopicPage.height

        model: topicThreadModel

        function fetchMoreIfNeeded() {
            if (!groupFodlerTopicPage.busy &&
                    topicThreadModel.hasMore &&
                    indexAt(contentX, contentY + height) > topicThreadModel.rowCount() - 2) {
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
        }
        VerticalScrollDecorator{}
    }

    BusyIndicator {
        size: BusyIndicatorSize.Large
        anchors.centerIn: parent
        running: groupFodlerTopicPage.busy
        visible: running
    }

}