import QtQuick 2.0
import Qt3D 2.0
import Qt3D.Shapes 2.0
import StereoViewport 1.0
import OculusReader 1.0
import MDStateManager 1.0

import CompPhys.MultiBillboard 1.0
import CompPhys.FlyModeNavigator 1.0

Rectangle {
    id: rectRoot
    width: 1920
    height: 1080

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
            position: Qt.vector3d(2.0, 1.0, 3.0)
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
        focus: true
        camera: viewportRoot.camera
    }

    Keys.onPressed:  {
        switch(event.key) {
        case Qt.Key_R:
            multiSphere.showWater = !multiSphere.showWater
            break
        case Qt.Key_O:
            oculusReader.enabled = !oculusReader.enabled
            break
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
