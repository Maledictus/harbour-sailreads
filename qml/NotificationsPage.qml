/*
    Copyright (c) 2014 Oleg Linkin <MaledictusDeMagog@gmail.com>

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

    signal refreshNotifications ()

    SilicaListView {
        id: listView
        model: notificationsModel
        anchors.fill: parent
        header: PageHeader {
            title: qsTr ("Notifications")
        }

        spacing: 10;

        PullDownMenu {
            MenuItem {
                text: qsTr ("Refresh")
                onClicked: refreshNotifications ()
            }
        }

        delegate: BackgroundItem {
            id: delegate
            height: textColumn.height

            Image {
                id: userImage

                anchors.left: parent.left
                anchors.leftMargin: Theme.paddingMedium
                anchors.verticalCenter: parent.verticalCenter

                height: sourceSize.height
                width: sourceSize.width
                source: notificationActorProfileImage === undefined ?
                    "" :
                    notificationActorProfileImage
            }

            Column
            {
                id: textColumn

                anchors.left: userImage.right
                anchors.right: parent.right
                anchors.leftMargin: Theme.paddingMedium
                anchors.rightMargin: Theme.paddingMedium

                spacing: Theme.paddingSmall
                Label {
                    id: notificationLabel

                    height: contentHeight

                    anchors.left: parent.left
                    anchors.right: parent.right

                    text: notificationText === undefined ? "" : notificationText
                    font.bold: !notificationRead
                    color: delegate.highlighted ? Theme.highlightColor : Theme.primaryColor
                    font.pixelSize: Theme.fontSizeSmall
                    wrapMode: Text.WordWrap
                }

                Label {
                    id: dateLabel
                    text: Qt.formatDateTime (notificationDate)
                    color: Theme.secondaryColor
                    font.pixelSize: Theme.fontSizeTiny
                }
            }

            onClicked: console.log ("clicked")
        }

        VerticalScrollDecorator {}
    }

    BusyIndicator {
        visible: loading && (notificationsModel.count === 0)
        running: visible;
        anchors.centerIn: parent;
    }
}
