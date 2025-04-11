import QtQuick

Item {
    id: button
    width: 40
    height: 40

    signal clicked()

    Rectangle {
        id: circle
        anchors.fill: parent
        color: "#ff4444"
        radius: width / 2
        border.color: "black"
        border.width: 1

        MouseArea {
            anchors.fill: parent
            onClicked: button.clicked()
            hoverEnabled: true
        }
    }

    Text {
        text: "X"
        anchors.centerIn: parent
        color: "white"
        font.pixelSize: 20
        font.bold: true
    }
}
