import QtQuick 2.0
import Sailfish.Silica 1.0
import harbour.sailreads 1.0

ListItem {
    id: listItem

    property real contentMargin

    property int userId
    property url userAvatarUrl
    property string userName
    property int reviewId
    property real reviewRate
    property string reviewText
    property var reviewDate
    property string reviewShortDescription
    property bool withBody: true

    signal userClicked(var userId)
    signal linkActivated(var link)

    contentHeight: column.height
    clip: true

    Column {
        id: column
        anchors {
            left: parent.left
            leftMargin: contentMargin
            right: parent.right
            rightMargin: contentMargin
        }

        UserShortReview {
            id: headerRow
            width: parent.width

            userId: listItem.userId
            avatarImage.source: userAvatarUrl
            nameLabel.label.text: userName
            shortDescription: reviewShortDescription
            ratingBox.rating: reviewRate
            dateLabel.text: Qt.formatDateTime(reviewDate)
            hasCommentImage.visible: !withBody && reviewText !== ""

            headerFontSize: Theme.fontSizeExtraSmall
            ratingIconSize: Theme.iconSizeExtraSmall
            highlighted: listItem.highlighted
        }

        Label {
            id: bodyLabel
            text: reviewText
            wrapMode: Text.WordWrap
            font.pixelSize: Theme.fontSizeSmall
            color: Theme.primaryColor
            elide: Text.ElideRight
            visible: withBody && text !== ""
            linkColor: Theme.highlightColor
            truncationMode: TruncationMode.Fade
            width: parent.width
            onLinkActivated: listItem.linkActivated(link)
        }
    }
}
