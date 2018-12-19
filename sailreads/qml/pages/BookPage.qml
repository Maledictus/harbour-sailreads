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

    property alias bookId: bookItem.bookId
    property var review
    property bool busy: sailreadsManager.busy && bookPage.status === PageStatus.Active

    function attachPage() {
        if (pageStack._currentContainer.attachedContainer === null
                && sailreadsManager.logged) {
            pageStack.pushAttached(Qt.resolvedUrl("StatusPage.qml"))
        }
    }

    BookItem {
        id: bookItem
        onBookChanged: {
            authorsLabel.text = generateAuthorsString()
            seriesValueLabel.text = generateSeriesString()
        }
    }

    Component.onDestruction: {
        sailreadsManager.abortRequest()
    }

    function generateAuthorsString() {
        if (bookItem.book === null || bookItem.book.authors.length === 0) {
            return "";
        }

        var result = qsTr("<style>a:link{color:" + Theme.primaryColor + ";}</style>")
        for (var i = 0; i < bookItem.book.authors.length; ++i) {
            result += "<a href=\"%1\" style=\"text-decoration:none;\">%2</a>"
                    .arg(Number(bookItem.book.authors[i].id).toFixed())
                    .arg(bookItem.book.authors[i].name)
            if (i +1 < bookItem.book.authors.length) {
                result += ", "
            }
        }
        return result
    }

    function generateSeriesString() {
        if (bookItem.book === null || bookItem.book.seriesList.length === 0) {
            return "";
        }

        var result = qsTr("<style>a:link{color:" + Theme.primaryColor + ";}</style>")
        for (var i = 0; i < bookItem.book.seriesList.length; ++i) {
            result += "<a href=\"%1\" style=\"text-decoration:none;\">%2</a>"
                    .arg(Number(bookItem.book.seriesList[i].id).toFixed())
                    .arg(bookItem.book.seriesList[i].title)
            if (i +1 < bookItem.book.seriesList.length) {
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
                    onClicked: {
                        var dialog = pageStack.push("../dialogs/AddBookToShelvesDialog.qml",
                                { usedShelves: review.shelvesList })
                        dialog.accepted.connect (function () {
                            sailreadsManager.addBookToShelves(bookId, dialog.newShelves)
                        })
                    }
                }

                MenuItem {
                    visible: review ? review.shelvesList.length === 0 : true
                    text: qsTr("Want to Read")
                }
            }

            PageHeader {
                title: review ? review.book.title : (bookItem.book ? bookItem.book.title : "")
                description: review && review.shelvesList.length > 0 ? review.shelvesList[0] : ""
            }

            BaseImage {
                source: review ? review.book.imageUrl : (bookItem.book ? bookItem.book.imageUrl : "")
                anchors.horizontalCenter: parent.horizontalCenter
                height: Theme.coverSizeSmall.height
                width: Theme.coverSizeSmall.width
                indicator.size: BusyIndicatorSize.Medium
            }

            Label {
                id: titleLabel
                width: parent.width
                text: review ? review.book.title : (bookItem.book ? bookItem.book.title : "")
                wrapMode: Text.WordWrap
                maximumLineCount: 2
                anchors {
                    left: parent.left
                    leftMargin: Theme.horizontalPageMargin
                    right: parent.right
                    rightMargin: Theme.horizontalPageMargin
                }
                color: Theme.highlightColor
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
                    var author
                    for (var i = 0; i < bookItem.book.authors.length; ++i) {
                        if (Number(bookItem.book.authors[i].id).toFixed() === Number(link).toFixed()) {
                            author = bookItem.book.authors[i]
                            break
                        }
                    }

                    pageStack.push(Qt.resolvedUrl("AuthorPage.qml"),
                            { authorId: Number(link).toFixed(), author: author })
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
                    color: Theme.highlightColor
                    rating: review ? review.book.averageRating :
                            (bookItem.book ? bookItem.book.averageRating : 0)
                }

                Label {
                    Layout.alignment: Qt.AlignLeft
                    color: Theme.highlightColor
                    text: Number(review ? review.book.averageRating : (bookItem.book ?
                            bookItem.book.averageRating : 0)).toLocaleString()
                    Layout.fillWidth: true
                    horizontalAlignment: Text.AlignLeft
                }

                Label {
                    color: Theme.highlightColor
                    Layout.alignment: Qt.AlignRight
                    font.pixelSize: Theme.fontSizeSmall
                    text: qsTr("%1 ratings").arg(Number(review ? review.book.ratingsCount :
                                (bookItem.book ? bookItem.book.ratingsCount : 0))
                                    .toLocaleString(Qt.locale(), 'f', 0))
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

                    color: Theme.highlightColor
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
                            pageStack.push(Qt.resolvedUrl("SeriesPage.qml"),
                                    { seriesId: link })
                        }
                    }
                }

                KeyValueLabel {
                    font.pixelSize: Theme.fontSizeSmall
                    key: qsTr("Pages")
                    value: bookItem.book ? bookItem.book.numPages : 0
                    visible: bookItem.book ? bookItem.book.numPages > 0 : false
                }

                KeyValueLabel {
                    font.pixelSize: Theme.fontSizeSmall
                    key: qsTr("Published")
                    value: bookItem.book ? (bookItem.book.publishedYear > 0 ? bookItem.book.publishedYear : bookItem.book.publicationYear) : 0
                    visible: bookItem.book ? (bookItem.book.publishedYear > 0 || bookItem.book.publicationYear > 0) : false
                }

                KeyValueLabel {
                    font.pixelSize: Theme.fontSizeSmall
                    key: "ISBN"
                    value: bookItem.book ? bookItem.book.isbn : ""
                    visible: bookItem.book ? (bookItem.book.isbn !== "") : false
                }

                KeyValueLabel {
                    font.pixelSize: Theme.fontSizeSmall
                    key: "ISBN13"
                    value: bookItem.book ? bookItem.book.isbn13 : 0
                    visible: bookItem.book ? (bookItem.book.isbn13 !== "") : false
                }

                Item { height: Theme.paddingMedium }

                CollapsedLabel {
                    text: bookItem.book ? bookItem.book.description : ""
                    onLinkActivated: {
                        Qt.openUrlExternally(link)
                    }
                }
            }

            MoreButton {
                width: parent.width
                height: Theme.itemSizeMedium
                text: qsTr("Similar Books")
                counter: bookItem.book ? bookItem.book.similarBooks.length : 0
                busy: bookPage.busy
                enabled: !busy
                visible: bookItem.book ? bookItem.book.similarBooks.length > 0 : false
                onClicked: {
                    pageStack.push(Qt.resolvedUrl("BooksPage.qml"),
                        { books: bookItem.book.similarBooks, title: qsTr("Similar Books") })
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
                model: bookItem.book ? bookItem.book.similarBooks : undefined
                spacing: Theme.paddingSmall
                delegate: BaseImage {
                    height: 1.5 * width
                    width: Theme.iconSizeLarge
                    source: modelData.imageUrl
                    indicator.size: BusyIndicatorSize.Medium
                    onClicked:  {
                        pageStack.push(Qt.resolvedUrl("BookPage.qml"),
                                { bookId: bookItem.book.similarBooks[index].id,
                                    book: bookItem.book.similarBooks[index] })
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
        running: bookPage.busy
        visible: running
    }
}
