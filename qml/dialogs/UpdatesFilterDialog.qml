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

import QtQuick 2.6
import Sailfish.Silica 1.0
import harbour.sailreads 1.0

Dialog {
    id: updatesFilterDialog

    property string items: "all"
    property string scope: "top_friends"

    Column {
        anchors.fill: parent

        DialogHeader {
            acceptText: qsTr("Filter")
            cancelText: qsTr("Cancel")
        }

        ComboBox {
            id: itemsComboBox

            property var itemsCategories: ["all", "books", "reviews"]

            function update(index) {
                items = itemsCategories[index]
            }
            currentIndex: {
                for (var i = 0; i < itemsCategories.length; ++i) {
                    if (items === itemsCategories[i]) {
                        return i
                    }
                }
                console.log("Unsupported items selected")
                return 0
            }
            label: qsTr("Content")
            description: qsTr("What types of updates you see")
            menu: ContextMenu {
                onActivated: itemsComboBox.update(index)
                MenuItem {
                    text: qsTr("Everything")
                }
                MenuItem {
                    text: qsTr("Book updates only")
                }
                MenuItem {
                    text: qsTr("Reviews only")
                }
            }
        }

        ComboBox {
            id: scopeComboBox

            property var scopeFields: [ "friends", "top_friends_following", "top_friends" ]

            function update(index) {
                scope = scopeFields[index]
            }
            currentIndex: {
                for (var i = 0; i < scopeFields.length; ++i) {
                    if (scope === scopeFields[i]) {
                        return i
                    }
                }
                console.log("Unsupported scope selected")
                return 0
            }
            label: qsTr("People")
            description: qsTr("Whose updates you see")
            menu: ContextMenu {
                onActivated: scopeComboBox.update(index)
                MenuItem {
                    text: qsTr("All friends and people I follow")
                }
                MenuItem {
                    text: qsTr("Top friends and people I follow")
                }
                MenuItem {
                    text: qsTr("Top friends only")
                }
            }
        }
    }
}
