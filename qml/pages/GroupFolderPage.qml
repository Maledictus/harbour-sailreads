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
import QtQml.Models 2.2
import Sailfish.Silica 1.0
import harbour.sailreads 1.0

import "../components"
import "../utils/Utils.js" as Utils

Page {
    id: folderPage

    property int groupId
    property string folderId
    property string folderName
    property bool busy: sailreadsManager.busy && folderPage.status === PageStatus.Active

    Component.onDestruction: {
        topicsModel.cancelRequest()
    }

    GroupFolderTopicsModel {
        id: topicsModel
        groupId: folderPage.groupId
        groupFolderId: folderPage.folderId
    }

    SilicaListView {
        id: topicsView

        anchors.fill: parent

        cacheBuffer: folderPage.height

        model: topicsModel

        PullDownMenu {
            busy: folderPage.busy
            MenuItem {
                text: qsTr("Add new topic")
                onClicked: {
                    var dialog = pageStack.push(Qt.resolvedUrl("../dialogs/AddGroupFolderTopicDialog.qml"))
                    dialog.accepted.connect (function () {
                        sailreadsManager.addNewTopic(dialog.topic, 'Group', folderPage.groupId,
                                folderPage.folderId, dialog.question, dialog.updateFeed,
                                dialog.digest, dialog.topicComment)
                    })
                }
            }
            MenuItem {
                text: qsTr("Refresh")
                onClicked: topicsModel.loadGroupFolderTopics()
            }
        }

        ViewPlaceholder {
            enabled: !sailreadsManager.busy && topicsView.count === 0
            text: qsTr("There are no topics")
            hintText: qsTr("Pull down to refresh")
        }

        function fetchMoreIfNeeded() {
            if (!folderPage.busy &&
                    topicsModel.hasMore &&
                    indexAt(contentX, contentY + height) > topicsModel.rowCount() - 2) {
                topicsModel.fetchMoreContent()
            }
        }

        onContentYChanged: fetchMoreIfNeeded()

        header: PageHeader {
            title: folderPage.folderName
        }

        delegate: ListItem {
            width: topicsView.width
            contentHeight: column.height + separator.height + Theme.paddingMedium
            clip: true
            Column {
                id: column
                anchors {
                    left: parent.left
                    leftMargin: Theme.horizontalPageMargin
                    right: topicCommentsCountLabel.left
                    rightMargin: Theme.paddingMedium
                }
                Label {
                    id: topicNameLabel
                    width: parent.width
                    truncationMode: TruncationMode.Fade
                    text: topicTitle
                    wrapMode: Text.WordWrap
                    maximumLineCount: 2
                    font.bold: topicUnreadCount > 0
                }
                KeyValueLabel {
                    font.pixelSize: Theme.fontSizeSmall
                    truncationMode: TruncationMode.Fade
                    width: parent.width
                    visible: value !== ""
                    key: qsTr("Author")
                    value: topicAuthorName
                    font.bold: topicUnreadCount > 0
                }
                KeyValueLabel {
                    font.pixelSize: Theme.fontSizeSmall
                    truncationMode: TruncationMode.Fade
                    width: parent.width
                    visible: value !== ""
                    key: qsTr("Last update")
                    value: Utils.generateDateString(topicLastCommentDate, "dd.MM.yy hh:mm")
                    font.bold: topicUnreadCount > 0
                }
            }

            Label {
                id: topicCommentsCountLabel
                anchors {
                    right: parent.right
                    rightMargin: Theme.horizontalPageMargin
                    verticalCenter: parent.verticalCenter
                }
                color: Theme.secondaryColor
                text: topicCommentsCount
                font.bold: topicUnreadCount > 0
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

            onClicked: {
                pageStack.push(Qt.resolvedUrl("../pages/GroupFolderTopicPage.qml"),
                        { topicId: topicId })
            }
        }

        VerticalScrollDecorator {}
    }

    BusyIndicator {
        size: BusyIndicatorSize.Large
        anchors.centerIn: parent
        running: folderPage.busy
        visible: running
    }
}
