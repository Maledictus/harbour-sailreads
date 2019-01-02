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
import "../utils/Utils.js" as Utils

Page {
    id: bookPage

    property alias bookId: bookItem.bookId
    property alias book: bookItem.book
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
            if (book !== null) {
                authorsLabel.text = Utils.getAuthorsString(book.authors, Theme.primaryColor)
                seriesValueLabel.text = Utils.getSeriesString(book.seriesList, Theme.primaryColor)
            }
        }
    }

    Component.onDestruction: {
        sailreadsManager.abortRequest()
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
                    visible: false //TODO
                    text: qsTr("Add to Bookshelves")
                    onClicked: {
//                        var dialog = pageStack.push("../dialogs/AddBookToShelvesDialog.qml",
//                                { usedShelves: book.review.shelvesList })
//                        dialog.accepted.connect (function () {
//                            sailreadsManager.addBookToShelves(bookId, dialog.newShelves)
//                        })
                    }
                }

                MenuItem {
                    visible: false //TODO book && book.review && book.review.shelvesList.length === 0
                    text: qsTr("Want to Read")
                }
            }

            PageHeader {
                title: book ? book.title : ""
                description: book && book.review && book.review.shelvesList.length > 0 ? book.review.shelvesList[0] : ""
            }

            BaseImage {
                source: book ? book.imageUrl : ""
                anchors.horizontalCenter: parent.horizontalCenter
                height: Theme.coverSizeSmall.height
                width: Theme.coverSizeSmall.width
                indicator.size: BusyIndicatorSize.Medium
            }

            Label {
                id: titleLabel
                width: parent.width
                text: book ? book.title : ""
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
                Component.onCompleted: {
                    var result = ""
                    if (book !== null) {
                        result = Utils.getSeriesString(book.authors, Theme.primaryColor)
                    }
                    text = result
                }
                onLinkActivated: {
                    if (book) {
                        var author
                        for (var i = 0; i < book.authors.length; ++i) {
                            if (Number(book.authors[i].id).toFixed() === Number(link).toFixed()) {
                                author = book.authors[i]
                                break
                            }
                        }

                        pageStack.push(Qt.resolvedUrl("AuthorPage.qml"),
                                { authorId: Number(link).toFixed(), author: author })
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
                    color: Theme.highlightColor
                    rating: book ? book.averageRating : 0.0
                }

                Label {
                    Layout.alignment: Qt.AlignLeft
                    color: Theme.highlightColor
                    text: Number(book ? book.averageRating : 0.0).toLocaleString()
                    Layout.fillWidth: true
                    horizontalAlignment: Text.AlignLeft
                }

                Label {
                    color: Theme.highlightColor
                    Layout.alignment: Qt.AlignRight
                    font.pixelSize: Theme.fontSizeSmall
                    text: qsTr("%1 ratings").arg(Number(book ? book.ratingsCount : 0.0)
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
                    text: book && book.review && book.review.rating > 0 ?
                            qsTr("My Rating") : qsTr("Rate this book")
                }

                VotingBox {
                    rating: book && book.review ? book.review.rating : 0
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
                text: qsTr("My review")
                visible: false //TODObook && book.review
            }

            Button {
                anchors.horizontalCenter: parent.horizontalCenter
                text: qsTr("Write a Review")
                visible: false //TODO book && book.review && book.review.body === ""
                onClicked: {
                }
            }

            MoreButton {
                id: bookShelvesButton
                width: parent.width
                height: Theme.itemSizeMedium
                text: qsTr("Friends Reviews")
                counter: book ? book.friendReviews.length : 0
                busy: bookPage.busy
                enabled: !busy
                visible: book && book.friendReviews.length > 0
                onClicked: {
                    pageStack.push(Qt.resolvedUrl("UserReviewsPage.qml"),
                            { usersReviews: book ? book.friendReviews : [] })
                }
            }

            SilicaListView {
                id:friendsReviewsView

                visible: book && book.friendReviews.length > 0

                width: parent.width
                height: contentHeight
                delegate: ReviewListItem {
                    userId: modelData.user.id
                    userAvatarUrl: modelData.user.avatarUrl
                    userName: modelData.user.userName
                    reviewId: modelData.id
                    reviewRate: modelData.rating
                    reviewText: modelData.body
                    reviewDate: modelData.updatedDate
                    withBody: false

                    onUserClicked: {
                        pageStack.push(Qt.resolvedUrl("ProfilePage.qml"),
                                { userId: userId })
                    }

                    onClicked: {
                        pageStack.push(Qt.resolvedUrl("ReviewPage.qml"),
                                { reviewId: modelData.id, review: modelData })
                    }
                }

                model: {
                    var result = []
                    if (book) {
                        for (var i = 0; i < book.friendReviews.length; ++i) {
                            if (i < 3) {
                                result.push(book.friendReviews[i])
                            }
                        }
                    }
                    return result
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
                    visible: seriesValueLabel.text !== ""

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
                        Component.onCompleted: {
                            var result = ""
                            if (book !== null) {
                                result = Utils.getSeriesString(book.seriesList, Theme.primaryColor)
                            }
                            text = result
                        }
                        onLinkActivated: {
                            pageStack.push(Qt.resolvedUrl("SeriesPage.qml"),
                                    { seriesId: link })
                        }
                    }
                }

                KeyValueLabel {
                    font.pixelSize: Theme.fontSizeSmall
                    key: qsTr("Pages")
                    value: book ? book.numPages : 0
                    visible: book && book.numPages > 0
                }

                KeyValueLabel {
                    font.pixelSize: Theme.fontSizeSmall
                    key: qsTr("Published")
                    value: book ? (book.publishedYear > 0 ? book.publishedYear : book.publicationYear) : 0
                    visible: book && (book.publishedYear > 0 || book.publicationYear > 0)
                }

                KeyValueLabel {
                    font.pixelSize: Theme.fontSizeSmall
                    key: "ISBN"
                    value: book ? book.isbn : ""
                    visible: book && book.isbn !== ""
                }

                KeyValueLabel {
                    font.pixelSize: Theme.fontSizeSmall
                    key: "ISBN13"
                    value: book ? book.isbn13 : ""
                    visible: book && book.isbn13 !== ""
                }

                Item { height: Theme.paddingMedium }

                CollapsedLabel {
                    text: book ? book.description : ""
                    onLinkActivated: {
                        Qt.openUrlExternally(link)
                    }
                }
            }

            MoreButton {
                width: parent.width
                height: Theme.itemSizeMedium
                text: qsTr("Similar Books")
                counter: book ? book.similarBooks.length : 0
                busy: bookPage.busy
                enabled: !busy
                visible: book && book.similarBooks.length > 0
                onClicked: {
                    pageStack.push(Qt.resolvedUrl("BooksPage.qml"),
                        { books: book ? book.similarBooks : undefined, title: qsTr("Similar Books") })
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
                                { bookId: book ? book.similarBooks[index].id : 0,
                                    book: book ? book.similarBooks[index] : undefined })
                    }
                }
                HorizontalScrollDecorator{}
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

        VerticalScrollDecorator {}
    }


    BusyIndicator {
        size: BusyIndicatorSize.Large
        anchors.centerIn: parent
        running: bookPage.busy
        visible: running
    }
}
