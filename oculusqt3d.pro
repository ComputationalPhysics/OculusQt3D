QT += 3dquick

# Add more folders to ship with the application, here
folder_01.source = qml/oculus
folder_01.target = qml
folder_02.source = qml/multibillboard
folder_02.target = qml

DEPLOYMENTFOLDERS = folder_01 folder_02
#mac: CONFIG -= app_bundle

QMAKE_MACOSX_DEPLOYMENT_TARGET = 10.9

# Additional import path used to resolve QML modules in Creator's code model
# QML_IMPORT_PATH = /repos/multibillboard/build-multibillboard-Desktop_Qt_5_2_0_clang_64bit-Release/src/libs
mac: INCLUDEPATH += /Users/anderhaf/Downloads/OculusSDK/LibOVR/Include
mac: INCLUDEPATH += /repos/multibillboard/multibillboard/src/libs
mac: INCLUDEPATH += /repos/multibillboard/build-multibillboard-Desktop_Qt_5_2_0_clang_64bit-Release/src/libs/MultiBillboard/include

#INCLUDEPATH += /repos/flymodenavigator-qt3d/flymodenavigator-qt3d/
DEPENDPATH += /Users/anderhaf/Downloads/OculusSDK/LibOVR/Include
mac: LIBS += -framework CoreFoundation
mac: LIBS += -framework CoreGraphics
mac: LIBS += -framework IOKit
mac: LIBS += -framework ApplicationServices
mac: LIBS += -L/Users/anderhaf/Downloads/OculusSDK/LibOVR/Lib/MacOS/Release/ -lovr

linux: INCLUDEPATH += /home/compphys/Downloads/OculusSDK/LibOVR/Include
linux: QML_IMPORT_PATH += /home/compphys/sandbox/flymodenavigator-qt3d/build-flymodenavigator-Desktop_Qt_5_2_0_GCC_64bit-Release/src/libs
linux: LIBS += -L/home/compphys/Downloads/OculusSDK/LibOVR/Lib/Linux/Release/x86_64/ -lovr
linux: LIBS += -lX11
linux: LIBS += -lXinerama
linux: LIBS += -ludev


# If your application uses the Qt Mobility libraries, uncomment the following
# lines and add the respective components to the MOBILITY variable.
# CONFIG += mobility
# MOBILITY +=


# The .cpp file which was generated for your project. Feel free to hack it.
SOURCES += main.cpp \
    stereoviewport.cpp \
    qglmaskedsurface.cpp \
    qgldrawbuffersurface.cpp \
    fileio.cpp \
    oculusreader.cpp \
    oculusview.cpp \
    mdstatemanager.cpp \
    mdstate.cpp \
    exampledatasource.cpp

# Installation path
# target.path =

# Please do not modify the following two lines. Required for deployment.
include(qtquick2applicationviewer/qtquick2applicationviewer.pri)
qtcAddDeployment()



HEADERS += \
    stereoviewport.h \
    qglmaskedsurface_p.h \
    qgldrawbuffersurface_p.h \
    fileio.h \
    oculusreader.h \
    oculusview.h \
    exampledatasource.h \
    mdstatemanager.h \
    mdstate.h
#    /repos/flymodenavigator-qt3d/flymodenavigator-qt3d/mousemover.h
LIBS += -L/repos/multibillboard/build-multibillboard-Desktop_Qt_5_2_0_clang_64bit-Release/src/libs/MultiBillboard/ -lMultiBillboard

macx {
    LIBS_TARGET_DIR = $$OUT_PWD/$${TARGET}.app/Contents/Resources/
    LIBS_APP_DIR = $$OUT_PWD/$${TARGET}.app/Contents/MacOS/

    copydata.commands = $(COPY_DIR) $$PWD/qml /repos/multibillboard/build-multibillboard-Desktop_Qt_5_2_0_clang_64bit-Release/src/libs/MultiBillboard/ $$LIBS_TARGET_DIR && $(COPY_DIR) /repos/multibillboard/build-multibillboard-Desktop_Qt_5_2_0_clang_64bit-Release/src/libs/MultiBillboard/libMultiBillboard.dylib $$LIBS_APP_DIR
    first.depends = $(first) copydeploymentfolders copydata
    export(first.depends)
    export(copydata.commands)
    QMAKE_EXTRA_TARGETS += first copydata
}

#copydata.commands = $(COPY_DIR) $$PWD/qml $$OUT_PWD
#first.depends = $(first) copydata
#export(first.depends)
#export(copydata.commands)
#QMAKE_EXTRA_TARGETS += first copydata

QMAKE_CXXFLAGS += -std=c++0x
