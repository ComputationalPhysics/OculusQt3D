import QtQuick 2.0
import QtQuick.Controls 1.1
import QtQuick.Layouts 1.1
import QtQuick.Dialogs 1.1
import MDStateManager 1.0
import Settings 1.0

Rectangle {
    id: fileDataDialogRoot
    property MDStateManager stateManager
    property Settings settings
    width: 640
    height: 480
    opacity: 0.95
    visible: false
    enabled: true

    Keys.onPressed:  {
        switch(event.key) {
        case Qt.Key_Escape:
            fileDataDialogRoot.visible = false
            break
        }
    }

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

        Item {
            Layout.fillWidth: true
        }

        CheckBox {
            id: periodicCheckBox
            text: "Add periodic copies"
        }

        Button {
            text: "OK"
            onClicked: {
                stateManager.periodic = periodicCheckBox.checked
                if(mts0Radio.checked) {
                    stateManager.loadMts0(fileTextField.text, nTimeStepsField.text, Qt.vector3d(nCPUsx.text, nCPUsy.text, nCPUsz.text))
                } else {
                    stateManager.loadXyz(fileTextField.text)
                }
                fileDataDialogRoot.visible = false
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
                fileDataDialogRoot.visible = false
                rectRoot.focus = true
            }
        }
        Item {
            Layout.fillHeight: true
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
}
