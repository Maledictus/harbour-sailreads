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
    id: booksReviewsSortingDialog

    property int order: Qt.DescendingOrder
    property string field: "date_added"

    Column {
        anchors.fill: parent

        DialogHeader {
            acceptText: qsTr("Sort")
            cancelText: qsTr("Cancel")
        }

        ComboBox {
            id: sortingOrderComboBox

            property var orderCategories: [Qt.AscendingOrder, Qt.DescendingOrder]

            function update(index) {
                order = orderCategories[index]
            }
            currentIndex: {
                for (var i = 0; i < orderCategories.length; ++i) {
                    if (order === orderCategories[i]) {
                        return i
                    }
                }
                console.log("Unsupported sorting order selected")
                return 0
            }
            label: qsTr("Sorting order")
            menu: ContextMenu {
                onActivated: sortingOrderComboBox.update(index)
                MenuItem {
                    text: qsTr("Ascending")
                }
                MenuItem {
                    text: qsTr("Descending")
                }
            }
        }

        ComboBox {
            id: sortingFieldComboBox

            property var sortingFields: [ "title", "author", "avg_rating", "num_ratings", "date_pub",
                "num_pages", "date_added", "date_updated", "rating", "date_started", "date_read",
                "review" ]

            function update(index) {
                field = sortingFields[index]
            }
            currentIndex: {
                for (var i = 0; i < sortingFields.length; ++i) {
                    if (field === sortingFields[i]) {
                        return i
                    }
                }
                console.log("Unsupported sorting field selected")
                return 0
            }
            label: qsTr("Sorting field")
            menu: ContextMenu {
                onActivated: sortingFieldComboBox.update(index)
                MenuItem {
                    text: qsTr("Title")
                }
                MenuItem {
                    text: qsTr("Author")
                }
                MenuItem {
                    text: qsTr("Average rating")
                }
                MenuItem {
                    text: qsTr("Number of ratings")
                }
                MenuItem {
                    text: qsTr("First published date")
                }
                MenuItem {
                    text: qsTr("Pages count")
                }
                MenuItem {
                    text: qsTr("Date added")
                }
                MenuItem {
                    text: qsTr("Date updated")
                }
                MenuItem {
                    text: qsTr("Your rating")
                }
                MenuItem {
                    text: qsTr("Date started")
                }
                MenuItem {
                    text: qsTr("Date finished")
                }
                MenuItem {
                    text: qsTr("Review written")
                }
            }
        }
    }
}
