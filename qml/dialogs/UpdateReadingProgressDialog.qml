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

import "../components"
import "../utils/Utils.js" as Utils

Dialog {
    id: updateReadingProgressDialog

    property alias reviewId: reviewItem.reviewId
    property alias review: reviewItem.review
    property string bookId
    property var book
    property var parentPage
    property string progressKey: "page"
    property alias pageValue: setProgressComponent.pageValue
    property alias percentValue: setProgressComponent.percentValue
    property alias comment: commentTextArea.text

    property bool busy: sailreadsManager.busy && updateReadingProgressDialog.status === PageStatus.Active

    ReviewItem {
        id: reviewItem
        onReviewChanged: {
            if (review && review.lastUserStatus) {
                pageValue = review.lastUserStatus.page
                percentValue = review.lastUserStatus.percent
                if (pageValue > 0 && percentValue < 0) {
                    progressKey = "page"
                }
                else if (pageValue < 0 && percentValue > 0) {
                    progressKey = "percent"
                }
                for (var i = 0; i < progressComboBox.progressCategories.length; ++i) {
                    if (progressKey === progressComboBox.progressCategories[i]) {
                        progressComboBox.currentIndex = i;
                        break
                    }
                }
            }
        }
    }

    SilicaFlickable {
        anchors.fill: parent
        contentHeight: column.height + Theme.paddingSmall
        clip: true

        Column {
            id: column
            width: parent.width
            clip: true
            spacing: Theme.paddingMedium

            DialogHeader {
                acceptText: qsTr("Update")
                cancelText: qsTr("Cancel")
            }

            PullDownMenu {
                busy: updateReadingProgressDialog.busy
                MenuItem {
                    text: qsTr("I'm finished!")
                    onClicked: {
                        pageStack.push(Qt.resolvedUrl("../dialogs/AddEditReviewDialog.qml"),
                                { review: review, parentPage: parentPage, finished: true })
                    }
                }
            }

            ShortBookRow {
                anchors {
                    left: parent.left
                    right: parent.right
                    margins: Theme.horizontalPageMargin
                }

                bookImage: book ? book.imageUrl : ""
                bookTitle: book ? book.title : ""
                bookAuthors: book ? Utils.getAuthorsString(book.authors, Theme.primaryColor) : ""
                bookAverageRating: book ? book.averageRating : 0.0
                bookRatingsCount: book ? book.ratingsCount : 0

                onBookClicked: pageStack.push(Qt.resolvedUrl("BookPage.qml"),
                        { bookId: updateReadingProgressDialog.bookId,
                            book : updateReadingProgressDialog.book })
                onAuthorLinkActivated: pageStack.push(Qt.resolvedUrl("AuthorPage.qml"),
                        { authorId : link })
            }

            ComboBox {
                id: progressComboBox
                property var progressCategories: ["page", "percent"]
                function update() {
                    progressKey = progressCategories[currentIndex]
                }
                currentIndex: {
                    for (var i = 0; i < progressCategories.length; ++i) {
                        if (progressKey === progressCategories[i]) {
                            return i
                        }
                    }
                    return 0
                }

                label: qsTr("Progress")
                menu: ContextMenu {
                    onClosed: progressComboBox.update()
                    MenuItem {
                        text: qsTr("Pages")
                    }
                    MenuItem {
                        text: qsTr("Percent")
                    }
                }
            }

            Component {
                id: pageComponent
                Item {
                    height: row.height
                    Row {
                        id: row
                        spacing: Theme.paddingSmall
                        Label {
                            color: Theme.highlightColor
                            text: qsTr("On page")
                            anchors.verticalCenter: pageField.verticalCenter
                        }
                        TextMetrics {
                            id: textMetricsLabel
                            font.pixelSize: pageField.font.pixelSize
                            text: "Page number"
                            elide: Qt.ElideNone
                        }
                        TextField {
                            id: pageField

                            width: textMetricsLabel.width + 2 * Theme.horizontalPageMargin +
                                   Theme.paddingSmall
                            inputMethodHints: Qt.ImhDigitsOnly
                            placeholderText: qsTr("Page number")
                            label: qsTr("Page number")
                            horizontalAlignment: Qt.AlignHCenter
                            validator: IntValidator {
                                bottom: 0
                                top: 100000
                            }
                            text: pageValue > 0 ? pageValue : ""
                            onTextChanged: pageValue = parseInt(text, 10)
                        }
                        Label {
                            color: Theme.highlightColor
                            text: qsTr("of %1").arg(review && review.book ? review.book.numPages : 0)
                            anchors.verticalCenter: pageField.verticalCenter
                        }
                    }
                }
            }

            Component {
                id: percentComponent
                Item {
                    height: row.height
                    Row {
                        id: row
                        spacing: Theme.paddingSmall
                        TextMetrics {
                            id: textMetricsLabel
                            font.pixelSize: percentField.font.pixelSize
                            text: "Percent"
                            elide: Qt.ElideNone

                        }
                        TextField {
                            id: percentField
                            width: textMetricsLabel.width + 4 * Theme.horizontalPageMargin +
                                   Theme.paddingSmall
                            inputMethodHints: Qt.ImhDigitsOnly
                            placeholderText: qsTr("Percent")
                            label: qsTr("Percent")
                            horizontalAlignment: Qt.AlignHCenter
                            validator: IntValidator {
                                bottom: 0
                                top: 100
                            }
                            text: percentValue > 0 ? percentValue : ""
                            onTextChanged: percentValue = parseInt(text, 10)
                        }
                        Label {
                            color: Theme.highlightColor
                            text: qsTr("% read")
                            anchors.verticalCenter: percentField.verticalCenter
                        }
                    }
                }
            }

            Loader {
                id: setProgressComponent
                anchors {
                    left: parent.left
                    right: parent.right
                    margins: Theme.horizontalPageMargin
                }
                property int pageValue
                property int percentValue
                sourceComponent: progressKey === "percent" ? percentComponent : pageComponent
            }

            TextArea {
                id: commentTextArea
                width: parent.width
                placeholderText: qsTr("What do you think? (Optional)")
            }
        }
        VerticalScrollDecorator{}
    }

    canAccept: !updateReadingProgressDialog.busy && ((progressKey === "page" && pageValue > 0) ||
            (progressKey === "percent" && percentValue > 0))

    function getProgressValue() {
        if (progressKey === "percent") {
            return percentValue
        }
        else {
            return pageValue
        }
    }

    onAccepted: {
        sailreadsManager.updateReadingProgress(bookId, progressKey, getProgressValue(), comment)
    }
}
