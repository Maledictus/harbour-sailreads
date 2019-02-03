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

Row {
    property int rating: 0

    signal userVoted(int userVote)

    Repeater {
        id: ratingBox
        model: 5
        IconButton {
            readonly property int _vote: (index + 1)
            readonly property bool _selected: _vote <= rating
            width: Theme.iconSizeMedium
            height: Theme.iconSizeMedium
            icon.source: _selected ?
                    ("image://theme/icon-m-favorite-selected?" +
                            (pressed ? Theme.highlightColor : Theme.primaryColor)) :
                    ("image://theme/icon-m-favorite?" +
                            (pressed ? Theme.highlightColor : Theme.primaryColor))
            onClicked: {
                if (rating !== _vote) {
                    rating = _vote
                }
                else {
                    --rating
                }
                userVoted(rating)
            }
        }
    }
}
