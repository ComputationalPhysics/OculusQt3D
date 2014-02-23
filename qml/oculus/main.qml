import QtQuick 2.0
import Qt3D 2.0
import Qt3D.Shapes 2.0
import QtQuick.Controls 1.1
import QtQuick.Layouts 1.1
import QtQuick.Dialogs 1.1
import StereoViewport 1.0
import OculusReader 1.0
import MDStateManager 1.0
import Settings 1.0
import QtQuick.Window 2.0

import CompPhys.MultiBillboard 1.0
import CompPhys.FlyModeNavigator 1.0

import ScreenInfo 1.0

Rectangle {
    id: rectRoot
    property real pixelScale: 1.0
    property int mainScreenWidth: displaySettings.mainGeometry.width
    property int mainScreenHeight: displaySettings.mainGeometry.height
    property int oculusScreenWidth: displaySettings.oculusGeometry.width
    property int oculusScreenHeight: displaySettings.oculusGeometry.height
    property int totalScreenWidth: mainScreenWidth + mainScreenHeight
    width: 1280
    height: 720

    Component.onCompleted: {
        rectRoot.forceActiveFocus()
        console.log("width, height:")
    }

    ScreenInfo {
        id: screenInfo
        Component.onCompleted: {
            displaySettings.layoutScreens()
        }
        fullScreen: displaySettings.fullScreen
        topMost: displaySettings.topMost
        leftMost: displaySettings.leftMost
        rightMost: displaySettings.rightMost
        bottomMost: displaySettings.bottomMost
    }

    Settings {
        id: settings
    }

    OculusReader {
        id: oculusReader
        enabled: false
        camera: viewportRoot.camera
    }

    StereoViewport {
        id: viewportRoot
        fillColor: "black"
        width: displaySettings.viewportSize.width
        height: displaySettings.viewportSize.height
        stereoType: StereoViewport.StretchedLeftRight
        fovzoom: false

        Sphere {
            effect: Effect {
                color: "yellow"
            }
        }

        Sphere {
            x: -1
            y: -1
            z: -1
            effect: Effect {
                color: "red"
            }
        }

        light: Light {
            ambientColor: Qt.rgba(1,1,1,1)
            position: camera.eye
            quadraticAttenuation: 0.0001
        }

        camera: Camera {
            id: camera
            nearPlane: 5.0
            farPlane: 500
            fieldOfView: 60
            center: Qt.vector3d(1,0,0)
            eye: Qt.vector3d(5,0,0)
            eyeSeparation: 0.06
//            eyeSeparation: 0.0
        }

        MDStateManager {
            id: stateManager
            onCurrentTimestepChanged: viewportRoot.update()
        }

        MultiBillboard {
            id: multiSphere
            dataSource: stateManager

            effect: Effect {
                texture: "sphere2.png"
                blending: true
                useLighting: true
            }
        }
    }

    ShaderEffectSource {
        id: viewportClone
        visible: displaySettings.mainVisible
        property bool show3d: true
        x: displaySettings.mainGeometry.x
        y: displaySettings.mainGeometry.y
        width: displaySettings.mainGeometry.width
        height: displaySettings.mainGeometry.height
        sourceItem: viewportRoot
        hideSource: true
        sourceRect: show3d ? Qt.rect(0,0,viewportRoot.width, viewportRoot.height)
                           : Qt.rect(0,0,viewportRoot.width / 2, viewportRoot.height)

        onHeightChanged: {
            console.log("Main:")
            console.log(x + " " + y + " " + width + " " + height)
        }
        onWidthChanged: {
            console.log("Main:")
            console.log(x + " " + y + " " + width + " " + height)
        }
    }

    OculusView {
        visible: displaySettings.oculusVisible
        x: displaySettings.oculusGeometry.x
        y: displaySettings.oculusGeometry.y
        width: displaySettings.oculusGeometry.width // TODO figure out why we need a factor here to avoid errors without oculus
        height: displaySettings.oculusGeometry.height
        viewport: viewportRoot
    }

    FlyModeNavigator {
        id: flyModeNavigator
        camera: viewportRoot.camera
    }

    Rectangle {
        id: fileDataDialog
        anchors.fill: viewportClone
        opacity: 0.95
        visible: false
        enabled: true
        MouseArea {
            anchors.fill: parent
        }

        GridLayout {
            anchors.centerIn: parent
            width: parent.width * 0.5
            height: parent.height * 0.5
            columns: 2
            columnSpacing: parent.width * 0.01
            rowSpacing: parent.width * 0.01
            ExclusiveGroup { id: group }
            Label {
                text: "Format:"
                Layout.minimumWidth: 150
            }

            Row {
                spacing: 10
                RadioButton {
                    id: mts0Radio
                    text: "mts0"
                    exclusiveGroup: group
                    checked: true
                }
                RadioButton {
                    id: xyzRadio
                    text: "xyz"
                    exclusiveGroup: group
                }
            }

            Label {
                id: fileLabel
                text: "Folder:"
            }
            TextField {
                id: fileTextField
                Layout.fillWidth: true
                text: settings.value("previousFileDialogPath", "")
            }
            Button {
                text: "Browse..."
                onClicked: {
                    fileDialog.open()
                }
            }

            Item {
                Layout.fillWidth: true
            }
            Label {
                text: "Number of timesteps:"
                visible: mts0Radio.checked
            }
            TextField {
                id: nTimeStepsField
                text:  settings.value("previousNTimeSteps", "1")
                visible: mts0Radio.checked
            }
            Label {
                text: "Number of CPUs:"
                visible: mts0Radio.checked
            }
            Row {
                TextField {
                    width: 20
                    id: nCPUsx
                    text:  settings.value("previousNCPUsx", "2")
                }
                TextField {
                    width: 20
                    id: nCPUsy
                    text:  settings.value("previousNCPUsy", "2")
                }
                TextField {
                    width: 20
                    id: nCPUsz
                    text:  settings.value("previousNCPUsz", "2")
                }
                visible: mts0Radio.checked
            }
            Button {
                text: "OK"
                onClicked: {
                    if(mts0Radio.checked) {
                        stateManager.loadMts0(fileTextField.text, nTimeStepsField.text, Qt.vector3d(nCPUsx.text, nCPUsy.text, nCPUsz.text))
                    } else {
                        stateManager.loadXyz(fileTextField.text)
                    }
                    fileDataDialog.visible = false
                    rectRoot.focus = true
                    viewportRoot.update()
                    settings.setValue("previousFileDialogPath", fileTextField.text)
                    settings.setValue("previousNTimeSteps", nTimeStepsField.text)
                    settings.setValue("previousNCPUsx", nCPUsx.text)
                    settings.setValue("previousNCPUsy", nCPUsy.text)
                    settings.setValue("previousNCPUsz", nCPUsz.text)
                }
            }
            Button {
                text: "Cancel"
                onClicked:  {
                    fileDataDialog.visible = false
                    rectRoot.focus = true
                }
            }
            Item {
                Layout.fillHeight: true
            }
        }
    }

    FileDialog {
        id: fileDialog
        selectFolder: mts0Radio.checked
        onAccepted: {
            if(selectFolder) {
                fileTextField.text = folder.toString().replace("file://", "")
            } else {
                fileTextField.text = fileUrl.toString().replace("file://", "")
            }
        }
    }

    Keys.onPressed:  {
        if(event.modifiers & Qt.ControlModifier) {
            switch(event.key) {
            case Qt.Key_O:
                fileDataDialog.visible = true
                flyModeNavigator.deactivate()
                break;
            case Qt.Key_Q:
                flyModeNavigator.deactivate()
                Qt.quit();
                break;
            case Qt.Key_F:
                displaySettings.visible = true
                flyModeNavigator.deactivate()
                break;
            }
        } else {
            switch(event.key) {
            case Qt.Key_R:
                stateManager.showWater = !stateManager.showWater
                break
            case Qt.Key_O:
                oculusReader.enabled = !oculusReader.enabled
                break
            case Qt.Key_3:
                console.log("3 clicked")
                viewportClone.show3d = !viewportClone.show3d
                break
            }
        }
    }

    Text {
        text: "FPS: "+ multiSphere.fps.toFixed(0)
        color: "white"
    }

    Text {
        y: 20
        text: "Visible atoms: "+stateManager.numberOfAtoms
        color: "white"
    }

    DisplaySettings {
        id: displaySettings
        visible: false
        screenInfo: screenInfo
        onApply: {
            screenInfo.apply()
        }
        onClose: {
            displaySettings.visible = false
        }
        opacity: 1.0
    }
}
