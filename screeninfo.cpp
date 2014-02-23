#include <QGuiApplication>
#include <QScreen>
#include <QDebug>
#include <QQuickItem>
#include <QQuickWindow>
#include "screeninfo.h"
#include "screeninfoscreen.h"

ScreenInfo::ScreenInfo(QObject *parent) :
    QObject(parent)
{
    QList<QScreen*> screenList = QGuiApplication::screens();
    for(QScreen *screen : screenList) {
        ScreenInfoScreen* screenInfoScreen = new ScreenInfoScreen(screen->geometry(), this);
        m_screens.append(screenInfoScreen);
        emit screensChanged();
    }
}

QQmlListProperty<ScreenInfoScreen> ScreenInfo::screens()
{
    return QQmlListProperty<ScreenInfoScreen>(this, m_screens);
}

void ScreenInfo::setupWindowInformation() {

}

void ScreenInfo::toggleFullscreen()
{
    QQuickItem *parentItem = dynamic_cast<QQuickItem*>(parent());
    if(!parentItem) {
        qWarning() << "Could not find parent window!";
        return;
    }
    if(!parentItem->window()) {
        qWarning() << "Could not find parent window!";
        return;
    }
    QQuickWindow* rootWindow = parentItem->window();
    qDebug() << "Window of parent is " << rootWindow->winId();
#ifdef Q_OS_LINUX
    display = XOpenDisplay(NULL);
    XSynchronize(display, True);
    x11root = XDefaultRootWindow(display);

    long colorBlue = 0xff0000ff; // shows up if the Qt window is not on top

    x11w = XCreateSimpleWindow(display, x11root, 0, 0, 640, 480, 0, 1 /*magic number*/, colorBlue);

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

    if(rootWindow->visibility() == QQuickWindow::FullScreen) {
        rootWindow->setVisibility(QQuickWindow::Windowed);
    } else {
        rootWindow->setVisibility(QQuickWindow::FullScreen);
    }

    Atom fullmons = XInternAtom(display, "_NET_WM_FULLSCREEN_MONITORS", False);
    XEvent xev2;
    memset(&xev2, 0, sizeof(xev2));
    xev2.type = ClientMessage;
    xev2.xclient.window = parentItem->window()->winId();
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
