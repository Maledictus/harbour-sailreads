import QtQuick 2.0
import Sailfish.Silica 1.0

Item {
    default property alias children: container.children

    height: Theme.itemSizeMedium
    width: parent.width
    Row {
        id: container

        anchors {
            verticalCenter: parent.verticalCenter
            left: parent.left
            leftMargin: Theme.horizontalPageMargin
            right: parent.right
            rightMargin: Theme.horizontalPageMargin
        }
    }
}
