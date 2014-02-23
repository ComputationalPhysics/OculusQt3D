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
    property int mainScreenWidth: 640
    property int mainScreenHeight: 480
    property int oculusScreenWidth: 320
    property int oculusScreenHeight: 240
    property int totalScreenWidth: 760
    width: mainScreenWidth
    height: mainScreenHeight

    Component.onCompleted: {
        rectRoot.forceActiveFocus()
        console.log("width, height:")
    }

    ScreenInfo {
        id: screenInfo
        Component.onCompleted: {
            mainScreenWidth = screens[0].geometry.width
            mainScreenHeight = screens[0].geometry.height
            oculusScreenWidth = screens[1].geometry.width
            oculusScreenHeight = screens[1].geometry.height
            totalScreenWidth = mainScreenWidth + oculusScreenWidth
        }
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
        //        width: rectRoot.width * 1.8
        //        height: rectRoot.height * 1.8 // TODO: Check real ratio
        width: mainScreenWidth / totalScreenWidth * parent.width * pixelScale
        height: parent.height * pixelScale
        stereoType: StereoViewport.StretchedLeftRight
        fovzoom: false
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
//            eyeSeparation: 0.06
            eyeSeparation: 0.0
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
        property bool show3d: true
        sourceItem: viewportRoot
        width: viewportRoot.width / pixelScale
        height: viewportRoot.height / pixelScale
        sourceRect: show3d ? Qt.rect(0,0,viewportRoot.width, viewportRoot.height)
                           : Qt.rect(0,0,viewportRoot.width / 2, viewportRoot.height)
    }

//    OculusView {
//        anchors {
//            right: parent.right
//        }
//        width: oculusScreenWidth / totalScreenWidth * parent.width
//        height: oculusScreenHeight / mainScreenHeight * parent.height
//        viewport: viewportRoot
//    }

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
                Qt.quit();
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
}
