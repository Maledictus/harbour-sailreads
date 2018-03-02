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

import QtQuick 2.0
import Sailfish.Silica 1.0
import harbour.sailreads 1.0
import "./components"

Page {
    id: profilePage

    RemorsePopup { id: remorse }

    ListModel {
        id: menusModel
    }

    SilicaListView {
        id: menusView

        anchors.fill: parent

        header: UserHeaderItem {
            id: headerItem

            userId: sailreadsManager.userProfile.userId
            avatarSource: sailreadsManager.userProfile.avatarUrl
            fullName: sailreadsManager.userProfile.fullName
        }

        model: menusModel

        spacing: Theme.paddingMedium

        PullDownMenu {
            visible: sailreadsManager.logged

            MenuItem {
                text: qsTr("Logout")
                onClicked: {
                }
            }
        }

        delegate: ListItem {
            id: rootDelegateItem

            width: menusView.width

            Label {
                anchors.left: parent.left
                anchors.leftMargin: Theme.paddingMedium
                anchors.right: parent.right
                anchors.rightMargin: Theme.paddingMedium
                anchors.verticalCenter: parent.verticalCenter

                text: title
                color: rootDelegateItem.highlighted ? Theme.highlightColor : Theme.primaryColor
            }

            onClicked: {
                pageStack.clear()
                pageStack.push(Qt.resolvedUrl(url), properties)
                pageStack.navigateForward(PageStackAction.Immediate)
                pageStack.navigateBack()
            }
        }

        VerticalScrollDecorator {}
    }

    Component.onCompleted: {
        menusModel.append ({ title: qsTr("Friends Feed"), name: "0",
                url: "FriendsPage.qml" })
        menusModel.append ({ title: qsTr("My Blog"), name: "1",
                url: "UserJournalPage.qml" })
        menusModel.append ({ title: qsTr("Friends"), name: "2",
                url: "FriendsListPage.qml" })
        menusModel.append ({ title: qsTr("Friends groups"), name: "3",
                url: "FriendGroupsPage.qml" })
        menusModel.append ({ title: qsTr ("Messages"), name: "4",
                url: "MessagesPage.qml" })
        menusModel.append ({ title: qsTr ("Notifications"), name: "5",
                url: "NotificationsPage.qml" })
    }
}
