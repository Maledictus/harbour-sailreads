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
    id: groupsPage

    property int userId: 0
    property bool busy: sailreadsManager.busy && groupsPage.status === PageStatus.Active

    function attachPage() {
        if (pageStack._currentContainer.attachedContainer === null
                && sailreadsManager.logged) {
            //pageStack.pushAttached(Qt.resolvedUrl("StatusPage.qml"))
        }
    }

    Component.onDestruction: {
        sailreadsManager.abortRequest()
    }

    UserGroupsModel {
        id: groupsModel
        userId: groupsPage.userId
    }

    SilicaListView {
        id: groupsView
        anchors.fill: parent
        header: PageHeader {
            title: qsTr("Groups")
        }

        PullDownMenu {
            MenuItem {
                text: qsTr("Search")
                onClicked: {
                    pageStack.push(Qt.resolvedUrl("GroupSearchPage.qml"))
                }
            }
            MenuItem {
                text: qsTr("Refresh")
                onClicked: {
                    sailreadsManager.loadGroups(userId)
                }
            }
        }

        ViewPlaceholder {
            enabled: !sailreadsManager.busy && groupsView.count === 0
            text: qsTr ("There are no groups.\nPull down to refresh or to search groups")
        }

        cacheBuffer: groupsPage.height
        model: groupsModel

        function fetchMoreIfNeeded() {
            if (!groupsPage.busy &&
                    groupsModel.hasMore &&
                    indexAt(contentX, contentY + height) > groupsModel.rowCount() - 2) {
                groupsModel.fetchMoreContent()
            }
        }

        onContentYChanged: fetchMoreIfNeeded()

        delegate: ListItem {
            width: groupsView.width
            contentHeight: Math.max(groupIconImage.height, column.height) + separator.height +
                    Theme.paddingMedium
            clip: true

            BaseImage {
                id: groupIconImage
                anchors {
                    left: parent.left
                    leftMargin: Theme.horizontalPageMargin
                    top: column.top
                    topMargin: Theme.paddingSmall
                }
                source: groupImageUrl
                height: Theme.iconSizeLarge
                width: Theme.iconSizeMedium
                fillMode: Image.PreserveAspectFit
                horizontalAlignment: Image.AlignLeft
                verticalAlignment: Image.AlignTop
                indicator.size: BusyIndicatorSize.Medium
            }

            Column {
                id: column

                anchors {
                    right: parent.right
                    rightMargin: Theme.horizontalPageMargin
                    left: groupIconImage.right
                    leftMargin: Theme.paddingMedium
                }

                Label {
                    id: groupNameLabel
                    width: parent.width
                    font.family: Theme.fontFamilyHeading
                    truncationMode: TruncationMode.Fade
                    text: groupName
                }
                KeyValueLabel {
                    id: groupLastActivityLabel
                    key: qsTr("Last activity")
                    value: Utils.generateDateString(groupLastActivity, "dd MMM yyyy hh:mm")
                }
                KeyValueLabel {
                    id: groupMembersLabel
                    key: qsTr("Members")
                    value: Number(groupUsersCount).toLocaleString(Qt.locale(), 'f', 0)
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

            onClicked: {
                pageStack.push(Qt.resolvedUrl("GroupPage.qml"),
                        { groupId: groupId, groupName: groupName })
            }
        }

        VerticalScrollDecorator {}
    }

    BusyIndicator {
        size: BusyIndicatorSize.Large
        anchors.centerIn: parent
        running: groupsPage.busy
        visible: running
    }
}
