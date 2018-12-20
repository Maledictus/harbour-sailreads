import QtQuick 2.0
import QtQuick.Layouts 1.1
import Sailfish.Silica 1.0
import harbour.sailreads 1.0

ListItem {
    id: listItem
    contentHeight: row.height + separator.height + Theme.paddingMedium
    clip: true

    Row {
        id: row
        spacing: Theme.paddingMedium
        height: Math.max(bookImage.height, column.height)
        anchors {
            left: parent.left
            leftMargin: Theme.horizontalPageMargin
            right: parent.right
            rightMargin: Theme.horizontalPageMargin
        }
        BaseImage {
            id: bookImage
            anchors {
                top: column.top
                topMargin: Theme.paddingSmall
            }

            source: bookImageUrl
            height: 1.5 * width
            width: Theme.iconSizeLarge
            horizontalAlignment: Image.AlignLeft
            verticalAlignment: Image.AlignTop
            indicator.size: BusyIndicatorSize.Medium
        }

        Column {
            id: column
            width: parent.width - bookImage.width - Theme.paddingMedium
            Label {
                id: bookNameLabel
                width: parent.width
                wrapMode: Text.WordWrap
                maximumLineCount: 2
                font.family: Theme.fontFamilyHeading
                font.pixelSize: Theme.fontSizeSmall
                text: bookTitle
            }

            Label {
                id: authorsLabe
                width: parent.width
                truncationMode: TruncationMode.Fade
                font.pixelSize: Theme.fontSizeTiny
                text: bookAuthors
            }

            Row {
                spacing: Theme.paddingSmall
                RatingBox {
                    rating: bookAverageRating
                }

                Label {
                    font.pixelSize: Theme.fontSizeExtraSmall
                    text: qsTr("%1/%2 ratings").arg(Number(bookAverageRating).toLocaleString())
                            .arg(Number(bookRatingsCount).toLocaleString(Qt.locale(), 'f', 0))
                }
            }
        }
    }

    Separator {
        id: separator
        anchors {
            top: row.bottom
            topMargin: Theme.paddingMedium
        }

        width: parent.width
        color: Theme.primaryColor
        horizontalAlignment: Qt.AlignHCenter
    }
}
