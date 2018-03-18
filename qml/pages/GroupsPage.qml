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

import "../utils/Utils.js" as Utils

Page {
    id: groupsPage

    property int userId: 0
    property bool busy: sailreadsManager.busy && groupsPage.status == PageStatus.Active

    BaseProxyModel {
        id: groupsModel
        sortRole: GroupsModel.LastActivity
        dynamicSortFilter: true
        sourceModel: GroupsModel {
            userId: groupsPage.userId
        }
    }

    SilicaListView {
        id: groupsView
        anchors.fill: parent
        header: PageHeader {
            title: qsTr("Groups")
        }

        PullDownMenu {
            MenuItem {
                text: qsTr("Refresh")
                onClicked: {
                    sailreadsManager.loadGroups(userId)
                }
            }
        }

        ViewPlaceholder {
            enabled: !sailreadsManager.busy && groupsView.count === 0
            text: qsTr ("There are no groups. Pull down to refresh")
        }

        model: groupsModel

        spacing: Theme.paddingSmall

        delegate: ListItem {
            width: groupsView.width
            contentHeight: Theme.itemSizeLarge
            clip: true

            Image {
                id: groupIconImage
                anchors {
                    left: parent.left
                    leftMargin: Theme.horizontalPageMargin
                    verticalCenter: parent.verticalCenter
                }
                source: groupImageUrl
                height: Theme.iconSizeLarge
                width: Theme.iconSizeLarge
                fillMode: Image.PreserveAspectFit
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
                    font.family: Theme.fontFamilyHeading
                    truncationMode: TruncationMode.Fade
                    text: groupName
                }
                Label {
                    id: groupLastActivityLabel
                    truncationMode: TruncationMode.Fade
                    font.pixelSize: Theme.fontSizeExtraSmall
                    text: qsTr("Last activity: %1")
                            .arg(Utils.generateDateString(groupLastActivity, "dd MMM yyyy hh:mm"))
                }
                Label {
                    id: groupMembersLabel
                    truncationMode: TruncationMode.Fade
                    font.pixelSize: Theme.fontSizeExtraSmall
                    text: qsTr("%1 members").arg(groupUsersCount)
                }
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
