/*
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

import QtQuick 2.0
import Sailfish.Silica 1.0
import harbour.sailreads 1.0

Item {
    id: headerRow

    property alias avatarImage: avatarImageItem
    property alias nameLabel: nameLabelItem
    property alias ratingBox: ratingBoxItem
    property alias dateLabel: dateLabelItem
    property alias hasCommentImage: hasCommentImageItem
    property int userId

    property int headerFontSize: Theme.fontSizeExtraSmall
    property int bodyFontSize: Theme.fontSizeSmall
    property int ratingIconSize: Theme.iconSizeExtraSmall

    property bool highlighted: false

    signal userClicked(var userId)

    width: parent.width
    height: Math.max(avatarImageItem.height, reviewHeader.height) + Theme.paddingMedium
    BaseImage {
        id: avatarImageItem
        anchors {
            left: parent.left
            verticalCenter: parent.verticalCenter
        }
        indicator.size: BusyIndicatorSize.Medium
        onClicked: userClicked(userId)
    }

    Column {
        id: reviewHeader
        anchors {
            left: avatarImageItem.right
            leftMargin: Theme.paddingMedium
            right: hasCommentImageItem.left
            rightMargin: Theme.paddingMedium
        }

        Row {
            spacing: Theme.paddingMedium
            ClickableLabel {
                id: nameLabelItem
                width: label.implicitWidth
                label.font.pixelSize: headerFontSize
                onClicked: userClicked(userId)
            }

            Label {
                text: qsTr("rated it")
                font.pixelSize: headerFontSize
                color: Theme.highlightColor
            }
        }

        RatingBox {
            id: ratingBoxItem
            starIconSize: ratingIconSize
            color: Theme.highlightColor
        }

        Label {
            id: dateLabelItem
            visible: text !== ""
            font.pixelSize: headerFontSize
            color: Theme.highlightColor
        }
    }

    Image {
        id: hasCommentImageItem
        anchors {
            right: parent.right
            verticalCenter: reviewHeader.verticalCenter
        }
        source: "image://theme/icon-m-note" +
                (highlighted ? "?" + Theme.highlightColor : "")
    }
}
