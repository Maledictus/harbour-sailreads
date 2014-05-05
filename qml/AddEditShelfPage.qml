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

Dialog {
    id: dialog

    canAccept: shelfNameTextfield.text !== ""

    property string shelfId
    property alias shelfName: shelfNameTextfield.text
    property alias exclusive: exclusiveSwitch.checked
    property alias sortable: sortableSwitch.checked
    property alias featured: featuredSwitch.checked
    property alias dialogHeaderText: dialogHeader.acceptText

    Column {
        width: parent.width

        DialogHeader {
            id: dialogHeader;
            acceptText: qsTr (shelfId !== "" ? "Edit shelf" : "Create shelf")
        }

        TextField {
            id: shelfNameTextfield
            width: parent.width
            placeholderText: qsTr ("Shelf name")
        }

        TextSwitch {
            id: exclusiveSwitch
            text: "Exclusive"
            description: qsTr ("Book can only be on one of exclusive shelves")
        }

        TextSwitch {
            id: sortableSwitch
            text: "Sortable"
            description: qsTr ("You can enable shelf sorting to customize the order of your shelves")
        }

        TextSwitch {
            id: featuredSwitch
            text: "Featured"
            description: qsTr ("You may choose one shelf to feature on the top of your profile")
        }
    }
}
