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
import "../utils/Utils.js" as Utils

Page {
    id: groupMembersPage

    property alias groupId: groupMembersModel.groupId
    property bool busy: sailreadsManager.busy && groupMembersPage.status === PageStatus.Active

    Component.onDestruction: {
        groupMembersModel.cancelRequest()
    }

    GroupMembersModel {
        id: groupMembersModel
    }

    SilicaGridView {
        id: groupMembersView
        anchors.fill: parent
        header: PageHeader {
            title: qsTr("Members")
        }

        PullDownMenu {
            busy: groupMembersPage.busy
            MenuItem {
                text: qsTr("Refresh")
                onClicked: groupMembersModel.loadGroupMembers()
            }
        }

        cellHeight: Theme.itemSizeMedium
        cellWidth: Math.floor(groupMembersPage.width / 2)
        cacheBuffer: groupMembersPage.height

        model: groupMembersModel

        function fetchMoreIfNeeded() {
            if (!groupMembersPage.busy &&
                    groupMembersModel.hasMore &&
                    indexAt(contentX, contentY + height) > groupMembersModel.rowCount() - 12) {
                groupMembersModel.fetchMoreContent()
            }
        }

        onContentYChanged: fetchMoreIfNeeded()

        delegate: ListItem {
            id: listItem
            width: groupMembersView.cellWidth
            height: groupMembersView.cellHeight
            Column {
                id: column

                anchors {
                    right: parent.right
                    rightMargin: Theme.horizontalPageMargin
                    left: parent.left
                    leftMargin: Theme.horizontalPageMargin
                }

                Label {
                    id: groupMemberNameLabel
                    width: parent.width
                    font.family: Theme.fontFamilyHeading
                    truncationMode: TruncationMode.Fade
                    text: groupMemberFirstName
                    color: listItem.highlighted ? Theme.highlightColor: Theme.primaryColor
                }

                KeyValueLabel {
                    font.pixelSize: Theme.fontSizeSmall
                    width: parent.width
                    visible: value !== ""
                    key: qsTr("Comments")
                    value: groupMemberCommentsCount
                }
            }

            onClicked: {
                pageStack.push(Qt.resolvedUrl("ProfilePage.qml"),
                        { userId: groupMemberId })
            }
        }

        VerticalScrollDecorator{}
    }

    BusyIndicator {
        size: BusyIndicatorSize.Large
        anchors.centerIn: parent
        running: groupMembersPage.busy
        visible: running
    }
}
