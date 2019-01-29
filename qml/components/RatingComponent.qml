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

import "../components"

Row {
    property real averageRating: 0.0
    property int ratingsCount: 0
    property color color: Theme.primaryColor
    property alias starIconSize: ratingBox.starIconSize

    spacing: Theme.paddingSmall
    height: childrenRect.height
    width: parent.width

    RatingBox {
        id: ratingBox
        rating: averageRating
        color: parent.color
    }

    Label {
        font.pixelSize: Theme.fontSizeExtraSmall
        color: parent.color
        text: qsTr("%1 /").arg(Number(averageRating).toLocaleString(Qt.locale(), 'f', 2))
    }

    Label {
        font.pixelSize: Theme.fontSizeExtraSmall
        color: parent.color
        text: qsTr("%Ln rating(s)", "", ratingsCount)
    }
}
