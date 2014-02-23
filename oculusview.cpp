#include "oculusview.h"

#include <QList>
#include <QScreen>
#include <QDebug>
#include <QtGui/QGuiApplication>
#ifdef Q_OS_LINUX
#include <X11/Xlib.h>
#include <X11/X.h>
#endif

OculusView::OculusView(QWindow *parent) :
    QtQuick2ApplicationViewer(parent)
{
}

void OculusView::fullScreenAllMonitors() {
}
