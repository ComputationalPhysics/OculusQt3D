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
#ifdef Q_OS_LINUX
    Display* display = XOpenDisplay(NULL);
    XSynchronize(display, True);
    XID x11root = XDefaultRootWindow(display);

    long colorBlue = 0xff0000ff; // shows up if the Qt window is not on top

    XID x11w = XCreateSimpleWindow(display, x11root, 0, 0, 640, 480, 0, 1 /*magic number*/, colorBlue);

    int leftMost = 0;
    int rightMost = 0;
    int topMost = 0;
    int bottomMost = 0;
    int leftMostPixel = INFINITY;
    int rightMostPixel = -INFINITY;
    int topMostPixel = INFINITY;
    int bottomMostPixel = -INFINITY;
    QList<QScreen*> screenList = QGuiApplication::screens();
    for(int screenID = 0; screenID < screenList.length(); screenID++) {
        QScreen* screen = screenList.at(screenID);
        qDebug() << screenID << screen->geometry().y();
        int screenLeftMostPixel = screen->geometry().x();
        int screenRightMostPixel = screen->geometry().x() + screen->geometry().width();
        int screenTopMostPixel = screen->geometry().y();
        int screenBottomMostPixel = screen->geometry().y() + screen->geometry().height();
        if(screenLeftMostPixel < leftMostPixel) {
            leftMostPixel = screenLeftMostPixel;
            leftMost = screenID;
        }
        if(screenRightMostPixel > rightMostPixel) {
            rightMostPixel = screenRightMostPixel;
            rightMost = screenID;
        }
        if(screenTopMostPixel < topMostPixel) {
            topMostPixel = screenTopMostPixel;
            topMost = screenID;
        }
        if(screenBottomMostPixel > bottomMostPixel) {
            bottomMostPixel = screenBottomMostPixel;
            bottomMost = screenID;
        }
    }

    QRect virtualGeometry = QGuiApplication::screens().at(0)->virtualGeometry();
    XReparentWindow(display, winId(), x11w, 0, 0);

    XMapWindow(display, x11w); // must be performed after XReparentWindow,
    // otherwise the window is not visible.

    setGeometry(virtualGeometry);
    XEvent xev;
    Atom wm_state = XInternAtom(display, "_NET_WM_STATE", False);
    Atom fullscreen = XInternAtom(display, "_NET_WM_STATE_FULLSCREEN", False);
    memset(&xev, 0, sizeof(xev));
    xev.type = ClientMessage;
    xev.xclient.window = x11w;
    xev.xclient.message_type = wm_state;
    xev.xclient.format = 32;
    xev.xclient.data.l[0] = 1;
    xev.xclient.data.l[1] = fullscreen;
    xev.xclient.data.l[2] = 0;
    XSendEvent(display, XDefaultRootWindow(display), False,
               SubstructureNotifyMask, &xev);

    Atom fullmons = XInternAtom(display, "_NET_WM_FULLSCREEN_MONITORS", False);
    XEvent xev2;
    memset(&xev2, 0, sizeof(xev2));
    xev2.type = ClientMessage;
    xev2.xclient.window = x11w;
    xev2.xclient.message_type = fullmons;
    xev2.xclient.format = 32;
    xev2.xclient.data.l[0] = topMost; /* your topmost monitor number */
    xev2.xclient.data.l[1] = bottomMost; /* bottommost */
    xev2.xclient.data.l[2] = leftMost; /* leftmost */
    xev2.xclient.data.l[3] = rightMost; /* rightmost */
    xev2.xclient.data.l[4] = 0; /* source indication */

    XSendEvent (display, DefaultRootWindow(display), False,
                SubstructureRedirectMask | SubstructureNotifyMask, &xev2);
#endif
}
