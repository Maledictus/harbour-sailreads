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

import QtQuick 2.5
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
            //pageStack.pushAttached(Qt.resolvedUrl("StatusPage.qml"))
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
                title: "Author: %1".arg(authorProfile.author ? authorProfile.author.name : "")
            }

            PullDownMenu {
                busy: authorPage.busy
                MenuItem {
                    visible: author && author.userId > 0
                    text: qsTr("Open profile")
                    onClicked: {
                        pageStack.push(Qt.resolvedUrl("ProfilePage.qml"),
                                { userId : author.userId })
                    }
                }

                MenuItem {
                    text: {
                        return authorProfile.author && authorProfile.author.followingId > 0 ?
                               qsTr("Unfollow") :
                               qsTr("Follow")
                    }
                    onClicked: {
                        if (authorProfile.author && authorProfile.author.followingId > 0) {
                            sailreadsManager.unfollowAuthor(authorProfile.author.id,
                                    authorProfile.author.followingId);
                        }
                        else {
                            sailreadsManager.followAuthor(authorProfile.author.id);
                        }
                    }
                }

                MenuItem {
                    text: qsTr("Refresh")
                    onClicked: {
                        authorProfile.loadAuthorProfile()
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
                    source: {
                        if (authorProfile.author) {
                            if (authorProfile.author.largeImageUrl.toString() !== "") {
                                return authorProfile.author.largeImageUrl
                            }
                            return authorProfile.author.imageUrl
                        }
                        return ""
                    }
                    indicator.size: BusyIndicatorSize.Medium
                }

                Column {
                    width: parent.width - Theme.iconSizeExtraLarge - Theme.paddingMedium

                    Row {
                        visible: authorProfile.author && authorProfile.author.isGoodreadsAuthor
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
                        value: authorProfile.author && authorProfile.author.birthDate ?
                               Qt.formatDate(authorProfile.author.birthDate) :
                               ""
                    }
                    KeyValueLabel {
                        font.pixelSize: Theme.fontSizeSmall
                        width: parent.width
                        visible: value !== ""
                        key: qsTr("Died")
                        value: authorProfile.author && authorProfile.author.deathDate ?
                                Qt.formatDate(authorProfile.author.deathDate) :
                                ""
                    }
                    KeyValueLabel {
                        font.pixelSize: Theme.fontSizeSmall
                        width: parent.width
                        visible: value !== ""
                        key: qsTr("Influences")
                        value: authorProfile.author ? authorProfile.author.influences : ""
                    }
                    KeyValueLabel {
                        font.pixelSize: Theme.fontSizeSmall
                        width: parent.width
                        visible: value !== ""
                        key: qsTr("Followers")
                        value: authorProfile.author ? authorProfile.author.followersCount : ""
                    }

                    Row {
                        spacing: Theme.paddingSmall
                        RatingBox {
                            id: ratingBox
                            rating: authorProfile.author ? authorProfile.author.averageRating : 0.0
                            color: Theme.highlightColor
                        }

                        Label {
                            font.pixelSize: Theme.fontSizeSmall
                            color: Theme.highlightColor
                            anchors.verticalCenter: ratingBox.verticalCenter
                            text: Number(ratingBox.rating).toLocaleString(Qt.locale(), 'f', 2)
                        }
                    }

                    KeyValueLabel {
                        font.pixelSize: Theme.fontSizeSmall
                        width: parent.width
                        visible: value !== ""
                        key: qsTr("Ratings")
                        value: authorProfile.author ?
                                Number(authorProfile.author.ratingsCount).toLocaleString() : ""
                    }
                    KeyValueLabel {
                        font.pixelSize: Theme.fontSizeSmall
                        width: parent.width
                        visible: value !== ""
                        key: qsTr("Reviews")
                        value: authorProfile.author ?
                                Number(authorProfile.author.textReviewsCount).toLocaleString() : ""
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
                visible: authorProfile.author && authorProfile.author.about !== ""
                text: authorProfile.author ? (column._style + authorProfile.author.about) : ""
                onLinkActivated: mainWindow.openPageFromUrl(link)
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
                    pageStack.push(Qt.resolvedUrl("AuthorSeriesPage.qml"), {
                        authorId: authorProfile.author ? authorProfile.author.id : 0,
                        authorName: authorProfile.author ? authorProfile.author.name : ""
                    })
                }
            }

            MoreButton {
                id: booksButton
                width: parent.width
                height: Theme.itemSizeMedium
                text: qsTr("Books")
                counter: authorProfile.author ? authorProfile.author.worksCount : 0
                busy: authorPage.busy
                enabled: !busy
                onClicked: {
                    pageStack.push(Qt.resolvedUrl("AuthorBooksPage.qml"), {
                        authorId: authorProfile.author ? authorProfile.author.id : 0,
                        authorName: authorProfile.author ? authorProfile.author.name : ""
                    })
                }
            }

            SilicaListView {
                id: booksView

                width: parent.width
                height: contentHeight
                clip: true

                model: authorProfile.author ? authorProfile.author.books : null

                delegate: BookListItem {
                    width: booksView.width
                    clip: true

                    imageUrl: modelData.imageUrl
                    title: modelData.title
                    authors: modelData.authorsString
                    averageRating: modelData.averageRating
                    ratingsCount: modelData.ratingsCount

                    onClicked: {
                        pageStack.push(Qt.resolvedUrl("BookPage.qml"),
                                { book: modelData, bookId: modelData.id })
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
