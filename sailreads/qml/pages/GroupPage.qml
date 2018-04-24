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
    id: groupPage

    property int groupId: 0
    property string groupName
    property bool busy: sailreadsManager.busy && groupPage.status == PageStatus.Active
    property variant group

    function attachPage() {
        if (pageStack._currentContainer.attachedContainer === null
                && sailreadsManager.logged) {
            pageStack.pushAttached(Qt.resolvedUrl("StatusPage.qml"))
        }
    }

    Component.onCompleted: {
        sailreadsManager.loadGroup(groupId, groupName)
    }

    GroupFoldersModel {
        id: groupFoldersModel
        groupId: groupPage.groupId
    }

    Connections {
        target: sailreadsManager
        onGotUserGroup: {
            if (inGroupId == groupPage.groupId) {
                groupPage.group = inGroup
                groupFoldersModel.handleGotGroup(inGroupId, inGroup)
                membersButton.counter = inGroup.usersCount
            }
        }
    }

    SilicaFlickable {
        id: groupView

        anchors.fill: parent
        contentHeight: column.height

        Column {
            id: column
            anchors.horizontalCenter: parent.horizontalCenter
            width: parent.width

            PageHeader {
                title: group !== undefined ? group.name : ""
            }

            PullDownMenu {
                MenuItem {
                    text: qsTr("Refresh")
                    onClicked: {
                        sailreadsManager.loadGroup(groupId, groupName)
                    }
                }
            }

            Row {
                anchors {
                    left: parent.left
                    leftMargin: Theme.horizontalPageMargin
                    right: parent.right
                    rightMargin: Theme.horizontalPageMargin
                }

                spacing: Theme.paddingMedium

                Image {
                    id: groupImage
                    width: Theme.iconSizeExtraLarge
                    height: Theme.iconSizeExtraLarge
                    fillMode: Image.PreserveAspectFit
                    source: group !== undefined ? group.imageUrl : ""
                    BusyIndicator {
                        size: BusyIndicatorSize.Medium
                        anchors.centerIn: parent
                        running: groupImage.status == Image.Loading
                        visible: running
                    }
                }

                Column {
                    anchors.top: parent.top

                    width: parent.width - Theme.iconSizeExtraLarge - Theme.paddingMedium
                    KeyValueLabel {
                        visible: value !== ""
                        font.pixelSize: Theme.fontSizeSmall
                        key: qsTr("Category")
                        value: group !== undefined ? group.category : ""
                    }
                    KeyValueLabel {
                        visible: value !== ""
                        font.pixelSize: Theme.fontSizeSmall
                        key: qsTr("Subcategory")
                        value: group !== undefined ? group.subCategory : ""
                    }
                    KeyValueLabel {
                        visible: value !== ""
                        font.pixelSize: Theme.fontSizeSmall
                        key: qsTr("Last activity")
                        value: group !== undefined ?
                                Utils.generateDateString(group.lastActivity, "dd MMM yyyy hh:mm") :
                                ""
                    }
                }
            }

            Column {
                anchors {
                    left: parent.left
                    leftMargin: Theme.horizontalPageMargin
                    right: parent.right
                    rightMargin: Theme.horizontalPageMargin
                }
                KeyValueLabel {
                    visible: value !== ""
                    font.pixelSize: Theme.fontSizeSmall
                    key: qsTr("Access")
                    value: group !== undefined ?
                            (group.isPublic ? qsTr("public") : qsTr("private")) : ""
                }
                KeyValueLabel {
                    visible: value !== ""
                    font.pixelSize: Theme.fontSizeSmall
                    key: qsTr("Members")
                    value: group !== undefined ? Number(group.usersCount).toFixed() : 0
                }
            }

            SectionHeader {
                text: qsTr("Description")
                visible: group !== undefined && group.description !== ""
            }

            CollapsedLabel {
                anchors {
                    left: parent.left
                    leftMargin: Theme.horizontalPageMargin
                    right: parent.right
                    rightMargin: Theme.horizontalPageMargin
                }
                visible: group !== undefined && group.description !== ""
                text: group !== undefined ? group.description : ""
            }

            SectionHeader {
                text: qsTr("Rules")
                visible: group !== undefined && group.rules !== ""
            }

            CollapsedLabel {
                anchors {
                    left: parent.left
                    leftMargin: Theme.horizontalPageMargin
                    right: parent.right
                    rightMargin: Theme.horizontalPageMargin
                }
                visible: group !== undefined && group.rules !== ""
                text: group !== undefined ? group.rules : ""
            }

            SectionHeader {
                text: qsTr("Folders")
                visible: group !== undefined && group.rules !== ""
            }

            SilicaListView {
                id: foldersView

                width: parent.width
                height: contentHeight

                model: groupFoldersModel
                delegate: ListItem {
                    width: parent.width
                    Column {
                        width: parent.width
                        anchors {
                            left: parent.left
                            leftMargin: Theme.horizontalPageMargin
                            right: parent.right
                            rightMargin: Theme.horizontalPageMargin
                        }
                        Label {
                            text: groupFolderName.replace("&amp;", "&")
                        }
                        KeyValueLabel {
                            key: qsTr("Topics")
                            value: groupFolderItemsCount
                        }
                    }

                    onClicked: {
                        pageStack.push(Qt.resolvedUrl("GroupFolderPage.qml"),
                                { folderId: groupFolderId, groupId: groupPage.groupId,
                                    folderName: groupFolderName.replace("&amp;", "&") })
                    }
                }
            }

            MoreButton {
                id: membersButton
                width: parent.width
                height: Theme.itemSizeMedium
                text: qsTr("Members")
                busy: groupPage.busy
                onClicked: {
                    pageStack.push(Qt.resolvedUrl("GroupMembersPage.qml"),
                            { groupId: groupPage.groupId })
                }
            }
        }
        VerticalScrollDecorator{}
    }

    BusyIndicator {
        size: BusyIndicatorSize.Large
        anchors.centerIn: parent
        running: groupPage.busy
        visible: running
    }

}
