import QtQuick

Rectangle {
    id: weatherWidget
    width: 350
    height: 200
    color: "#2E3B4E"
    radius: 10

    Column {
        anchors.centerIn: parent
        spacing: 10

        Row {
            spacing: 10
            anchors.horizontalCenter: parent.horizontalCenter

            // Icon
            Image {
                id: weatherIcon
                source: todayWeatherData.iconUrl
                width: 64
                height: 64
                fillMode: Image.PreserveAspectFit
            }

            // Temperature and Condition
            Column {
                spacing: 5

                Text {
                    id: temperatureText
                    text: todayWeatherData.temperature
                    font.pixelSize: 32
                    color: "lightblue"
                }
                Text {
                    id: conditionText
                    text: todayWeatherData.condition
                    font.pixelSize: 24
                    color: "white"
                }
            }
        }
    }
}
