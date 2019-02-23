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
    id: usersReviewsPage

    property var usersReviews: []
    property bool busy: sailreadsManager.busy && usersReviewsPage.status === PageStatus.Active

    function attachPage() {
        if (pageStack._currentContainer.attachedContainer === null
                && sailreadsManager.logged) {
            pageStack.pushAttached(Qt.resolvedUrl("StatusPage.qml"))
        }
    }

    onStatusChanged: {
        if (status === PageStatus.Active) {
            attachPage()
        }
    }

    SilicaListView {
        id: usersReviewsView

        anchors.fill: parent
        clip: true

        header: PageHeader {
            title: qsTr("Friends reviews")
        }

        width: parent.width
        height: contentHeight
        spacing: Theme.paddingMedium

        delegate: ReviewListItem {
            userId: modelData.user.id
            contentMargin: Theme.horizontalPageMargin
            userAvatarUrl: modelData.user.avatarUrl
            userName: modelData.user.userName
            reviewId: modelData.id
            reviewRate: modelData.rating
            reviewText: modelData.body
            reviewDate: modelData.updatedDate
            reviewShortDescription: modelData.shortDescription

            onUserClicked: {
                pageStack.push(Qt.resolvedUrl("ProfilePage.qml"),
                        { userId: userId })
            }
            onLinkActivated: mainWindow.openPageFromUrl(link)

            onClicked: {
                pageStack.push(Qt.resolvedUrl("ReviewPage.qml"),
                        { reviewId: modelData.id, review: modelData })
            }

            menu: ContextMenu {
                MenuItem {
                    text: qsTr("Open on GoodReads.com")
                    onClicked: mainWindow.openInBrowser(modelData.url)
                }
            }
        }

        model: usersReviews
    }
}
