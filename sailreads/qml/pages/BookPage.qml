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
    property var review
    property var book
    property bool busy: sailreadsManager.busy && bookPage.status === PageStatus.Active

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
                authorsLabel.text = generateAuthorsString()
                seriesValueLabel.text = generateSeriesString()
            }
        }
    }

    function generateAuthorsString() {
        if (book === undefined || book.authors.length === 0) {
            return "";
        }

        var result = qsTr("<style>a:link{color:" + Theme.highlightColor + ";}</style>by ")
        for (var i = 0; i < book.authors.length; ++i) {
            result += "<a href=\"%1\" style=\"text-decoration:none;\">%2</a>"
                    .arg(book.authors[i].id)
                    .arg(book.authors[i].name)
            if (i +1 < book.authors.length) {
                result += ", "
            }
        }
        return result
    }

    function generateSeriesString() {
        if (book === undefined || book.seriesWorks.length === 0) {
            return "";
        }

        var result = qsTr("<style>a:link{color:" + Theme.highlightColor + ";}</style>")
        for (var i = 0; i < book.seriesWorks.length; ++i) {
            result += "<a href=\"%1\" style=\"text-decoration:none;\">%2</a>"
                    .arg(book.seriesWorks[i].series.id)
                    .arg(book.seriesWorks[i].series.title)
            if (i +1 < book.seriesWorks.length) {
                result += ", "
            }
        }
        return result
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

            Label {
                id: authorsLabel
                anchors {
                    left: parent.left
                    leftMargin: Theme.horizontalPageMargin
                    right: parent.right
                    rightMargin: Theme.horizontalPageMargin
                }
                textFormat: Text.RichText
                font.pixelSize: Theme.fontSizeExtraSmall
                wrapMode: Text.WordWrap
                horizontalAlignment: height < 2 * font.pixelSize ? Text.AlignHCenter : Text.AlignJustify
                Component.onCompleted: text = generateAuthorsString()
                onLinkActivated: {
                    //TODO open author page
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
                    rating: review ? review.rating : 0
                    anchors {
                        horizontalCenter: parent.horizontalCenter
                    }
                    onUserVoteChanged: {
                        //TODO send vote
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

                Row {
                    width: parent.width
                    height: seriesValueLabel.height
                    spacing: Theme.paddingSmall

                    Label {
                        id: seriesKeyLabel
                        font.pixelSize: Theme.fontSizeSmall
                        textFormat: Text.StyledText
                        visible: seriesValueLabel.text !== ""
                        text: "<font color=\"" + Theme.secondaryHighlightColor + "\">" +
                              qsTr("Series") + "</font>"
                    }
                    Label {
                        id: seriesValueLabel
                        width: parent.width - seriesKeyLabel.width - Theme.paddingSmall
                        textFormat: Text.RichText
                        font.pixelSize: Theme.fontSizeSmall
                        wrapMode: Text.WordWrap
                        Component.onCompleted: text = generateSeriesString()
                        onLinkActivated: {
                            //TODO open series page
                        }
                    }
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
                counter: book ? book.similarBooks.length : 0
                busy: bookPage.busy
                enabled: !busy
                visible: book ? book.similarBooks.length > 0 : false
                onClicked: {
                    pageStack.push(Qt.resolvedUrl("BooksPage.qml"),
                        { books: book.similarBooks, title: qsTr("Similar Books") })
                }
            }

            SilicaListView {
                orientation: Qt.Horizontal
                height: Theme.iconSizeLarge * 1.5
                anchors {
                    left: parent.left
                    leftMargin: Theme.horizontalPageMargin
                    right: parent.right
                    rightMargin: Theme.horizontalPageMargin
                }
                clip: true
                model: book ? book.similarBooks : undefined
                spacing: Theme.paddingSmall
                delegate: BaseImage {
                    height: 1.5 * width
                    width: Theme.iconSizeLarge
                    source: modelData.imageUrl
                    indicator.size: BusyIndicatorSize.Medium
                    onClicked:  {
                        pageStack.push(Qt.resolvedUrl("BookPage.qml"),
                                { bookId: book.similarBooks[index].id,
                                    book: book.similarBooks[index] })
                    }
                }
            }

//TODO
//            MoreButton {
//                width: parent.width
//                height: Theme.itemSizeMedium
//                text: qsTr("Community Reviews")
//                busy: bookPage.busy
//                enabled: !busy
//                visible: book ? book.reviewsWidgetContent !== "" : false
//                onClicked: {
//                    pageStack.push(Qt.resolvedUrl("CommunityReviewsPage.qml"),
//                            { content: book.reviewsWidgetContent })
//                }
//            }
        }
    }


    BusyIndicator {
        size: BusyIndicatorSize.Large
        anchors.centerIn: parent
        running: sailreadsManager.busy
        visible: running
    }
}
