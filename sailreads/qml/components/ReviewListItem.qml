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

    signal userClicked(var userId)
    signal reviewClicked(var reviewId)

    contentHeight: column.height + separator.height + Theme.paddingMedium
    clip: true

    Column {
        id: column

        anchors {
            left: parent.left
            leftMargin: Theme.horizontalPageMargin
            right: parent.right
            rightMargin: Theme.horizontalPageMargin
            top: parent.top
            topMargin: Theme.paddingSmall
        }

        Row {
            id: headerRow
            spacing: Theme.paddingMedium
            width: parent.width

            BaseImage {
                id: avatatImage
                height: width
                width: Theme.iconSizeMedium
                indicator.size: BusyIndicatorSize.Medium
                anchors.verticalCenter: reviewHeader.verticalCenter
                onClicked: userClicked(userId)
            }

            Column {
                id: reviewHeader
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
        }

        ClickableLabel {
            id: bodyLabel
            label.text: reviewText
            label.wrapMode: Text.WordWrap
            label.maximumLineCount: 4
            label.font.pixelSize: Theme.fontSizeSmall
            label.elide: Text.ElideRight
            visible: label.text !== ""
            width: parent.width
            onClicked: reviewClicked(reviewId)
            onLinkActivated: Qt.openUrlExternally(link)
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
