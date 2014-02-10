import QtQuick 2.0
import Qt3D 2.0
import FileIO 1.0
import StereoViewport 1.0

Rectangle {
    id: oculusRoot
    color: "blue"
    property StereoViewport viewport
    property bool hideViewport: false
    property point lensOffsetFromCenter: Qt.point(0,0)
    property rect distortion: Qt.rect(1, 0.22, 0.24, 0.0)
    property real aspectRatio: width / height;
    property real fillScale: 1.8;
    width: 1280
    height: 720
    ShaderEffectSource {
        id: shaderEffectSourceLeft
        width: oculusRoot.width / 2
        anchors {
            left: oculusRoot.left
            top: oculusRoot.top
            bottom: oculusRoot.bottom
        }
        visible: true

        hideSource: hideViewport
        sourceItem: viewport
        sourceRect: Qt.rect(0, 0, viewport.width / 4, viewport.height)
    }

    ShaderEffectSource {
        id: shaderEffectSourceRight
        width: oculusRoot.width / 2
        anchors {
            right: oculusRoot.right
            top: oculusRoot.top
            bottom: oculusRoot.bottom
        }
        visible: true

        hideSource: hideViewport
        sourceItem: viewport
        sourceRect: Qt.rect(viewport.width / 2, 0, viewport.width / 4, viewport.height)
    }

    Item {
        width: oculusRoot.width / 2
        anchors {
            left: oculusRoot.left
            top: oculusRoot.top
            bottom: oculusRoot.bottom
        }
        clip: true
        ShaderEffect {
            width: parent.width + 100
            height: parent.height
            x: 0

            property variant qt_Texture0: shaderEffectSourceLeft
            property point lensOffsetFromCenter: oculusRoot.lensOffsetFromCenter
            property rect distortion: oculusRoot.distortion
            property real aspectRatio: oculusRoot.aspectRatio
            property real fillScale: oculusRoot.fillScale
            vertexShader: vertexShaderFile.read()
            fragmentShader: fragmentShaderFile.read()
        }
    }

    Item {
        width: oculusRoot.width / 2
        anchors {
            right: oculusRoot.right
            top: oculusRoot.top
            bottom: oculusRoot.bottom
        }
        clip: true
        ShaderEffect {
            width: parent.width + 100
            height: parent.height
            x: -100

            property variant qt_Texture0: shaderEffectSourceRight
            property point lensOffsetFromCenter: Qt.point(-oculusRoot.lensOffsetFromCenter.x, oculusRoot.lensOffsetFromCenter.y)
            property rect distortion: oculusRoot.distortion
            property real aspectRatio: oculusRoot.aspectRatio
            property real fillScale: oculusRoot.fillScale
            vertexShader: vertexShaderFile.read()
            fragmentShader: fragmentShaderFile.read()
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
}
