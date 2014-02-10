#include <QtGui/QGuiApplication>
#include "qtquick2applicationviewer.h"
#include <stereoviewport.h>
#include <fileio.h>
#include <OVR.h>
#include <oculusreader.h>
#include <oculusview.h>
#include <mdstatemanager.h>
//#include <exampledatasource.h>

//#include <mousemover.h>
#include <QQmlContext>
#include <QQuickView>
#include <QQmlEngine>
#include <QScreen>

int main(int argc, char *argv[])
{
    MDStateManager manager;

    OVR::System::Init();
    qmlRegisterType<FileIO>("FileIO", 1, 0, "FileIO");
    qmlRegisterType<StereoViewport>("StereoViewport", 1, 0, "StereoViewport");
    qmlRegisterType<OculusReader>("OculusReader", 1, 0, "OculusReader");

    QGuiApplication app(argc, argv);

    OculusView view;
#ifdef Q_OS_MACX
    view.addImportPath(".");
#else
    view.addImportPath("../libs");
#endif

#ifdef Q_OS_LINUX
//    view.engine()->addImportPath("/home/compphys/sandbox/flymodenavigator-qt3d/build-flymodenavigator-Desktop_Qt_5_2_0_GCC_64bit-Release/src/libs");
#else
//    view.engine()->addImportPath("/repos/flymodenavigator-qt3d/build-flymodenavigator-Desktop_Qt_5_2_0_clang_64bit-Release/src/libs");
#endif
    view.setMainQmlFile("qml/oculus/main.qml");
//    view.fullScreenAllMonitors();
    view.show();

    return app.exec();
}
