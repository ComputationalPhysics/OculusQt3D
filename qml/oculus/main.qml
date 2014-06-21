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

    Component.onCompleted: {
        rectRoot.forceActiveFocus()
    }

    Keys.onPressed:  {
        if(event.modifiers & Qt.ControlModifier) {
            switch(event.key) {
            case Qt.Key_O:
                displaySettings.visible = false
                fileDataDialog.visible = !fileDataDialog.visible
                flyModeNavigator.deactivate()
                break;
            case Qt.Key_Q:
                flyModeNavigator.deactivate()
                Qt.quit();
                break;
            case Qt.Key_F:
                fileDataDialog.visible = false
                displaySettings.visible = !displaySettings.visible
                flyModeNavigator.deactivate()
                break;
            }
        } else {
            switch(event.key) {
            case Qt.Key_Escape:
                fileDataDialog.visible = false
                displaySettings.visible = false
                break
            case Qt.Key_P:
                multiSphere.hasPeriodicCopies = !multiSphere.hasPeriodicCopies
                break;
            case Qt.Key_R:
                stateManager.showWater = !stateManager.showWater
                break
            case Qt.Key_O:
                oculusReader.enabled = !oculusReader.enabled
                break
            case Qt.Key_3:
                viewportClone.show3d = !viewportClone.show3d
                break
            case Qt.Key_Plus:
                displaySettings.linearAttenuation.value += 0.01
                break
            case Qt.Key_Minus:
                displaySettings.linearAttenuation.value -= 0.01
                break
            }
        }
    }

    width: 1280
    height: 720
    focus: true

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
        stereoType: displaySettings.stretchedLeftRight ? StereoViewport.StretchedLeftRight : StereoViewport.LeftRight
        onStereoTypeChanged: {
            console.log(stereoType)
        }

        fovzoom: false

        light: Light {
            ambientColor: Qt.rgba(1,1,1,1)
            position: camera.eye
            linearAttenuation: displaySettings.linearAttenuation
            constantAttenuation: displaySettings.constantAttenuation
        }

        camera: Camera {
            id: camera
            nearPlane: 5.0
            farPlane: 5000
            fieldOfView: 60
            center: Qt.vector3d(1,0,0)
            eye: Qt.vector3d(5,0,0)
            eyeSeparation: 0.06
            onEyeChanged: {
                if(multiSphere.hasPeriodicCopies) {
                    var diffX = center.x - eye.x
                    var diffY = center.y - eye.y
                    var diffZ = center.z - eye.z
                    if(eye.x > stateManager.systemSize.x) eye.x = 0
                    if(eye.x < 0) eye.x = stateManager.systemSize.x

                    if(eye.y > stateManager.systemSize.y) eye.y = 0
                    if(eye.y < 0) eye.y = stateManager.systemSize.y

                    if(eye.z > stateManager.systemSize.z) eye.z = 0
                    if(eye.z < 0) eye.z = stateManager.systemSize.z

                    center.x = eye.x + diffX
                    center.y = eye.y + diffY
                    center.z = eye.z + diffZ
                }
            }
        }

        MDStateManager {
            id: stateManager
            onCurrentTimestepChanged: viewportRoot.update()
        }

        MultiBillboard {
            id: multiSphere
            hasPeriodicCopies: false
            dataSource: stateManager
            texture: "sphere3.png"
        }
        FrameRateCounter {
            id: frameRate
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
                           : (displaySettings.stretchedLeftRight ? Qt.rect(0,0,viewportRoot.width / 2, viewportRoot.height)
                                                                 : Qt.rect(0,viewportRoot.height/4, viewportRoot.width / 2, viewportRoot.height / 2))

    }

    OculusView {
        visible: displaySettings.oculusVisible
        x: displaySettings.oculusGeometry.x
        y: displaySettings.oculusGeometry.y
        width: displaySettings.oculusGeometry.width // TODO figure out why we need a factor here to avoid errors without oculus
        height: displaySettings.oculusGeometry.height
        viewport: viewportRoot
        stretchedSource: displaySettings.stretchedLeftRight
    }

    FlyModeNavigator {
        id: flyModeNavigator
        camera: viewportRoot.camera
        onFocusChanged: {
            if(!flyModeNavigator.focus) {
                rectRoot.focus = true
            }
        }
    }

    FileDataDialog {
        id: fileDataDialog
        stateManager: stateManager
        settings: settings
    }

    Text {
        text: "FPS: "+ frameRate.fps.toFixed(0)
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
