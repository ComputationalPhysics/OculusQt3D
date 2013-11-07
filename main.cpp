#include <QtGui/QGuiApplication>
#include "qtquick2applicationviewer.h"
#include <stereoviewport.h>
#include <fileio.h>
#include <OVR.h>
#include <oculusreader.h>
#include <mts0_io.h>
#include <multibillboard.h>
//#include <mousemover.h>
#include <QQmlContext>
#include <QQuickView>
#include <QQmlEngine>

int main(int argc, char *argv[])
{
    OVR::System::Init();

    qmlRegisterType<FileIO>("FileIO", 1, 0, "FileIO");
    qmlRegisterType<StereoViewport>("StereoViewport", 1, 0, "StereoViewport");
    qmlRegisterType<OculusReader>("OculusReader", 1, 0, "OculusReader");
    qmlRegisterType<MultiBillboard>("MultiBillboard", 1, 0, "MultiBillboard");
    qmlRegisterType<Mts0_io>("Mts0_io", 1, 0, "Mts0_io");

    QGuiApplication app(argc, argv);

    QQuickView view;
    view.engine()->addImportPath("/home/svenni/Dropbox/projects/programming/qt3dadditions/build-flymodenavigator-Desktop_Qt_5_1_1_GCC_64bit-Debug/src/libs");
    view.setSource(QUrl::fromLocalFile("qml/oculus/main.qml"));
//    MouseMover mouseMover(&view);

//    view.rootContext()->setContextProperty("mouseMover", &mouseMover);

    view.show();
    return app.exec();
}
