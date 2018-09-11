/*
The MIT License(MIT)

Copyright (c) 2018 Oleg Linkin <maledictusdemagog@gmail.com>

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files(the "Software"), to deal
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
    id: addEditShelfDialog

    property alias name: shelfNameField.text
    property alias exclusive: exclusiveShelfSwitch.checked
    property alias sortable: sortableShelfSwitch.checked
    property alias featured: featutredShelfSwitch.checked

    property string mode: "add"

    Column {
        id: column

        anchors.fill: parent

        clip: true

        spacing: Theme.paddingMedium

        DialogHeader {
            acceptText: mode === "add" ? qsTr("Add") : qsTr("Edit")
            cancelText: qsTr("Cancel")
        }

        TextField {
            id: shelfNameField

            width: parent.width

            placeholderText: qsTr("Shelf name")
            maximumLength: 35
        }

        TextSwitch {
            id: exclusiveShelfSwitch

            width: parent.width
            text: qsTr("Exclusive")
            description: qsTr("A book can only be on one of the exclisive shelves")
        }

        TextSwitch {
            id: sortableShelfSwitch

            visible: mode !== "add"
            width: parent.width
            text: qsTr("Sortable")
            description: qsTr("You can enable shelf sorting to customize the order of your books on shelf")
        }

        TextSwitch {
            id: featutredShelfSwitch

            visible: mode !== "add"
            width: parent.width
            text: qsTr("Featured")
            description: qsTr("You may choose one shelf to feature on the top of your profile.")
        }
    }

    canAccept: shelfNameField.text !== ""
}
