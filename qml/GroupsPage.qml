/*
Copyright (c) 2014 Oleg Linkin <maledictusdemagog@gmail.com>

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

import QtQuick 2.0
import Sailfish.Silica 1.0

Page {
    id: page

    property bool loading: manager.requestInProcess
    property string uid

    signal switchToMyProfile ()
    signal refreshGroups (string uid)

    SilicaListView {
        id: listView
        model: groupsModel
        anchors.fill: parent
        spacing: Theme.paddingMedium
        header: PageHeader {
            title: qsTr("Groups")
        }

        PullDownMenu {
            MenuItem {
                text: qsTr ("My Profile")
                visible: uid != "self"
                onClicked: switchToMyProfile ()
            }

            MenuItem {
                text: qsTr ("Refresh")
                onClicked: refreshGroups (uid);
            }
        }

        ViewPlaceholder {
            enabled: !loading && (groupsModel.count === 0)
            text: qsTr ("List of groups is empty")
        }

        delegate: BackgroundItem {
            id: delegate

            height: contentItem.childrenRect.height

            Image {
                id: groupImage
                source: groupImageUrl === undefined ? "" : groupImageUrl
                anchors.left: parent.left;
                width: 86;
                height: 86;
            }

            Label {
                id: groupNameLabel
                anchors.verticalCenter: groupImage.verticalCenter
                anchors.left: groupImage.right
                anchors.right: groupUsersCountLabel.left
                anchors.leftMargin: Theme.paddingMedium;
                anchors.rightMargin: Theme.paddingMedium
                text: groupName === undefined ? "" : groupName
                color: delegate.highlighted ? Theme.highlightColor : Theme.primaryColor
                wrapMode: Text.WordWrap
            }

            Label {
                id: groupUsersCountLabel
                anchors.verticalCenter: groupImage.verticalCenter
                anchors.right: parent.right
                anchors.leftMargin: Theme.paddingMedium;
                anchors.rightMargin: Theme.paddingMedium
                text: groupUsersCount === undefined ? "" : groupUsersCount
                color: delegate.highlighted ? Theme.highlightColor : Theme.primaryColor
            }
        }

        VerticalScrollDecorator {}
    }

    BusyIndicator {
        anchors.centerIn: parent;
        visible: loading && (groupsModel.count === 0)
        running: visible;
    }


}





