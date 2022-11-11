import QtQuick 2.15
import QtQuick.Window 2.15

import QtQuick.Controls 2.15

Window {
    width: 640
    height: 480
    visible: true

    property alias domainName: url.text
    property alias faviconImage: favicon.source

    signal download(string d)
    signal loadImage(string imagePath)

    onLoadImage: favicon.source = imagePath

    Rectangle {
        anchors.fill: parent

        TextInput {
            id: url

            anchors.top: parent.top
            anchors.left: parent.left
            anchors.right: parent.right

            focus: true

            height: 32
        }

        Button {
            id: downloadBtn

            anchors.top: url.bottom

            text: "Download"

            onClicked: download(url.text)
        }

        Image {
            id: favicon

            anchors.centerIn: parent
            fillMode: Image.PreserveAspectFit
        }
    }
}
