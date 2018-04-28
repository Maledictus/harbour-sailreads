/*
The MIT License (MIT)

Copyright (c) 2018 Oleg Linkin <maledictusdemagog@gmail.com>

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

import QtQuick 2.2
import QtQuick.Layouts 1.1
import Sailfish.Silica 1.0
import harbour.sailreads 1.0

import "../components"

Page {
    id: bookPage

    property int bookId
    property variant review
    property variant book
    property bool busy: sailreadsManager.busy && bookPage.status == PageStatus.Active

    function attachPage() {
        if (pageStack._currentContainer.attachedContainer === null
                && sailreadsManager.logged) {
            pageStack.pushAttached(Qt.resolvedUrl("StatusPage.qml"))
        }
    }

    Component.onCompleted: {
        sailreadsManager.loadBook(bookId)
    }

    Component.onDestruction: {
        sailreadsManager.abortRequest()
    }

    Connections {
        target: sailreadsManager
        onGotBook: {
            if (bookPage.bookId === book.id) {
                bookPage.book = book
            }
        }
    }

    SilicaFlickable {
        anchors.fill: parent
        contentHeight: column.height + Theme.paddingMedium

        Column {
            id: column

            width: parent.width

            spacing: Theme.paddingSmall

            PullDownMenu {
                MenuItem {
                    text: qsTr("Add to Bookshelves")
                }

                MenuItem {
                    visible: review ? review.shelvesList.length === 0 : true
                    text: qsTr("Want to Read")
                }
            }

            PageHeader {
                title: review ? review.book.title : (book ? book.title : "")
                description: review ? (review.shelvesList.length > 0 ? review.shelvesList[0] : "") : ""
            }

            BaseImage {
                source: review ? review.book.imageUrl : (book ? book.imageUrl : "")
                anchors.horizontalCenter: parent.horizontalCenter
                height: Theme.coverSizeSmall.height
                width: Theme.coverSizeSmall.width
                indicator.size: BusyIndicatorSize.Medium
            }

            Label {
                id: titleLabel
                width: parent.width
                text: review ? review.book.title : (book ? book.title : "")
                wrapMode: Text.WordWrap
                maximumLineCount: 2
                anchors {
                    left: parent.left
                    leftMargin: Theme.horizontalPageMargin
                    right: parent.right
                    rightMargin: Theme.horizontalPageMargin
                }
                horizontalAlignment: Text.AlignHCenter
                font.family: Theme.fontFamilyHeading
            }

            Flow {
                spacing: Theme.paddingMedium
                anchors {
                    leftMargin: 3 * Theme.horizontalPageMargin
                    rightMargin: 3 * Theme.horizontalPageMargin
                    horizontalCenter: parent.horizontalCenter
                }

                Label {
                    font.pixelSize: Theme.fontSizeExtraSmall
                    text: qsTr("by")
                }

                Repeater {
                    model: book ? book.authors.length : 0
                    ClickableLabel {
                        label.text: book.authors[index].name
                        label.font.pixelSize: Theme.fontSizeExtraSmall
                        label.horizontalAlignment: Qt.AlignLeft
                        width: label.implicitWidth
                        onClicked: {
                            //TODO open author page
                        }
                    }
                }
            }

            Separator {
                width: parent.width
                color: Theme.primaryColor
                horizontalAlignment: Qt.AlignHCenter
            }

            RowLayout {
                anchors {
                    left: parent.left
                    leftMargin: Theme.horizontalPageMargin
                    right: parent.right
                    rightMargin: Theme.horizontalPageMargin
                }

                RatingBox {
                    Layout.alignment: Qt.AlignLeft
                    rating: review ? review.book.averageRating : (book ? book.averageRating : 0)
                }

                Label {
                    Layout.alignment: Qt.AlignLeft
                    text: Number(review ? review.book.averageRating : (book ? book.averageRating : 0))
                            .toFixed(2)
                    Layout.fillWidth: true
                    horizontalAlignment: Text.AlignLeft
                }

                Label {
                    Layout.alignment: Qt.AlignRight
                    font.pixelSize: Theme.fontSizeSmall
                    text: qsTr("%1 ratings")
                            .arg(Number(review ? review.book.ratingsCount :
                                        (book ? book.ratingsCount : 0)).toFixed())
                }
            }

            Separator {
                width: parent.width
                color: Theme.primaryColor
                horizontalAlignment: Qt.AlignHCenter
            }

            Column {
                anchors {
                    left: parent.left
                    leftMargin: Theme.horizontalPageMargin
                    right: parent.right
                    rightMargin: Theme.horizontalPageMargin
                }

                Label {
                    anchors {
                        horizontalCenter: parent.horizontalCenter
                    }

                    font.pixelSize: Theme.fontSizeMedium
                    horizontalAlignment: Text.AlignHCenter
                    text: qsTr("Rate this book:")
                }

                VotingBox {
                    anchors {
                        horizontalCenter: parent.horizontalCenter
                    }
                    onUserVoteChanged: {
                        //TODO
                        console.log(userVote)
                    }
                }
            }

            SectionHeader {
                text: qsTr("Description")
            }

            Column {
                anchors {
                    left: parent.left
                    leftMargin: Theme.horizontalPageMargin
                    right: parent.right
                    rightMargin: Theme.horizontalPageMargin
                }

                KeyValueLabel {
                    font.pixelSize: Theme.fontSizeSmall
                    key: qsTr("Pages")
                    value: book ? book.numPages : 0
                    visible: book ? book.numPages > 0 : false
                }

                KeyValueLabel {
                    font.pixelSize: Theme.fontSizeSmall
                    key: qsTr("Published")
                    value: book ? (book.publishedYear > 0 ? book.publishedYear : book.publicationYear) : 0
                    visible: book ? (book.publishedYear > 0 || book.publicationYear > 0) : false
                }

                KeyValueLabel {
                    font.pixelSize: Theme.fontSizeSmall
                    key: "ISBN"
                    value: book ? book.isbn : ""
                    visible: book ? (book.isbn !== "") : false
                }

                KeyValueLabel {
                    font.pixelSize: Theme.fontSizeSmall
                    key: "ISBN13"
                    value: book ? book.isbn13 : 0
                    visible: book ? (book.isbn13 !== "") : false
                }

                Item { height: Theme.paddingMedium }

                CollapsedLabel {
                    text: book ? book.description : ""
                }
            }

            MoreButton {
                width: parent.width
                height: Theme.itemSizeMedium
                text: qsTr("Similar Books")
                busy: bookPage.busy
                enabled: !busy
                visible: book ? book.similarBooks.length > 0 : false
                onClicked: {
                    //TODO open page with similar books
                }
            }

            SilicaFlickable {
                height: row.height
                contentWidth: row.width
                anchors {
                    left: parent.left
                    right: parent.right
                }

                Row {
                    id: row
                    spacing: Theme.paddingSmall
                    Repeater {
                        model: book ? book.similarBooks.length : 0
                        BaseImage {
                            height: 1.5 * width
                            width: Theme.iconSizeLarge
                            source: book.similarBooks[index].imageUrl
                            indicator.size: BusyIndicatorSize.Medium
                            onClicked:  {
                                pageStack.push(Qt.resolvedUrl("BookPage.qml"),
                                        { bookId: book.similarBooks[index].id,
                                            book: book.similarBooks[index] })
                            }
                        }
                    }
                }
                HorizontalScrollDecorator{}
            }

            //Add to shelf
            //Similar books
            //More button on reviews
        }
    }


    BusyIndicator {
        size: BusyIndicatorSize.Large
        anchors.centerIn: parent
        running: sailreadsManager.busy
        visible: running
    }
}
