import QtQuick 2.0
import QtQuick.Controls 1.1
import QtQuick.Layouts 1.1
import ScreenInfo 1.0

Rectangle {
    id: displaySettingsRoot
    property ScreenInfo screenInfo
    property list<Item> screenComponents
    property alias linearAttenuation: linearAttenuationSlider
    property alias fullScreen: fullscreenCheckBox.checked
    property bool stretchedLeftRight: false
    property size viewportSize: Qt.size(1920, 1080)
    property rect mainGeometry: Qt.rect(0,0,640,480)
    property rect oculusGeometry: Qt.rect(640,0,320,240)
    property int topMost
    property int bottomMost
    property int leftMost
    property int rightMost
    property bool applyableAsFullScreen
    property bool applyableAsStretched
    property bool mainVisible: true
    property bool oculusVisible: true
    signal apply
    signal close

    Keys.onPressed:  {
        switch(event.key) {
        case Qt.Key_Escape:
            displaySettingsRoot.visible = false
            break
        }
    }

    onStretchedLeftRightChanged: {
        console.log(stretchedLeftRight)
    }

    function layoutScreens() {
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
            screenObject.screen = screen
            screenObject.x = screenArea.width * 0.1 + screen.geometry.x * scale
            screenObject.y = screenArea.height * 0.1 + screen.geometry.y * scale
            screenObject.width = screen.geometry.width * scale
            screenObject.height = screen.geometry.height * scale
            startX += screenObject.width + 10
            screenComponentsTmp.push(screenObject)
        }
        screenComponents = screenComponentsTmp
    }

    function disableUseAsOculusOnAll() {
        for(var i in screenComponents) {
            screenComponents[i].useAsOculus = false
        }
    }

    function checkApplyability() {
        var applyableTmp = false
        var applyableStretchedTmp = false
        for(var i in screenComponents) {
            if(screenComponents[i].use) {
                applyableTmp = true
                if(screenComponents[i].useAsOculus) {
                    applyableStretchedTmp = true
                }
            }
        }
        applyableAsFullScreen = applyableTmp
        applyableAsStretched = applyableStretchedTmp
    }

    function updateGeometry() {
        if(!fullScreen) {
            mainGeometry.x = 0
            mainGeometry.y = 0
            mainGeometry.width = parent.width * 0.6
            mainGeometry.height = parent.height
            oculusGeometry.x = mainGeometry.width
            oculusGeometry.y = 0
            oculusGeometry.width = parent.width - mainGeometry.width
            oculusGeometry.height = oculusGeometry.width * (mainGeometry.height / mainGeometry.width)
            mainVisible = true
            oculusVisible = true
        } else {
            var oculusMinX = 999999
            var oculusMinY = 999999
            var oculusMaxX = 0
            var oculusMaxY = 0
            var mainMinX = 999999
            var mainMinY = 999999
            var mainMaxX = 0
            var mainMaxY = 0
            leftMost = 0;
            rightMost = 0;
            topMost = 0;
            bottomMost = 0;
            var leftMostPixel = 999999;
            var rightMostPixel = -999999;
            var topMostPixel = 999999;
            var bottomMostPixel = -999999;
            var oculusInUse = false
            var mainInUse = false
            for(var i in screenComponents) {
                var screen = screenComponents[i].screen
                var geometry = screen.geometry
                var screenID = screen.id
                if(screenComponents[i].use) {
                    var screenLeftMostPixel = geometry.x;
                    var screenRightMostPixel = geometry.x + geometry.width;
                    var screenTopMostPixel = geometry.y;
                    var screenBottomMostPixel = geometry.y + geometry.height;
                    if(screenLeftMostPixel < leftMostPixel) {
                        leftMostPixel = screenLeftMostPixel;
                        leftMost = screenID;
                    }
                    if(screenRightMostPixel > rightMostPixel) {
                        rightMostPixel = screenRightMostPixel;
                        rightMost = screenID;
                    }
                    if(screenTopMostPixel < topMostPixel) {
                        topMostPixel = screenTopMostPixel;
                        topMost = screenID;
                    }
                    if(screenBottomMostPixel > bottomMostPixel) {
                        bottomMostPixel = screenBottomMostPixel;
                        bottomMost = screenID;
                    }
                    if(screenComponents[i].useAsOculus) {
                        oculusInUse = true
                        oculusMinX = Math.min(oculusMinX, geometry.x)
                        oculusMinY = Math.min(oculusMinY, geometry.y)
                        oculusMaxX = Math.max(oculusMaxX, geometry.x + geometry.width)
                        oculusMaxY = Math.max(oculusMaxY, geometry.y + geometry.height)
                    } else {
                        mainInUse = true
                        mainMinX = Math.min(mainMinX, geometry.x)
                        mainMinY = Math.min(mainMinY, geometry.y)
                        mainMaxX = Math.max(mainMaxX, geometry.x + geometry.width)
                        mainMaxY = Math.max(mainMaxY, geometry.y + geometry.height)
                    }
                }
            }
            if(oculusInUse && mainInUse) {
                if(oculusMinX < mainMinX) {
                    oculusGeometry.x = 0
                    mainGeometry.x = mainMinX - oculusMinX
                } else {
                    mainGeometry.x = 0
                    oculusGeometry.x = oculusMinX - mainMinX
                }
                if(oculusMinY < mainMinY) {
                    oculusGeometry.y = 0
                    mainGeometry.y = mainMinY - oculusMinY
                } else {
                    mainGeometry.y = 0
                    oculusGeometry.y = oculusMinY - mainMinY
                }
            } else {
                oculusGeometry.x = 0
                oculusGeometry.y = 0
                mainGeometry.x = 0
                mainGeometry.y = 0
            }

            oculusGeometry.width = oculusMaxX - oculusMinX
            oculusGeometry.height = oculusMaxY - oculusMinY
            mainGeometry.width = mainMaxX - mainMinX
            mainGeometry.height = mainMaxY - mainMinY

            mainVisible = mainInUse
            oculusVisible = oculusInUse

            if(!customSourceSizeCheckBox.checked) {
                widthTextField.text = mainGeometry.width
                heightTextField.text = mainGeometry.height
            }
        }
        if(oculusVisible) {
            stretchedLeftRight = stretchedLeftRightCheckBox.checked
        } else {
            stretchedLeftRight = true
        }
        viewportSize.width = parseInt(widthTextField.text)
        viewportSize.height = parseInt(heightTextField.text)
    }

    width: 640
    height: 480
    color: "#dfe3ee"

    MouseArea {
        id: dummyArea
        anchors.fill: parent
        onClicked: {

        }
    }

    CheckBox {
        id: fullscreenCheckBox
        text: "Fullscreen/multi-monitor"
        checked: false
        anchors {
            bottom: screenArea.top
            left: screenArea.left
            margins: parent.width * 0.01
        }
    }

    Rectangle {
        id: screenArea
        enabled: fullscreenCheckBox.checked
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: parent.top
        anchors.margins: parent.width * 0.1
        anchors.bottom: controls.top
        anchors.bottomMargin: parent.width * 0.02
        width: parent.width * 0.8
        color: "#8b9dc3"
        Rectangle {
            anchors.fill: parent
            visible: !screenArea.enabled
            color: "white"
            opacity: 0.8
            z: 99
        }
    }

    ColumnLayout {
        id: controls
        anchors {
            horizontalCenter: parent.horizontalCenter
            bottom: parent.bottom
            margins: parent.width * 0.02
        }
        spacing: parent.width * 0.02
        width: parent.width * 0.7
        Text {
            id: infoText
            color: screenArea.enabled ? "black" : "grey"
            width: parent.width
            text: "Select the displays you want to use. " +
                  "Click the circle to set it as the Oculus Rift display."
            wrapMode: Text.WrapAtWordBoundaryOrAnywhere
            horizontalAlignment: Text.AlignHCenter
        }

        CheckBox {
            id: stretchedLeftRightCheckBox
            enabled: applyableAsStretched
            anchors.horizontalCenter: parent.horizontalCenter
            text: "Stretched 3D mode"
        }


        RowLayout {
            anchors.horizontalCenter: parent.horizontalCenter
            Layout.fillWidth: true
            Label {
                text: "Light falloff:"
            }
            Slider {
                id: linearAttenuationSlider
                minimumValue: 0.0001
                maximumValue: 0.1
                value: 0.01
                Layout.fillWidth: true
            }
        }

        Row {
            id: sizeRow
            anchors.horizontalCenter: parent.horizontalCenter
            Label {
                text: "Source size: "
            }
            CheckBox {
                id: customSourceSizeCheckBox
                text: "Custom"
            }
            TextField {
                id: widthTextField
                text: "1920"
                enabled: customSourceSizeCheckBox.checked
            }
            TextField {
                id: heightTextField
                text: "1080"
                enabled: customSourceSizeCheckBox.checked
            }
        }

        Row {
            anchors.horizontalCenter: parent.horizontalCenter
            Button {
                text: "Apply"
                enabled: applyableAsFullScreen || !fullScreen
                onClicked: {
                    updateGeometry()
                    displaySettingsRoot.apply()
                }
            }

            Button {
                text: "Close"
                onClicked: {
                    displaySettingsRoot.close()
                }
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
            property ScreenInfoScreen screen
            width: 100
            height: 60
            color: "#3b5998"
            border {
                color: "#dfe3ee"
                width: parent.width * 0.01
            }
            Rectangle {
                color: screenRect.use ? "#dfe3ee" : "#3b5998"
                border.width: 1
                border.color: "#f7f7f7"
                anchors.fill: parent
                anchors.margins: parent.width * 0.1
            }

            MouseArea {
                anchors.fill: parent
                onClicked: {
                    screenRect.use = !screenRect.use
                    checkApplyability()
                }
            }
            Rectangle {
                id: useAsOculusToggleButton
                width: screenRect.width * 0.1
                height: width
                radius: width / 2
                visible: screenRect.use
                color: screenRect.useAsOculus ? "#f7f7f7" : "#8b9dc3"
                anchors {
                    right: parent.right
                    bottom: parent.bottom
                    margins: parent.width * 0.05
                }
                border {
                    color: screenRect.useAsOculus ? "#8b9dc3" : "#f7f7f7"
                    width: parent.width * 0.01
                }

                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        var toBeUsed = !screenRect.useAsOculus
                        displaySettingsRoot.disableUseAsOculusOnAll()
                        screenRect.useAsOculus = toBeUsed
                        checkApplyability()
                    }
                }
            }
        }
    }
}
