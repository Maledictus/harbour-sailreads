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
    property string uid

    signal switchToMyProfile ()
    signal switchToAddEditShelf (string shelfId, string name, bool exclusive)
    signal refreshShelves (string uid)

    SilicaListView {
        id: listView
        model: shelvesModel
        anchors.fill: parent
        spacing: Theme.paddingMedium
        header: PageHeader {
            title: qsTr("Bookshelves")
        }

        property Item contextMenu

        PullDownMenu {
            MenuItem {
                text: qsTr ("My Profile")
                visible: uid != "self"
                onClicked: switchToMyProfile ()
            }

            MenuItem {
                text: qsTr ("Add bookshelf")
                visible: true
                onClicked: switchToAddEditShelf ("", "", false);
            }

            MenuItem {
                text: qsTr ("Refresh")
                onClicked: refreshShelves (uid);
            }
        }


        ViewPlaceholder {
            enabled: !loading && (shelvesModel.count === 0)
            text: qsTr ("List of shelves is empty")
        }

        delegate: BackgroundItem {
            id: delegate

            property bool menuOpen: listView.contextMenu != null &&
                    listView.contextMenu.parent === delegate

            height: menuOpen ?
                    listView.contextMenu.height + contentItem.childrenRect.height :
                    contentItem.childrenRect.height

            Label {
                id: shelfNameLabel
                anchors.left: parent.left
                anchors.leftMargin: Theme.paddingMedium
                text: shelfName === undefined ? "" : shelfName
                font.pixelSize: Theme.fontSizeLarge
                color: delegate.highlighted ? Theme.highlightColor : Theme.primaryColor
            }

            Label {
                id: booksCountLabel
                anchors.right: parent.right
                anchors.rightMargin: Theme.paddingMedium
                text: shelfBooksCount === undefined ? "0" : shelfBooksCount
                font.pixelSize: Theme.fontSizeLarge
                color: delegate.highlighted ? Theme.highlightColor : Theme.primaryColor
            }

            onClicked: console.log (shelfID)

            onPressAndHold: {
                if (!listView.contextMenu) {
                    listView.contextMenu = contextMenuComponent.createObject (listView)
                }
                listView.contextMenu.shelfId = shelfID;
                listView.contextMenu.shelfName = shelfName;
                listView.contextMenu.shelfExclusive = shelfExclusive;
                listView.contextMenu.show (delegate)
            }
        }

        Component {
            id: contextMenuComponent
            ContextMenu {
                property string shelfId;
                property string shelfName;
                property bool shelfExclusive;
                property bool shelfSortable;
                property bool shelfFeatured;
                MenuItem {
                    visible: uid == "self"
                    text: qsTr ("Edit");
                    onClicked: switchToAddEditShelf (shelfId, shelfName, shelfExclusive)
                }
            }
        }

        VerticalScrollDecorator {}
    }

    BusyIndicator {
        anchors.centerIn: parent;
        visible: loading && (shelvesModel.count === 0)
        running: visible;
    }
}
