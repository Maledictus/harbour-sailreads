/*
The MIT License (MIT)

Copyright (c) 2018-2019 Oleg Linkin <maledictusdemagog@gmail.com>

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

import QtQuick 2.6
import Sailfish.Silica 1.0
import harbour.sailreads 1.0

import "../components"

Page {
    id: authorPage

    property alias authorId : authorProfile.authorId
    property alias author : authorProfile.author

    property bool busy: sailreadsManager.busy && authorPage.status === PageStatus.Active

    function attachPage() {
        if (pageStack._currentContainer.attachedContainer === null
                && sailreadsManager.logged) {
            pageStack.pushAttached(Qt.resolvedUrl("../pages/StatusPage.qml"))
        }
    }

    onStatusChanged: {
        if (status === PageStatus.Active) {
            attachPage()
        }
    }

    AuthorProfileItem {
        id: authorProfile
    }

    Component.onDestruction: {
        authorProfile.cancelRequest()
    }

    SilicaFlickable {
        id: authorView

        anchors.fill: parent
        height: parent.height
        contentHeight: column.height

        Column {
            id: column
            anchors.horizontalCenter: parent.horizontalCenter
            width: parent.width

            PageHeader {
                title: "Author: %1".arg(author ? author.name : "")
            }

            PullDownMenu {
                busy: authorPage.busy
                MenuItem {
                    visible: author && author.userId > 0
                    text: qsTr("Open profile")
                    onClicked: {
                        pageStack.push(Qt.resolvedUrl("../pages/ProfilePage.qml"),
                                { userId : author.userId })
                    }
                }

                MenuItem {
                    text: {
                        return author && author.followingId > 0 ?
                               qsTr("Unfollow") :
                               qsTr("Follow")
                    }
                    onClicked: {
                        if (author && author.followingId > 0) {
                            sailreadsManager.unfollowAuthor(author.id,
                                    author.followingId);
                        }
                        else {
                            sailreadsManager.followAuthor(author.id);
                        }
                    }
                }

                MenuItem {
                    text: qsTr("Refresh")
                    onClicked: {
                        authorProfile.loadAuthorProfile(false)
                    }
                }
            }

            spacing: Theme.paddingMedium

            Row {
                anchors {
                    left: parent.left
                    leftMargin: Theme.horizontalPageMargin
                    right: parent.right
                    rightMargin: Theme.horizontalPageMargin
                }

                spacing: Theme.paddingMedium

                BaseImage {
                    id: avatarImage
                    width: Theme.iconSizeExtraLarge * 1.5
                    height: Theme.iconSizeExtraLarge * 1.5
                    defaultImage: "qrc:/images/gra_small.png"
                    source: {
                        if (author) {
                            if (author.largeImageUrl.toString() !== "") {
                                return author.largeImageUrl
                            }
                            return author.imageUrl
                        }
                        return ""
                    }
                    indicator.size: BusyIndicatorSize.Medium
                }

                Column {
                    width: parent.width - Theme.iconSizeExtraLarge - Theme.paddingMedium

                    Row {
                        visible: author && author.isGoodreadsAuthor
                        spacing: Theme.paddingMedium
                        property color color: Theme.highlightColor
                        Image {
                            source: "image://theme/icon-s-certificates?" + parent.color
                            anchors.verticalCenter: parent.verticalCenter
                        }

                        Label {
                            text: qsTr("Goodreads Author")
                            color: parent.color
                            font.pixelSize: Theme.fontSizeSmall
                        }
                    }

                    KeyValueLabel {
                        font.pixelSize: Theme.fontSizeSmall
                        width: parent.width
                        visible: value !== ""
                        key: qsTr("Born")
                        value: author && author.birthDate ?
                               Qt.formatDate(author.birthDate) :
                               ""
                    }
                    KeyValueLabel {
                        font.pixelSize: Theme.fontSizeSmall
                        width: parent.width
                        visible: value !== ""
                        key: qsTr("Died")
                        value: author && author.deathDate ?
                                Qt.formatDate(author.deathDate) :
                                ""
                    }
                    KeyValueLabel {
                        font.pixelSize: Theme.fontSizeSmall
                        width: parent.width
                        visible: value !== ""
                        key: qsTr("Influences")
                        value: author ? author.influences : ""
                    }
                    KeyValueLabel {
                        font.pixelSize: Theme.fontSizeSmall
                        width: parent.width
                        visible: value !== ""
                        key: qsTr("Followers")
                        value: author ?
                                "%L1".arg(author.followersCount) :
                                ""
                    }

                    Row {
                        spacing: Theme.paddingSmall
                        RatingBox {
                            id: ratingBox
                            rating: author ? author.averageRating : 0.0
                            color: Theme.highlightColor
                        }

                        Label {
                            font.pixelSize: Theme.fontSizeSmall
                            color: Theme.highlightColor
                            anchors.verticalCenter: ratingBox.verticalCenter
                            text: Number(ratingBox.rating).toLocaleString()
                        }
                    }

                    KeyValueLabel {
                        font.pixelSize: Theme.fontSizeSmall
                        width: parent.width
                        visible: value !== ""
                        key: qsTr("Ratings")
                        value: author ?
                                "%L1".arg(Number(author.ratingsCount)
                                        .toLocaleString(Qt.locale(), 'f', 0)) : ""
                    }
                    KeyValueLabel {
                        font.pixelSize: Theme.fontSizeSmall
                        width: parent.width
                        visible: value !== ""
                        key: qsTr("Reviews")
                        value: author ?
                                "%L1".arg(author.textReviewsCount) :
                                ""
                    }
                }
            }

            Separator {
                id: separator

                width: parent.width
                color: Theme.primaryColor
                horizontalAlignment: Qt.AlignHCenter
            }

            property string _style: "<style>" +
                    "a:link { color:" + Theme.highlightColor + "; }" +
                    "p { color:" + Theme.primaryColor + "; }" +
                    "</style>"

            CollapsedLabel {
                anchors {
                    left: parent.left
                    leftMargin: Theme.horizontalPageMargin
                    right: parent.right
                    rightMargin: Theme.horizontalPageMargin
                }
                visible: author && author.about !== ""
                text: author ? (column._style + author.about) : ""
                onLinkActivated: mainWindow.openPageFromUrl(link)
            }

            MoreButton {
                width: parent.width
                height: Theme.itemSizeMedium
                text: qsTr("Quotes")
                counter: ""
                busy: authorPage.busy
                enabled: !busy
                onClicked: {
                    pageStack.push(Qt.resolvedUrl("../pages/AuthorQuotesPage.qml"),
                        { authorId: authorId, authorName: author ? author.name : "" })
                }
            }

            MoreButton {
                id: seriesButton
                width: parent.width
                height: Theme.itemSizeMedium
                text: qsTr("Series")
                counter: ""
                busy: authorPage.busy
                enabled: !busy
                onClicked: {
                    pageStack.push(Qt.resolvedUrl("../pages/AuthorSeriesPage.qml"), {
                        authorId: author ? author.id : "",
                        authorName: author ? author.name : ""
                    })
                }
            }

            MoreButton {
                id: booksButton
                width: parent.width
                height: Theme.itemSizeMedium
                text: qsTr("Books")
                counter: author ? author.worksCount : 0
                busy: authorPage.busy
                enabled: !busy
                onClicked: {
                    pageStack.push(Qt.resolvedUrl("../pages/AuthorBooksPage.qml"), {
                        authorId: author ? author.id : "",
                        authorName: author ? author.name : ""
                    })
                }
            }

            SilicaListView {
                id: booksView

                width: parent.width
                height: contentHeight
                clip: true

                model: authorProfile.shortBooksModel

                ViewPlaceholder {
                    enabled: !sailreadsManager.busy && booksView.count === 0
                    text: qsTr("There are no books")
                    hintText: qsTr("Pull down to refresh")
                }

                delegate: BookListItem {
                    id: delegate
                    width: booksView.width
                    clip: true

                    imageUrl: bookImageUrl
                    title: bookTitle
                    authors: bookAuthors
                    averageRating: bookAverageRating
                    ratingsCount: bookRatingsCount

                    IconTextButton {
                        parent: delegate.customItem
                        label.font.pixelSize: Theme.fontSizeMedium
                        label.color: bookBook.review || highlighted || delegate.highlighted ?
                                Theme.highlightColor : Theme.primaryColor
                        label.text: !bookBook.review ? qsTr("Want to Read") :
                                (bookBook.review ? bookBook.review.exclusiveShelf : "")
                        icon.source: !bookBook.review ? "image://theme/icon-m-add" :
                                "image://theme/icon-m-acknowledge"
                        icon.highlighted: bookBook.review || highlighted || delegate.highlighted
                        onClicked: {
                            if (!bookBook.review) {
                                sailreadsManager.addBookToShelves(bookBook.id, ["to-read"])
                            }
                            else {
                                pageStack.push(Qt.resolvedUrl("../pages/AddBookToShelvesPage.qml"),
                                        { bookId: bookBook.id, book: bookBook, review: bookBook.review })
                            }
                        }
                    }

                    menu: ContextMenu {
                        MenuItem {
                            text: qsTr("Add to My Books")
                            onClicked: {
                                pageStack.push(Qt.resolvedUrl("../pages/AddBookToShelvesPage.qml"),
                                        { bookId: bookBook.id, book: bookBook, review: bookBook.review })
                            }
                        }
                    }

                    onClicked: {
                        pageStack.push(Qt.resolvedUrl("../pages/BookPage.qml"),
                                { book: bookBook, bookId: bookBook.id })
                    }
                }
            }
        }
        VerticalScrollDecorator{}
    }

    BusyIndicator {
        size: BusyIndicatorSize.Large
        anchors.centerIn: parent
        running: authorPage.busy
        visible: running
    }
}
