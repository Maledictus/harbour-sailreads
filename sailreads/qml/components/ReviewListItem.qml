import QtQuick 2.0
import Sailfish.Silica 1.0
import harbour.sailreads 1.0

ListItem {
    id: listItem

    property int userId
    property alias userAvatarUrl: avatatImage.source
    property string userName
    property int reviewId
    property alias reviewRate: ratingBox.rating
    property string reviewText
    property var reviewDate
    property bool withBody: true

    signal userClicked(var userId)
    signal reviewClicked(var reviewId)

    contentHeight: column.height
    clip: true

    Column {
        id: column
        anchors {
            left: parent.left
            leftMargin: Theme.horizontalPageMargin
            right: parent.right
            rightMargin: Theme.horizontalPageMargin
        }

        Item {
            id: headerRow
            width: parent.width
            height: Math.max(avatatImage.height, reviewHeader.height) + Theme.paddingMedium
            BaseImage {
                id: avatatImage
                anchors {
                    left: parent.left
                    verticalCenter: parent.verticalCenter
                }
                fillMode: Image.PreserveAspectFit
                indicator.size: BusyIndicatorSize.Medium
                onClicked: userClicked(userId)
            }

            Column {
                id: reviewHeader
                anchors {
                    left: avatatImage.right
                    leftMargin: Theme.paddingMedium
                    right: hasCommentImage.left
                    rightMargin: Theme.paddingMedium
                }

                Row {
                    spacing: Theme.paddingMedium
                    ClickableLabel {
                        id: nameLabel
                        width: label.implicitWidth
                        label.text: userName
                        label.font.pixelSize: Theme.fontSizeExtraSmall
                        onClicked: userClicked(userId)
                    }

                    Label {
                        text: qsTr("rated it")
                        font.pixelSize: Theme.fontSizeExtraSmall
                        color: Theme.highlightColor
                    }
                }

                RatingBox {
                    id: ratingBox
                    iconWidth: Theme.iconSizeExtraSmall
                    iconHeight: Theme.iconSizeExtraSmall
                    color: Theme.highlightColor
                }

                Label {
                    id: dateLabel
                    visible: text !== ""
                    text: Qt.formatDateTime(reviewDate)
                    font.pixelSize: Theme.fontSizeTiny
                    color: Theme.highlightColor
                }
            }

            Image {
                id: hasCommentImage
                anchors {
                    right: parent.right
                    verticalCenter: reviewHeader.verticalCenter
                }
                visible: !withBody && reviewText !== ""
                source: "image://theme/icon-m-note" +
                        (highlighted ? "?" + Theme.highlightColor : "")
            }
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
            onLinkActivated: Qt.openUrlExternally(link)
        }
    }
}
