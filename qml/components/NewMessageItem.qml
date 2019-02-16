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
import "../utils/Utils.js" as Utils

Item {
    height: Math.max(textArea.height, sendButton.height) + Theme.paddingMedium
    anchors.left: parent.left
    anchors.right: parent.right
    anchors.rightMargin: Theme.horizontalPageMargin

    signal sendMessage(var message)

    TextArea {
        id: textArea
        anchors.left: parent.left
        anchors.right: sendButton.left
        anchors.rightMargin: Theme.paddingMedium
        anchors.bottom: parent.bottom
        placeholderText: qsTr("Message")
        wrapMode: Text.WordWrap
    }

    ClickableLabel {
        id: sendButton
        width: label.implicitWidth
        label.text: qsTr("Send")
        anchors.right: parent.right
        anchors.verticalCenter: textArea.top
        anchors.verticalCenterOffset: textArea.textVerticalCenterOffset + (textArea._editor.height - height)
        enabled: textArea.text !== ""
        onClicked: {
            sendMessage(textArea.text)
            textArea.text = ""
        }
    }
}
