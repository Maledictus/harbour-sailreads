/*
The MIT License(MIT)

Copyright (c) 2018-2019 Oleg Linkin <maledictusdemagog@gmail.com>

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
    property bool exclusive: false
    property bool sortable: false
    property alias featured: featutredShelfSwitch.checked
    property alias recommendFor: recommendForShelfSwitch.checked

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

        ComboBox {
            id: typeComboBox

            property var typeCategories: [false, true]

            function update() {
                addEditShelfDialog.exclusive = typeCategories[currentIndex]
            }
            currentIndex: {
                for (var i = 0; i < typeCategories.length; ++i) {
                    if (addEditShelfDialog.exclusive === typeCategories[i]) {
                        return i
                    }
                }
                return 0
            }

            label: qsTr("Type")
            description: {
                if (!addEditShelfDialog.exclusive) {
                    return qsTr("Add as many tags to a book as you like. Try creating a tag like " +
                            "magic-realism, african-writers or female-protagonists.")
                }
                else {
                    return qsTr("Books can only be on one shelf at a time. Want to Read, " +
                            "Currently-Reading and Read are the default shelves.")
                }
            }

            menu: ContextMenu {
                onClosed: typeComboBox.update()
                MenuItem {
                    text: qsTr("Tag")
                }
                MenuItem {
                    text: qsTr("Shelf")
                }
            }
        }

        TextField {
            id: shelfNameField

            width: parent.width

            placeholderText: !addEditShelfDialog.exclusive ?
                    qsTr("Tag name (e.g. modern-heroine)") :
                    qsTr("Shelf name (e.g. abandoned)")
            maximumLength: 35
        }

        TextSwitch {
            id: featutredShelfSwitch

            visible: mode !== "add"
            width: parent.width
            text: qsTr("Featured")
            description: qsTr("You may choose one shelf to feature on the top of your profile")
        }

        TextSwitch {
            id: recommendForShelfSwitch

            width: parent.width
            checked: true
            text: qsTr("Recommend For")
            description: qsTr("If you would like GoodReads to try to make recommendations for books in this shelf")
        }
    }

    canAccept: shelfNameField.text !== ""
}
