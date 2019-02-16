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
import Sailfish.Silica 1.0
import harbour.sailreads 1.0

import "../components"
import "../utils/Utils.js" as Utils

Dialog {
    id: addEditReviewDialog

    property var book
    property alias rating: ratingBox.rating
    property alias reviewText: reviewTextArea.text

    property string mode: "add"

    SilicaFlickable {
        anchors.fill: parent
        clip: true
        contentHeight: column.height + Theme.paddingSmall

        Column {
            id: column
            width: parent.width
            clip: true
            spacing: Theme.paddingMedium

            DialogHeader {
                acceptText: mode === "add" ? qsTr("Add") : qsTr("Edit")
                cancelText: qsTr("Cancel")
            }

            ShortBookRow {
                anchors {
                    left: parent.left
                    leftMargin: Theme.horizontalPageMargin
                    right: parent.right
                    rightMargin: Theme.horizontalPageMargin
                }

                width: parent.width
                bookImage: book ? book.imageUrl : ""
                bookTitle: book ? book.title : ""
                bookAuthors: book ?
                        Utils.getAuthorsString(book.authors, Theme.primaryColor) : ""
                bookAverageRating: book ? book.averageRating : 0.0
                bookRatingsCount: book ? book.ratingsCount : 0
            }

            Separator {
                id: separator
                width: parent.width
                color: Theme.primaryColor
                horizontalAlignment: Qt.AlignHCenter
            }

            Column {
                width: parent.width
                Label {
                    anchors.horizontalCenter: parent.horizontalCenter
                    color: Theme.highlightColor
                    font.pixelSize: Theme.fontSizeMedium
                    horizontalAlignment: Text.AlignHCenter
                    text: rating > 0 ? qsTr("My Rating") : qsTr("Rate this book")
                }

                VotingBox {
                    id: ratingBox
                    anchors.horizontalCenter: parent.horizontalCenter
                }
            }

            TextArea {
                id: reviewTextArea
                width: parent.width
                placeholderText: qsTr("Write a review")
                label: qsTr("Review")
            }
        }
    }
}
