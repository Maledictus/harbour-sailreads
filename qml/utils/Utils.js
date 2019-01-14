/*
The MIT License (MIT)

Copyright (c) 2018 Oleg Linkin <maledictusdemagog@gmail.com>

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

.pragma library

function generateDateString(dt, format) {
    var today = new Date()
    var date = "";
    if (today.getDate() === dt.getDate() &&
            today.getMonth() === dt.getMonth() &&
            today.getFullYear() === dt.getFullYear()) {
        date = qsTr("Today") + ", "
    }
    else if (today.getDate() === dt.getDate() + 1 &&
            today.getMonth() === dt.getMonth() &&
            today.getFullYear() === dt.getFullYear()){
        date = qsTr("Yesterday") + ", "
    }

    if(date !== "") {
        return date + " " + Qt.formatTime(dt, "hh:mm")
    }

    return Qt.formatDateTime(dt, format)
}

function getDetailsInfoString(age, gender, location) {
    var result = ""
    if (age !== 0) {
        result += age
    }

    if (gender !== "") {
        if (result !==  "") {
            result += ", "
        }

        result += gender
    }

    if (location !== "") {
        if (result !==  "") {
            result += ", "
        }

        result += location
    }

    return result
}

function getAuthorsString(authorsList, color) {
    if (authorsList.length === 0) {
        return "";
    }

    var result = qsTr("<style>a:link{color:" + color + ";}</style>")
    for (var i = 0; i < authorsList.length; ++i) {
        result += "<a href=\"%1\" style=\"text-decoration:none;\">%2</a>"
                .arg(Number(authorsList[i].id).toFixed())
                .arg(authorsList[i].name)
        if (i +1 < authorsList.length) {
            result += ", "
        }
    }
    return result
}

function getSeriesString(seriesList, color) {
    if (seriesList.length === 0) {
        return "";
    }

    var result = qsTr("<style>a:link{color:" + color + ";}</style>")
    for (var i = 0; i < seriesList.length; ++i) {
        result += "<a href=\"%1\" style=\"text-decoration:none;\">%2</a>"
                .arg(Number(seriesList[i].id).toFixed())
                .arg(seriesList[i].title)
        if (i +1 < seriesList.length) {
            result += ", "
        }
    }
    return result
}

function humanReadableSortField(sortField) {
    switch (sortField) {
    case "title": return qsTr("Title")
    case "author": return qsTr("Author")
    case "avg_rating": return qsTr("Average rating")
    case "num_ratings": return qsTr("Number of ratings")
    case "date_pub": return qsTr("First published date")
    case "num_pages": return qsTr("Pages count")
    case "date_updated": return qsTr("Date updated")
    case "rating": return qsTr("Your rating")
    case "date_started": return qsTr("Date started")
    case "date_read": return qsTr("Date finished")
    case "review": return qsTr("Review written")
    case "date_added":
    default:
        return qsTr("Date added")
    }
}
