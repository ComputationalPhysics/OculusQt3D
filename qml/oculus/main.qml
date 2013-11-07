import QtQuick 2.0
import Qt3D 2.0
import Qt3D.Shapes 2.0
import StereoViewport 1.0
import FileIO 1.0
import OculusReader 1.0
import Mts0_io 1.0
import MultiBillboard 1.0
import FlyModeNavigator 1.0
//import "../../../../flymodenavigator-qt3d/flymodenavigator-qt3d/qml/flymodenavigator"

Rectangle {
    id: rectRoot
    property point lensOffsetFromCenter: Qt.point(0,0)
    property rect distortion: Qt.rect(1, 0.22, 0.24, 0.0)
    property real aspectRatio: width / height;
    property real fillScale: 1.8;
    width: 1280
    height: 800

    Mts0_io {
        id: mts0_io
    }

//    OculusReader {
//        camera: viewportRoot.camera
//    }

    StereoViewport {
        id: viewportRoot
        fillColor: "black"
        width: rectRoot.width * 1.8
        height: rectRoot.height * 1.8 // TODO: Check real ratio
        stereoType: StereoViewport.LeftRight
        fovzoom: false
        light: Light {
            position: Qt.vector3d(2.0, 1.0, 3.0)
        }

        camera: Camera {
            id: camera
            nearPlane: 15.0
            farPlane: 1500
            fieldOfView: 90
            center: Qt.vector3d(1,0,0)
            eye: Qt.vector3d(5,0,0)
            eyeSeparation: 0.1
        }

        MultiBillboard {
            id: multiSphere
            cullFaces: Item3D.CullBackFaces
            sortPoints: Item3D.BackToFront

            mts0_io: mts0_io

            effect: ShaderProgram {
                texture: "particlebw2.png"

                vertexShader: billboardsVertexShader.read()
                fragmentShader: billboardsFragmentShader.read()
            }
        }

        // TODO Fix bug in MolecularDynamics class and remove this Sphere
        Sphere {
            x: -1000
            effect: Effect {
                color: "blue"
            }
        }
    }

    FileIO {
        id: vertexShaderFile
        source: "oculus.vert"
        onError: console.log(msg)
    }

    FileIO {
        id: fragmentShaderFile
        source: "oculus.frag"
        onError: console.log(msg)
    }

    FileIO {
        id: billboardsVertexShader
        source: "billboards.vert"
        onError: console.log(msg)
    }

    FileIO {
        id: billboardsFragmentShader
        source: "billboards.frag"
        onError: console.log(msg)
    }

    ShaderEffectSource {
        id: shaderEffectSourceLeft
        width: rectRoot.width / 2
        anchors {
            left: rectRoot.left
            top: rectRoot.top
            bottom: rectRoot.bottom
        }
        visible: true

        hideSource: true
        sourceItem: viewportRoot
        sourceRect: Qt.rect(0, 0, viewportRoot.width / 2, viewportRoot.height)
    }

    ShaderEffectSource {
        id: shaderEffectSourceRight
        width: rectRoot.width / 2
        anchors {
            right: rectRoot.right
            top: rectRoot.top
            bottom: rectRoot.bottom
        }
        visible: true

        hideSource: true
        sourceItem: viewportRoot
        sourceRect: Qt.rect(viewportRoot.width / 2, 0, viewportRoot.width / 2, viewportRoot.height)
    }

//    Item {
//        width: rectRoot.width / 2
//        anchors {
//            left: rectRoot.left
//            top: rectRoot.top
//            bottom: rectRoot.bottom
//        }
//        clip: true
//        ShaderEffect {
//            width: parent.width + 100
//            height: parent.height
//            x: 0

//            property variant qt_Texture0: shaderEffectSourceLeft
//            property point lensOffsetFromCenter: rectRoot.lensOffsetFromCenter
//            property rect distortion: rectRoot.distortion
//            property real aspectRatio: rectRoot.aspectRatio
//            property real fillScale: rectRoot.fillScale
//            vertexShader: vertexShaderFile.read()
//            fragmentShader: fragmentShaderFile.read()
//        }
//    }

//    Item {
//        width: rectRoot.width / 2
//        anchors {
//            right: rectRoot.right
//            top: rectRoot.top
//            bottom: rectRoot.bottom
//        }
//        clip: true
//        ShaderEffect {
//            width: parent.width + 100
//            height: parent.height
//            x: -100

//            property variant qt_Texture0: shaderEffectSourceRight
//            property point lensOffsetFromCenter: Qt.point(-rectRoot.lensOffsetFromCenter.x, rectRoot.lensOffsetFromCenter.y)
//            property rect distortion: rectRoot.distortion
//            property real aspectRatio: rectRoot.aspectRatio
//            property real fillScale: rectRoot.fillScale
//            vertexShader: vertexShaderFile.read()
//            fragmentShader: fragmentShaderFile.read()
//        }
//    }

    FlyModeNavigator {
        focus: true
        camera: viewportRoot.camera
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
