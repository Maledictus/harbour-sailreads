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

    property bool self: true

    property bool loading

    property alias profilePhotoUrl: userPhoto.source
    property alias userName: userNameField.title
    property alias userDetails: userDetails.text
    property alias userInterests: userInterests.text
    property alias notificationsCount: notificationsCountLabel.text
    property alias friendsCount: friendsCountLabel.text
    property alias groupsCount: groupsCountLabel.text
    property alias reviewsCount: reviewsCountLabel.text

    signal switchToMyProfile ()
    signal switchToRecentUpdates ()
    signal switchToNotifications ()
    signal switchToFriends ()
    signal switchToGroups ()
    signal switchToReviews ()

    onSelfChanged: { // details should be empty while loading
        userPhoto.source = ""
        userNameField.title = ""
        userLocation.text = ""
        friendsCountLabel.text = ""
        groupsCountLabel.text = ""
        reviewsCountLabel.text = ""
    }

    BusyIndicator {
       anchors.centerIn: parent
       visible: loading && (userNameField.title === "")
       running: visible
    }

    SilicaFlickable {
        anchors.fill: parent
        Column {
            width: parent.width

            PullDownMenu {
                MenuItem {
                    text: qsTr ("My Profile")
                    onClicked: switchToMyProfile ()
                    visible: !self
                }
                MenuItem {
                    text: qsTr ("Recent updates")
                    onClicked: switchToRecentUpdates ()
                }
            }

            PageHeader {
                id: userNameField
            }

            Item {
                height: 158
                width: parent.width
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.margins: Theme.paddingMedium

                Label {
                    id: userDetailsLabel
                    anchors.left: userPhoto.right
                    anchors.bottom: userPhoto.verticalCenter
                    anchors.margins: Theme.paddingMedium
                    wrapMode: Text.WordWrap
                    font.bold: false
                    font.pixelSize: Theme.fontSizeTiny
                    color: Theme.secondaryColor
                    text: qsTr ("Details:")
                }

                Label {
                    id: userDetails
                    anchors.right: parent.right
                    anchors.left: userDetailsLabel.right
                    anchors.bottom: userPhoto.verticalCenter
                    anchors.leftMargin: Theme.paddingMedium
                    anchors.bottomMargin: Theme.paddingMedium
                    wrapMode: Text.WordWrap
                    font.bold: false
                    font.pixelSize: Theme.fontSizeTiny
                    color: Theme.secondaryColor
                }

                Label {
                    id: userInterestsLabel
                    anchors.left: userPhoto.right
                    anchors.top: userDetailsLabel.bottom
                    anchors.margins: Theme.paddingMedium
                    wrapMode: Text.WordWrap
                    font.bold: false
                    font.pixelSize: Theme.fontSizeTiny
                    color: Theme.secondaryColor
                    text: qsTr ("Interests:")
                }

                Label {
                    id: userInterests
                    anchors.right: parent.right
                    anchors.left: userInterestsLabel.right
                    anchors.top: userDetailsLabel.bottom
                    anchors.leftMargin: Theme.paddingMedium
                    anchors.rightMargin: Theme.paddingMedium
                    anchors.topMargin: Theme.paddingMedium
                    wrapMode: Text.WordWrap
                    font.bold: false
                    font.pixelSize: Theme.fontSizeTiny
                    color: Theme.secondaryColor
                }

                Image {
                    anchors.fill: userPhoto
                    source: "./images/blank_boy.png"
                    visible: (userPhoto.status !== Image.Ready)
                }

                Image {
                    id: userPhoto
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.left: parent.left
                    width: 128
                    height: 128
                }

            }

            BackgroundItem {
                visible: self
                width: parent.width
                Label {
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.left: parent.left
                    anchors.right: parent.right
                    anchors.margins: Theme.paddingMedium
                    text: qsTr ("Notifications")
                    color: parent.highlighted ? Theme.highlightColor : Theme.primaryColor
                }

                Label {
                    id: notificationsCountLabel
                    anchors.right: parent.right
                    anchors.verticalCenter: parent.verticalCenter
                    font.pixelSize: Theme.fontSizeMedium
                    color: parent.highlighted ? Theme.secondaryHighlightColor : Theme.secondaryColor
                    anchors.margins: Theme.paddingMedium

                }

                onClicked: switchToNotifications ()
            }


            BackgroundItem {
                width: parent.width
                Label {
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.left: parent.left
                    anchors.right: parent.right
                    anchors.margins: Theme.paddingMedium
                    text: qsTr ("Friends")
                    color: parent.highlighted ? Theme.highlightColor : Theme.primaryColor
                }

                Label {
                    id: friendsCountLabel
                    anchors.right: parent.right
                    anchors.verticalCenter: parent.verticalCenter
                    font.pixelSize: Theme.fontSizeMedium
                    color: parent.highlighted ? Theme.secondaryHighlightColor : Theme.secondaryColor
                    anchors.margins: Theme.paddingMedium

                }

                onClicked: switchToFriends ()
            }

            BackgroundItem {
                width: parent.width
                Label {
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.left: parent.left
                    anchors.right: parent.right
                    anchors.margins: Theme.paddingMedium
                    text: qsTr ("Groups")
                    color: parent.highlighted ? Theme.highlightColor : Theme.primaryColor
                }

                Label {
                    id: groupsCountLabel
                    anchors.right: parent.right
                    anchors.verticalCenter: parent.verticalCenter
                    font.pixelSize: Theme.fontSizeMedium
                    color: parent.highlighted ? Theme.secondaryHighlightColor : Theme.secondaryColor
                    anchors.margins: Theme.paddingMedium

                }

                onClicked: switchToGrups ()
            }

            BackgroundItem {
                width: parent.width
                Label {
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.left: parent.left
                    anchors.right: parent.right
                    anchors.margins: Theme.paddingMedium
                    text: qsTr ("Reviews")
                    color: parent.highlighted ? Theme.highlightColor : Theme.primaryColor
                }

                Label {
                    id: reviewsCountLabel
                    anchors.right: parent.right
                    anchors.verticalCenter: parent.verticalCenter
                    font.pixelSize: Theme.fontSizeMedium
                    color: parent.highlighted ? Theme.secondaryHighlightColor : Theme.secondaryColor
                    anchors.margins: Theme.paddingMedium

                }

                onClicked: switchToReviews ()
            }
        }
    }
}
