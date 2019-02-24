import QtQuick 2.6
import Sailfish.Silica 1.0

Column {
    property alias title: label.text
    property alias model: repeater.model

    width: parent.width
    visible: model && model.count

    Label {
        id: label
        width: parent.width
        wrapMode: Text.WordWrap
        horizontalAlignment: Qt.AlignHCenter
        color: Theme.secondaryHighlightColor
        font.pixelSize: Theme.fontSizeSmall
    }

    Repeater {
        id: repeater

        delegate: Label {
            width: parent.width
            wrapMode: Text.WordWrap
            horizontalAlignment: Qt.AlignHCenter
            color: Theme.secondaryColor
            linkColor: Theme.primaryColor
            font.pixelSize: Theme.fontSizeSmall
            text: link ? "<a href='%0'>%1</a>".arg(link).arg(name) : name
            onLinkActivated: Qt.openUrlExternally(link)
        }
    }
}
