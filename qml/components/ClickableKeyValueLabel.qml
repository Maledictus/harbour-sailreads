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

Row {
    id: row
    property string key
    property string value
    property color keyColor: Theme.secondaryHighlightColor
    property bool highlighted: false

    signal clicked()
    signal linkActivated(var link)

    width: parent ? parent.width : Screen.Width
    height: Math.max(keyLabel.height, valueLabel.height)
    spacing: Theme.paddingSmall

    Label {
        id: keyLabel
        text: key
        color: keyColor
        wrapMode: Text.NoWrap
        font.pixelSize: Theme.fontSizeExtraSmall
        textFormat: Text.StyledText
    }
    ClickableLabel {
        id: valueLabel
        text: value
        width: parent.width - keyLabel.width - row.spacing
        label.font.pixelSize: Theme.fontSizeExtraSmall
        onClicked: row.clicked()
        highlighted: row.highlighted || down
        onLinkActivated: row.linkActivated(link)
    }
}
