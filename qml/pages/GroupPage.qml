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
    id: groupPage

    property alias groupId: groupItem.groupId
    property string groupName
    property bool busy: sailreadsManager.busy && groupPage.status === PageStatus.Active

    Component.onDestruction: {
        groupItem.cancelRequest()
    }

    GroupItem {
        id: groupItem
        onGroupChanged: {
            membersButton.counter = group.usersCount
        }
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
                busy: groupPage.busy
                MenuItem {
                    text: qsTr("Add new topic")
                    visible: groupItem.group && groupItem.group.isMember
                    onClicked: {
                        var dialog = pageStack.push("../dialogs/AddGroupFolderTopicDialog.qml")
                        dialog.accepted.connect (function () {
                            sailreadsManager.addNewTopic(dialog.topic, 'Group', groupPage.groupId,
                                    "", dialog.question, dialog.updateFeed,
                                    dialog.digest, dialog.topicComment)
                        })
                    }
                }

                MenuItem {
                    text: qsTr("Join a group")
                    visible: groupItem.group && groupItem.group.groupAccess != Group.Secret ?
                            !groupItem.group.isMember : false
                    onClicked: {
                        if (!groupItem.group) {
                            return
                        }

                        if (groupItem.group.groupAccess == Group.Public) {
                            sailreadsManager.joinGroup(groupId)
                        }
                        else {
                            mainWindow.openInBrowser(groupItem.group.url)
                        }
                    }
                }

                MenuItem {
                    text: qsTr("Refresh")
                    onClicked: {
                        groupItem.loadGroup()
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
                    defaultImage: "qrc:/images/grga.jpg"
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

                function getGroupTypeString(group) {
                    var result = ""
                    if (!group) {
                        return result
                    }

                    switch (group.groupAccess) {
                    case Group.Restricted:
                        result = qsTr("Restricted. Members can join if they have " +
                                "an email address at one of specified domains")
                        break
                    case Group.Private:
                        result = qsTr("Private. Members must be invited or approved " +
                                "by the group's moderator")
                        break
                    case Group.Secret:
                        result = qsTr("Secret")
                        break
                    case Group.Public:
                    default:
                        result = qsTr("Public. Anyone can join and invite others to join")
                        break;
                    }
                    return result
                }

                KeyValueLabel {
                    visible: value !== ""
                    font.pixelSize: Theme.fontSizeSmall
                    key: qsTr("Group Type")
                    value: groupItem.group ? parent.getGroupTypeString(groupItem.group) : ""
                }
                KeyValueLabel {
                    visible: value !== ""
                    font.pixelSize: Theme.fontSizeSmall
                    key: qsTr("Members")
                    value: "%L1".arg(groupItem.group ? groupItem.group.usersCount : 0)
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
                onLinkActivated: mainWindow.openPageFromUrl(link)
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
                onLinkActivated: mainWindow.openPageFromUrl(link)
            }

            SectionHeader {
                text: qsTr("Discussion Board")
                visible: groupItem.group && groupItem.group.foldersCount > 0
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
