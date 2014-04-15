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

    property bool loading;
    property string outputType: "updates"

    signal switchToMyProfile ();
    signal refreshUpdates ()

    SilicaListView {
        id: listView
        model: updatesModel
        anchors.fill: parent
        header: PageHeader {
            title: outputType === "updates" ? qsTr ("Recent updates") : ""
        }

        spacing: 10;

        PullDownMenu {
            MenuItem {
                text: qsTr ("My Profile")
                onClicked: switchToMyProfile ();
            }

            MenuItem {
                text: outputType === "updates" ? qsTr ("Refresh") : qsTr ("Recent updates")
                onClicked: refreshUpdates ();
            }
        }

        delegate: BackgroundItem {
            id: delegate
            height: contentItem.childrenRect.height

            Label {
                id: actorLabel
                anchors.left: parent.left
                anchors.top: parent.top
                anchors.leftMargin: Theme.paddingMedium
                anchors.rightMargin: Theme.paddingMedium

                text: updateActorName
                color: delegate.highlighted ? Theme.highlightColor : Theme.primaryColor
            }

            Label {
                id: actionTextLabel
                anchors.left: actorLabel.right
                anchors.top: parent.top
                anchors.right: parent.right
                anchors.leftMargin: Theme.paddingMedium
                anchors.rightMargin: Theme.paddingMedium
                textFormat: Text.RichText
                text: updateActionText
                color: delegate.highlighted ? Theme.highlightColor : Theme.primaryColor
            }

        }

        VerticalScrollDecorator {}
    }

    BusyIndicator {
        visible: loading
        running: visible;
        anchors.centerIn: parent;
    }
}
