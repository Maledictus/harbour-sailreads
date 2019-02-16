import QtQuick 2.6

Image {
    property bool highlighted
    property string _highlightSource
    property url _imageSource

    source: highlighted ? _highlightSource : _imageSource
}
