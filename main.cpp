#include <QtGui/QGuiApplication>
#include "frameratecounter.h"
#include "qtquick2applicationviewer.h"
#include <stereoviewport.h>
#include <fileio.h>
#include <OVR.h>
#include <oculusreader.h>
#include <oculusview.h>
#include <mdstatemanager.h>
#include <settings.h>
//#include <exampledatasource.h>

//#include <mousemover.h>
#include <QQmlContext>
#include <QQuickView>
#include <QQmlEngine>
#include <QScreen>
#include <screeninfoscreen.h>
#include <screeninfo.h>

int main(int argc, char *argv[])
{
    OVR::System::Init();
    qmlRegisterType<FileIO>("FileIO", 1, 0, "FileIO");
    qmlRegisterType<StereoViewport>("StereoViewport", 1, 0, "StereoViewport");
    qmlRegisterType<OculusReader>("OculusReader", 1, 0, "OculusReader");
    qmlRegisterType<FrameRateCounter>("OculusReader", 1, 0, "FrameRateCounter");
    qmlRegisterType<MDStateManager>("MDStateManager", 1, 0, "MDStateManager");
    qmlRegisterType<Settings>("Settings", 1, 0, "Settings");
    qmlRegisterType<ScreenInfo>("ScreenInfo", 1, 0, "ScreenInfo");
    qmlRegisterType<ScreenInfoScreen>("ScreenInfo", 1, 0, "ScreenInfoScreen");

    QGuiApplication app(argc, argv);

    QSurfaceFormat format;
    format.setMajorVersion(4);
    format.setMinorVersion(3);
    OculusView view;
    view.setFormat(format);
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

    setlocale (LC_ALL, "en_US.UTF8");
    return app.exec();
}
