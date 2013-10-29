import QtQuick 2.0
import Qt3D 2.0
import Qt3D.Shapes 2.0
import StereoViewport 1.0
import FileIO 1.0

Rectangle {
    id: rectRoot
    property point lensOffsetFromCenter: Qt.point(0.05,0.0)
    property color distortion: Qt.rgba(1, 0.22, 0.24, 0.1)
    property real aspectRatio: width / height;
    property real fillScale: 0.2;
    width: 1000
    height: 800
    StereoViewport {
        id: viewportRoot
        fillColor: "black"
        anchors.fill: parent
        stereoType: StereoViewport.LeftRight
        light: Light {
            position: Qt.vector3d(2.0, 1.0, 3.0)
        }

        camera: Camera {
            nearPlane: 0.1
            farPlane: 100
            fieldOfView: 90
            center: Qt.vector3d(0,0,0)
            eye: Qt.vector3d(3,2,2)
            eyeSeparation: 0.1
        }

        Cube {
            effect: Effect {
                color: "yellow"
            }
        }

        Cube {
            x: 1.5
            effect: Effect {
                color: "yellow"
            }
        }

        Cube {
            y: 1.5
            effect: Effect {
                color: "yellow"
            }
        }

        Cube {
            z: 1.5
            effect: Effect {
                color: "yellow"
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

    ShaderEffectSource {
        id: shaderEffectSourceLeft
        width: viewportRoot.width / 2
        anchors {
            left: viewportRoot.left
            top: viewportRoot.top
            bottom: viewportRoot.bottom
        }

        hideSource: true
        sourceItem: viewportRoot
        sourceRect: Qt.rect(0, 0, viewportRoot.width / 2, viewportRoot.height)
    }

    ShaderEffectSource {
        id: shaderEffectSourceRight
        width: viewportRoot.width / 2
        anchors {
            right: viewportRoot.right
            top: viewportRoot.top
            bottom: viewportRoot.bottom
        }

        hideSource: true
        sourceItem: viewportRoot
        sourceRect: Qt.rect(viewportRoot.width / 2, 0, viewportRoot.width / 2, viewportRoot.height)
    }

    ShaderEffect {
        width: viewportRoot.width / 2
        anchors {
            left: viewportRoot.left
            top: viewportRoot.top
            bottom: viewportRoot.bottom
        }

        property variant qt_Texture0: shaderEffectSourceLeft
        property point lensOffsetFromCenter: rectRoot.lensOffsetFromCenter
        property color distortion: rectRoot.distortion
        property real aspectRatio: rectRoot.aspectRatio
        property real fillScale: rectRoot.fillScale
        vertexShader: vertexShaderFile.read()
        fragmentShader: fragmentShaderFile.read()
    }

    ShaderEffect {
        width: viewportRoot.width / 2
        anchors {
            right: viewportRoot.right
            top: viewportRoot.top
            bottom: viewportRoot.bottom
        }

        property variant qt_Texture0: shaderEffectSourceRight
        property point lensOffsetFromCenter: Qt.point(-rectRoot.lensOffsetFromCenter.x, rectRoot.lensOffsetFromCenter.y)
        property color distortion: rectRoot.distortion
        property real aspectRatio: rectRoot.aspectRatio
        property real fillScale: rectRoot.fillScale
        vertexShader: vertexShaderFile.read()
        fragmentShader: fragmentShaderFile.read()
    }
}
