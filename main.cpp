#include <QtGui/QGuiApplication>
#include "qtquick2applicationviewer.h"
#include <stereoviewport.h>
#include <fileio.h>

int main(int argc, char *argv[])
{
    qmlRegisterType<FileIO>("FileIO", 1, 0, "FileIO");
    qmlRegisterType<StereoViewport>("StereoViewport", 1, 0, "StereoViewport");
    QGuiApplication app(argc, argv);

    QtQuick2ApplicationViewer viewer;
    viewer.setMainQmlFile(QStringLiteral("qml/oculus/main.qml"));
    viewer.showExpanded();

    return app.exec();
}
