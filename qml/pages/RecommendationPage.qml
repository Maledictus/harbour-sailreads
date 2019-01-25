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

import QtQuick 2.2
import Sailfish.Silica 1.0
import harbour.sailreads 1.0

import "../components"
import "../utils/Utils.js" as Utils

Page {
    id: recommendationPage

    property alias recommendationId : recommendationItem.recommendationId
    property alias recommendation : recommendationItem.recommendation
    property bool busy: sailreadsManager.busy && recommendationPage.status === PageStatus.Active

    function attachPage() {
        if (pageStack._currentContainer.attachedContainer === null
                && sailreadsManager.logged) {
            //pageStack.pushAttached(Qt.resolvedUrl("StatusPage.qml"))
        }
    }

    Component.onDestruction: {
        recommendationItem.cancelRequest()
    }

    RecommendationItem {
        id: recommendationItem
    }

    SilicaListView {
        id: commentsView
        anchors.fill: parent
        cacheBuffer: recommendationPage.height

        function fetchMoreIfNeeded() {
            if (!recommendationPage.busy &&
                    commentsModel.hasMore &&
                    commentsModel.rowCount() > 0 &&
                    indexAt(contentX, contentY + height) > commentsModel.rowCount() - 2) {
                commentsModel.fetchMoreContent()
            }
        }


        contentY: -headerItem.height

        onContentYChanged: fetchMoreIfNeeded()

        PullDownMenu {
            busy: recommendationPage.busy
             MenuItem {
                text: qsTr("Refresh")
                onClicked: {
                    recommendationItem.loadRecommendation()
                }
            }
        }

        header: Item {
            width: commentsView.width
            height: recommendationBox.height
        }

        model: RecommendationCommentsModel {
            id: commentsModel
            recommendationId: recommendationPage.recommendationId
        }

        delegate: CommentListItem {
            width: parent.width
            author: commentAuthor
            body: commentBody
            updateDate: commentUpdateDate
            onUserClicked: pageStack.push(Qt.resolvedUrl("ProfilePage.qml"), { userId: userId })
            onLinkActivated: mainWindow.openPageFromUrl(link)
        }
        VerticalScrollDecorator{}
    }

    Column {
        id: recommendationBox
        parent: commentsView.headerItem ? commentsView.headerItem : recommendationPage
        width: parent.width

        spacing: Theme.paddingMedium

        PageHeader {
            title: qsTr("Recommendation")
        }

        Column {
            anchors {
                left: parent.left
                leftMargin: Theme.horizontalPageMargin
                right: parent.right
                rightMargin: Theme.horizontalPageMargin
            }

            spacing: Theme.paddingMedium

            UserStatusRow {
                width: parent.width
                userAvatar: recommendation && recommendation.fromUser ?
                        recommendation.fromUser.avatarUrl :
                        "qrc:/images/gra_small.png"
                userHeader: {
                    var fromUrl = ""
                    var toUrl = ""
                    var bookUrl = ""
                    if (recommendation && recommendation.fromUser) {
                        fromUrl = "<a href=\"" + recommendation.fromUser.webUrl + "\" \">" +
                                recommendation.fromUser.userName + "</a>"
                    }

                    if (recommendation && recommendation.toUser) {
                        toUrl = "<a href=\"" + recommendation.toUser.webUrl + "\" \">" +
                                recommendation.toUser.userName + "</a>"
                    }

                    if (recommendation && recommendation.book) {
                        bookUrl = "<a href=\"" + recommendation.book.link + "\" \">" +
                                recommendation.book.title + "</a>"
                    }
                    return qsTr ("%1 recommended the book %2 to %3")
                        .arg(fromUrl)
                        .arg(bookUrl)
                        .arg(toUrl)
                }
                postDate: recommendation ? Qt.formatDateTime(recommendation.createDate) : ""
                onUserClicked: pageStack.push(Qt.resolvedUrl("ProfilePage.qml"),
                        { userId: recommendation && recommendation.fromUser ? recommendation.fromUser.id : "" })
                onLinkActivated: mainWindow.openPageFromUrl(link)
            }

            ShortBookRow {
                width: parent.width
                bookImage: recommendation && recommendation.book ? recommendation.book.imageUrl : ""
                bookTitle: recommendation && recommendation.book ? recommendation.book.titleWithoutSeries : ""
                bookAuthors: recommendation && recommendation.book ?
                        Utils.getAuthorsString(recommendation.book.authors, Theme.primaryColor) : ""
                onBookClicked: pageStack.push(Qt.resolvedUrl("BookPage.qml"),
                        { bookId: recommendation && recommendation.book ? recommendation.book.id : "",
                            book : recommendation ? recommendation.book : null })
                onAuthorLinkActivated: pageStack.push(Qt.resolvedUrl("AuthorPage.qml"),
                        { authorId : link })
            }

            Item {
                width: parent.width
                height: likeButton.height
                IconText {
                    id: voteIcon

                    anchors {
                        left: parent.left
                        baseline: parent.verticalCenter
                    }
                    label.text: recommendation ? recommendation.likesCount : 0
                    label.color: Theme.highlightColor
                    label.font.pixelSize: Theme.fontSizeExtraSmall
                    icon.source: "image://theme/icon-s-like?" + Theme.highlightColor
                }

                IconText {
                    id: commentsIcon

                    anchors {
                        left: voteIcon.right
                        leftMargin: Theme.paddingLarge
                        baseline: parent.verticalCenter
                    }

                    label.text: recommendation ? recommendation.commentsCount : 0
                    label.color: Theme.highlightColor
                    label.font.pixelSize: Theme.fontSizeExtraSmall
                    icon.source: "image://theme/icon-s-chat?" + Theme.highlightColor
                }

                IconButton {
                    id: likeButton
                    anchors {
                        right: parent.right
                        bottom: parent.bottom
                    }

                    icon.source: "image://theme/icon-m-like?" +
                            (pressed || (recommendation && recommendation.isLiked) ?
                                Theme.highlightColor :
                                Theme.primaryColor)
                    onClicked: {
                        //TODO like review
                    }
                }
            }

            SectionHeader {
                text: qsTr("Comments")
            }
        }
    }

    BusyIndicator {
        size: BusyIndicatorSize.Large
        anchors.centerIn: parent
        running: recommendationPage.busy
        visible: running
    }
}
