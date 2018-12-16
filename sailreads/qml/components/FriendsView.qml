import QtQuick 2.0
import Sailfish.Silica 1.0
import harbour.sailreads 1.0

SilicaListView {
    id: view
    width: parent.width
    height: parent.height
    clip: true

    signal friendItemClicked(var userId)

    delegate: ListItem {
        id: rootDelegateItem
        width: view.width
        contentHeight: row.height + separator.height +
                Theme.paddingMedium
        clip: true

        Row {
            id: row
            spacing: Theme.paddingMedium
            height: Math.max(friendIconImage.height, column.height)
            anchors {
                left: parent.left
                leftMargin: Theme.horizontalPageMargin
                right: parent.right
                rightMargin: Theme.horizontalPageMargin
            }
            BaseImage {
                id: friendIconImage
                anchors {
                    top: column.top
                    topMargin: Theme.paddingSmall
                }
                height: Theme.iconSizeLarge
                width: Theme.iconSizeMedium
                source: friendAvatarUrl
                fillMode: Image.PreserveAspectFit
                horizontalAlignment: Image.AlignLeft
                verticalAlignment: Image.AlignTop
            }

            Column {
                id: column
                width: parent.width - friendIconImage.width -
                        Theme.paddingMedium
                Label {
                    font.family: Theme.fontFamilyHeading
                    truncationMode: TruncationMode.Fade
                    text: friendName
                }
                Label {
                    truncationMode: TruncationMode.Fade
                    font.pixelSize: Theme.fontSizeExtraSmall
                    text: qsTr("%1 books").arg(friendBooksCount)
                }
                Label {
                    truncationMode: TruncationMode.Fade
                    font.pixelSize: Theme.fontSizeExtraSmall
                    text: qsTr("%1 friends").arg(friendFriendsCount)
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

//        RemorseItem { id: remorse }

//        menu: ContextMenu {
//            hasContent: userId === sailreadsManager.authUser.id
//            MenuItem {
//                text: qsTr("Remove")
//                onClicked: {
//                    remorse.execute(rootDelegateItem, qsTr("Removing"), function() {
//                        sailreadsManager.removeFriend(friendId)
//                    })
//                }
//            }
//        }

        onClicked: {
            friendItemClicked(friendId)
//            pageStack.push(Qt.resolvedUrl("ProfilePage.qml"),
//                    { userId: friendId })
        }
    }

    VerticalScrollDecorator {}
}
