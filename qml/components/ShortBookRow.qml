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

import QtQuick 2.6
import Sailfish.Silica 1.0
import harbour.sailreads 1.0

import "../components"

Row {
    id: shortBookRow

    property alias bookImage: bookImageItem.source
    property alias bookTitle: bookTitleLabel.text
    property alias bookAuthors: authorsLabel.text
    property real bookAverageRating
    property int bookRatingsCount

    signal bookClicked()
    signal authorLinkActivated(var link)

    width: parent ? parent.width : Screen.width
    spacing: Theme.paddingMedium

    BaseImage {
        id: bookImageItem
        height: Theme.coverSizeSmall.height
        width: Theme.coverSizeSmall.width
        indicator.size: BusyIndicatorSize.Medium
        onClicked: bookClicked()
    }

    Column {
        id: column
        width: parent.width - bookImageItem.width - 2 * Theme.paddingMedium
        ClickableLabel {
            id: bookTitleLabel
            width: parent.width
            label.font.family: Theme.fontFamilyHeading
            label.font.pixelSize: Theme.fontSizeMedium
            label.wrapMode: Text.WordWrap
            onClicked: bookClicked()
        }

        Label {
            width: parent.width
            id: authorsLabel
            textFormat: Text.RichText
            wrapMode: Text.WordWrap
            font.pixelSize: Theme.fontSizeSmall
            color: Theme.highlightColor
            onLinkActivated: authorLinkActivated(link)
        }

        RatingBox {
            id: ratingBox
            rating: bookAverageRating
            color: Theme.highlightColor
            starIconSize: Theme.iconSizeSmall
        }

        KeyValueLabel {
            id: averageRatingLabel
            key: qsTr("Average rating")
            visible: value !== ""
            value: Number(bookAverageRating).toLocaleString()
        }

        KeyValueLabel {
            id: ratingsCountLabel
            key: qsTr("Ratings count")
            visible: value !== ""
            value: "%L1".arg(bookRatingsCount)
        }
    }
}
