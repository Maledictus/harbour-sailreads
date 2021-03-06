import QtQuick 2.6
import Sailfish.Silica 1.0

import "../utils/Utils.js" as Utils

Item {
    id: listItem
    property var author
    property alias body: label.text
    property var updateDate

    signal userClicked(var userId)
    signal linkActivated(var link)

    width: parent.width
    height: column.height + separator.height + Theme.paddingMedium
    clip: true
    Column {
        id: column
        anchors {
            left: parent.left
            leftMargin: Theme.horizontalPageMargin
            right: parent.right
            rightMargin: Theme.horizontalPageMargin
        }

        PosterHeaderItem {
            width: parent.width
            posterName: author ? author.userName.toUpperCase() : ""
            postDate: Utils.generateDateString(updateDate, "dd.MM.yy hh:mm")
            posterAvatar: author ? author.avatarUrl : ""
            onClicked: userClicked(author.id)
        }

        Label {
            id: label
            width: parent.width
            wrapMode: Text.WordWrap
            textFormat: Text.StyledText
            font.pixelSize: Theme.fontSizeSmall
            linkColor: Theme.primaryColor
            color: Theme.highlightColor
            onLinkActivated: listItem.linkActivated(link)
        }
    }

    Separator {
        id: separator
        anchors {
            top: column.bottom
            topMargin: Theme.paddingMedium
        }

        width: parent.width
        color: Theme.primaryColor
        horizontalAlignment: Qt.AlignHCenter
    }
}
