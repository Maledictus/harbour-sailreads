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
    id: groupPage

    property alias groupId: groupItem.groupId
    property string groupName
    property bool busy: sailreadsManager.busy && groupPage.status === PageStatus.Active

    function attachPage() {
        if (pageStack._currentContainer.attachedContainer === null
                && sailreadsManager.logged) {
            //pageStack.pushAttached(Qt.resolvedUrl("StatusPage.qml"))
        }
    }

    GroupItem {
        id: groupItem
        onGroupChanged: {
            groupFoldersModel.handleGotGroup(groupId, group)
            membersButton.counter = group.usersCount
        }
    }

    Component.onDestruction: {
        sailreadsManager.abortRequest()
    }

    GroupFoldersModel {
        id: groupFoldersModel
        groupId: groupPage.groupId
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
                title: groupItem.group ? groupItem.group.name : ""
            }

            PullDownMenu {
                MenuItem {
                    text: qsTr("Join a group")
                    visible: groupItem.group ? !groupItem.group.isMember : false
                    onClicked: {
                        sailreadsManager.joinGroup(groupId)
                    }
                }

                MenuItem {
                    text: qsTr("Refresh")
                    onClicked: {
                        groupItem.updateGroup()
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

                BaseImage {
                    id: groupImage
                    width: Theme.iconSizeExtraLarge
                    height: Theme.iconSizeExtraLarge
                    fillMode: Image.PreserveAspectFit
                    source: groupItem.group ? groupItem.group.imageUrl : ""
                    indicator.size: BusyIndicatorSize.Medium
                }

                Column {
                    anchors.top: parent.top

                    width: parent.width - Theme.iconSizeExtraLarge - Theme.paddingMedium
                    KeyValueLabel {
                        visible: value !== ""
                        font.pixelSize: Theme.fontSizeSmall
                        key: qsTr("Category")
                        value: groupItem.group ? groupItem.group.category : ""
                    }
                    KeyValueLabel {
                        visible: value !== ""
                        font.pixelSize: Theme.fontSizeSmall
                        key: qsTr("Subcategory")
                        value: groupItem.group ? groupItem.group.subCategory : ""
                    }
                    KeyValueLabel {
                        visible: value !== ""
                        font.pixelSize: Theme.fontSizeSmall
                        key: qsTr("Last activity")
                        value: groupItem.group ?
                                Utils.generateDateString(groupItem.group.lastActivity, "dd MMM yyyy hh:mm") :
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
                    value: groupItem.group ?
                            (groupItem.group.isPublic ? qsTr("public") : qsTr("private")) : ""
                }
                KeyValueLabel {
                    visible: value !== ""
                    font.pixelSize: Theme.fontSizeSmall
                    key: qsTr("Members")
                    value: groupItem.group ? Number(groupItem.group.usersCount)
                                .toLocaleString(Qt.locale(), 'f', 0) : 0
                }
            }

            SectionHeader {
                text: qsTr("Description")
                visible: groupItem.group && groupItem.group.description !== ""
            }

            property string _style: "<style>" +
                    "a:link { color:" + Theme.highlightColor + "; }" +
                    "p { color:" + Theme.primaryColor + "; }" +
                    "</style>"

            CollapsedLabel {
                anchors {
                    left: parent.left
                    leftMargin: Theme.horizontalPageMargin
                    right: parent.right
                    rightMargin: Theme.horizontalPageMargin
                }
                visible: groupItem.group && groupItem.group.description !== ""
                text: groupItem.group ? (column._style + groupItem.group.description) : ""
                onLinkActivated: {
                    Qt.openUrlExternally(link)
                }
            }

            SectionHeader {
                text: qsTr("Rules")
                visible: groupItem.group && groupItem.group.rules !== ""
            }

            CollapsedLabel {
                anchors {
                    left: parent.left
                    leftMargin: Theme.horizontalPageMargin
                    right: parent.right
                    rightMargin: Theme.horizontalPageMargin
                }
                visible: groupItem.group && groupItem.group.rules !== ""
                text: groupItem.group ? (column._style + groupItem.group.rules) : ""
                onLinkActivated: {
                    Qt.openUrlExternally(link)
                }
            }

            SectionHeader {
                text: qsTr("Folders")
                visible: groupItem.group && groupItem.group.rules !== ""
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
