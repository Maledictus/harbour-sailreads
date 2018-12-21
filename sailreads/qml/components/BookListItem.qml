import QtQuick 2.0
import QtQuick.Layouts 1.1
import Sailfish.Silica 1.0
import harbour.sailreads 1.0

ListItem {
    id: listItem

    property alias imageUrl: bookImage.source
    property alias title: bookNameLabel.text
    property alias authors: authorsLabel.text
    property real averageRating: 0.0
    property real ratingsCount: 0.0

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
            }

            Label {
                id: authorsLabel
                width: parent.width
                truncationMode: TruncationMode.Fade
                font.pixelSize: Theme.fontSizeTiny
                text: listItem.bookAuthors
            }

            Row {
                spacing: Theme.paddingSmall
                RatingBox {
                    id: ratingBox
                    rating: listItem.averageRating
                }

                Label {
                    font.pixelSize: Theme.fontSizeExtraSmall
                    text: qsTr("%1/%2 ratings").arg(Number(listItem.averageRating).toLocaleString())
                            .arg(Number(listItem.ratingsCount).toLocaleString(Qt.locale(), 'f', 0))
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
