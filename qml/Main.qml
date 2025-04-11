import QtQuick
import "widgets"

Window {
    width: 800
    height: 480
    visible: true
    title: qsTr("Dashboard")

    // WeatherWidget.qml to instantiate the WeatherWidget component
    WeatherWidget {
        anchors.centerIn: parent

        MouseArea {
            anchors.fill: parent
            onClicked: {
                weatherModalLoader.source = "modals/WeatherModal.qml"
                weatherModalLoader.visible = true
            }
        }
    }

    Loader {
        id: weatherModalLoader
        anchors.fill: parent
        visible: false

        onLoaded: {
            if (item && item.closeRequested) {
                item.closeRequested.connect(function() {
                    weatherModalLoader.visible = false
                });
            }
        }
    }
}
