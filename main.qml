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
    signal faviconDownloadFailed()

    onLoadImage:
        imagePath => favicon.source = imagePath

    onFaviconDownloadFailed: downloadBtn.enabled = true;

    Rectangle {
        anchors.fill: parent
        color: "grey"

        TextField {
            id: url

            anchors.top: parent.top
            anchors.left: parent.left
            anchors.right: parent.right

            focus: true

            color: "black"

            placeholderText: "Enter text"
            background: Rectangle {
                radius: 5
                color: "white"
                implicitHeight: 32
                border.color: "#333"
                border.width: 1
            }
        }

        Button {
            id: downloadBtn

            anchors.top: url.bottom

            text: "Download"

            onClicked: {
//                enabled = false;
                download(url.text);
            }
        }

        Image {
            id: favicon

            anchors.centerIn: parent
            fillMode: Image.PreserveAspectFit

            smooth: true
        }
    }
}
