import QtQuick 2.0
import Qt3D 2.0
import Qt3D.Shapes 2.0
import QtQuick.Controls 1.1
import QtQuick.Layouts 1.1
import QtQuick.Dialogs 1.1
import StereoViewport 1.0
import OculusReader 1.0
import MDStateManager 1.0

import CompPhys.MultiBillboard 1.0
import CompPhys.FlyModeNavigator 1.0

Rectangle {
    id: rectRoot
    width: 1920
    height: 1080

    Component.onCompleted: {
        rectRoot.forceActiveFocus()
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
        width: 1280
        height: 720
        stereoType: StereoViewport.LeftRight
        fovzoom: false
        light: Light {
            ambientColor: Qt.rgba(1,1,1,1)
            position: camera.eye
            quadraticAttenuation: 0.0001
        }

        camera: Camera {
            id: camera
            nearPlane: 2.0
            farPlane: 500
            fieldOfView: 60
            center: Qt.vector3d(1,0,0)
            eye: Qt.vector3d(5,0,0)
            eyeSeparation: 0.03
        }

        MultiBillboard {
            id: multiSphere
            dataSource: MDStateManager {
                id: stateManager
            }

            effect: Effect {
                texture: "sphere2.png"
                blending: true
                useLighting: true
            }
        }
    }

    OculusView {
        anchors {
            right: parent.right
        }

        width: 640
        height: 360
        viewport: viewportRoot
    }

    FlyModeNavigator {
        id: flyModeNavigator
        camera: viewportRoot.camera
    }

    Rectangle {
        id: fileDataDialog
        anchors.fill: viewportRoot
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
                text: "99"
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
                    text: "2"
                }
                TextField {
                    width: 20
                    id: nCPUsy
                    text: "2"
                }
                TextField {
                    width: 20
                    id: nCPUsz
                    text: "2"
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
            }
        } else {
            switch(event.key) {
            case Qt.Key_R:
                multiSphere.showWater = !multiSphere.showWater
                break
            case Qt.Key_O:
                oculusReader.enabled = !oculusReader.enabled
                break
            }
        }
    }

    Text {
        text: "FPS: "+multiSphere.fps.toFixed(2)
        color: "white"
    }

    Text {
        y: 20
        text: "Visible atoms: "+multiSphere.visibleAtoms
        color: "white"
    }
}
