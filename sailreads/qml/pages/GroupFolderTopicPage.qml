/*
The MIT License (MIT)

Copyright (c) 2018 Oleg Linkin <maledictusdemagog@gmail.com>

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
    property int topicId: 0
    property variant topic
    property bool busy: sailreadsManager.busy && groupFodlerTopicPage.status == PageStatus.Active

    function attachPage() {
        if (pageStack._currentContainer.attachedContainer === null
                && sailreadsManager.logged) {
            pageStack.pushAttached(Qt.resolvedUrl("StatusPage.qml"))
        }
    }

    Component.onDestruction: {
        sailreadsManager.abortRequest()
    }

    Connections {
        target: sailreadsManager
        onGotGroupFolderTopic: {
            if (groupFodlerTopicPage.topicId === inTopic.id) {
                groupFodlerTopicPage.topic = inTopic
            }
        }
    }

    CommentsModel {
        id: topicThreadModel
        topicId: groupFodlerTopicPage.topicId
    }

    Column {
        id: headerBox
        parent: topicThreadView.headerItem ? topicThreadView.headerItem : groupFodlerTopicPage
        width: parent.width

        PageHeader {
            id: pageHeader
            title: topic !== undefined ? topic.title : qsTr("Topic")
        }

        Column {
            anchors {
                left: parent.left
                leftMargin: Theme.horizontalPageMargin
                right: parent.left
                rightMargin: Theme.horizontalPageMargin
            }

            KeyValueLabel {
                key: qsTr("Author")
                value: topic !== undefined ? topic.author.userName : ""
                truncationMode: TruncationMode.Fade
                font.pixelSize: Theme.fontSizeSmall
            }
            KeyValueLabel {
                key: qsTr("Group")
                value: topic !== undefined ? topic.group.name : ""
                truncationMode: TruncationMode.Fade
            }
            KeyValueLabel {
                key: qsTr("Folder")
                value: topic !== undefined ? topic.folder.name : ""
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
                    sailreadsManager.loadGroupFolderTopic(groupFodlerTopicPage.topicId)
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

        delegate: ListItem {
            width: parent.width
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
                    width: parent.width
                    posterName: commentAuthor.userName
                    postDate: Utils.generateDateString(commentUpdateDate, "dd MMM yyyy hh:mm")
                    posterAvatar: commentAuthor.avatarUrl

                    onClicked: {
                        pageStack.push(Qt.resolvedUrl("ProfilePage.qml"),
                                { userId: commentAuthor.id })
                    }
                }

                Label {
                    id: label
                    width: parent.width
                    wrapMode: Text.WordWrap
                    text: commentBody
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
        VerticalScrollDecorator{}
    }

    BusyIndicator {
        size: BusyIndicatorSize.Large
        anchors.centerIn: parent
        running: groupFodlerTopicPage.busy
        visible: running
    }

}
