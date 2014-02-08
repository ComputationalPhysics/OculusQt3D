#include <QtGui/QGuiApplication>
#include "qtquick2applicationviewer.h"
#include <stereoviewport.h>
#include <fileio.h>
#include <OVR.h>
#include <oculusreader.h>
#include <mts0_io.h>
#include <multibillboard.h>
#include <oculusview.h>
//#include <mousemover.h>
#include <QQmlContext>
#include <QQuickView>
#include <QQmlEngine>
#include <QScreen>
#include <X11/Xlib.h>
#include <X11/X.h>

int main(int argc, char *argv[])
{
    OVR::System::Init();
Window x11root;
    qmlRegisterType<FileIO>("FileIO", 1, 0, "FileIO");
    qmlRegisterType<StereoViewport>("StereoViewport", 1, 0, "StereoViewport");
    qmlRegisterType<OculusReader>("OculusReader", 1, 0, "OculusReader");
    qmlRegisterType<MultiBillboard>("MultiBillboard", 1, 0, "MultiBillboard");
    qmlRegisterType<Mts0_io>("Mts0_io", 1, 0, "Mts0_io");


    QGuiApplication app(argc, argv);

    OculusView view;
#ifdef Q_OS_LINUX
    view.engine()->addImportPath("/home/compphys/sandbox/flymodenavigator-qt3d/build-flymodenavigator-Desktop_Qt_5_2_0_GCC_64bit-Release/src/libs");
#else
    view.engine()->addImportPath("/repos/flymodenavigator-qt3d/build-flymodenavigator-Desktop_Qt_5_2_0_clang_64bit-Release/src/libs");
#endif
    view.setMainQmlFile("qml/oculus/main.qml");
//    view.fullScreenAllMonitors();
    view.show();

    return app.exec();
}
