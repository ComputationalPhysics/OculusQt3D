import QtQuick 2.0
import QtQuick.Controls 1.1
import ScreenInfo 1.0

Rectangle {
    id: displaySettingsRoot
    property ScreenInfo screenInfo
    property list<Item> screenComponents
    property Item selectedScreen
    width: 640
    height: 480

    function refresh() {
        console.log("Refreshing " + screenInfo.screens.length)
        var screenComponentsTmp = []
        var startX = 0
        var maxX = 0
        var maxY = 0
        for(var i in screenInfo.screens) {
            var screen = screenInfo.screens[i]
            maxX = Math.max(maxX, screen.geometry.x + screen.geometry.width)
            maxY = Math.max(maxY, screen.geometry.y + screen.geometry.height)
        }

        for(var i in screenInfo.screens) {
            var screen = screenInfo.screens[i]
            var xScale = screenArea.width / maxX * 0.8
            var yScale = screenArea.height / maxY * 0.8
            var scale = Math.min(xScale, yScale)
            var screenObject = screenRectComponent.createObject(screenArea)
            screenObject.x = screenArea.width * 0.1 + screen.geometry.x * scale
            screenObject.y = screenArea.height * 0.1 + screen.geometry.y * scale
            screenObject.width = screen.geometry.width * scale
            screenObject.height = screen.geometry.height * scale
            startX += screenObject.width + 10
            screenComponentsTmp.push(screenObject)
        }
        screenComponents = screenComponentsTmp
    }

    function deselectAll() {
        for(var i in screenComponents) {
            screenComponents[i].selected = false
            selectedScreen = null
        }
    }

    function toggleSelected() {
        for(var i in screenComponents) {
            if(screenComponents[i].selected) {
                screenComponents[i].use = !screenComponents[i].use
                if(!screenComponents[i].use) {
                    screenComponents[i].useAsOculus = false
                }
            }
        }
    }

    function toggleSelectedAsOculus() {
        for(var i in screenComponents) {
            if(screenComponents[i].selected) {
                screenComponents[i].useAsOculus = !screenComponents[i].useAsOculus
                if(!screenComponents[i].useAsOculus) {
                    screenComponets[i].use = true
                }
            }
        }
    }

    MouseArea {
        id: dummyArea
        anchors.fill: parent
        onClicked: {

        }
    }

    Rectangle {
        id: screenArea
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: parent.top
        anchors.margins: parent.width * 0.1
        width: parent.width * 0.8
        height: parent.height * 0.7
        color: "grey"
    }

    Row {
        anchors.top: screenArea.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        height: useAsOculusButton.height
        width: screenArea.width
        Button {
            id: useAsOculusButton
            enabled: selectedScreen
            text: (selectedScreen && selectedScreen.useAsOculus) ? "Enable as Oculus display" : "Disable as Oculus display"
            onClicked: {
                toggleSelectedAsOculus()
            }
        }

        Button {
            enabled: selectedScreen
            text: (selectedScreen && selectedScreen.use) ? "Disable" : "Enable"
            onClicked: {
                toggleSelected()
            }
        }
    }

    Component {
        id: screenRectComponent
        Rectangle {
            id: screenRect
            property bool selected: false
            property bool use: false
            property bool useAsOculus: false
            width: 100
            height: 60
            color: selected ? "cyan" : "blue"
            Rectangle {
                width: screenRect.width * 0.1
                height: width
                radius: width / 2
                color: screenRect.use ? "green" : "red"
                anchors {
                    right: parent.right
                    bottom: parent.bottom
                    margins: parent.width * 0.05
                }
            }

            MouseArea {
                anchors.fill: parent
                onClicked: {
                    displaySettingsRoot.deselectAll()
                    screenRect.selected = true
                    displaySettingsRoot.selectedScreen = screenRect
                }
            }
        }
    }
}
