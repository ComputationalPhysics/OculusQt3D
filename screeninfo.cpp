#include <QGuiApplication>
#include <QScreen>
#include <QDebug>
#include <QQuickItem>
#include <QQuickWindow>
#include "screeninfo.h"
#include "screeninfoscreen.h"

#ifdef Q_OS_LINUX
#include <X11/Xlib.h>
#include <X11/X.h>
#endif

ScreenInfo::ScreenInfo(QObject *parent) :
    QObject(parent),
    m_fullScreen(false),
    m_topMost(0),
    m_bottomMost(0),
    m_leftMost(0),
    m_rightMost(0)
{
    QList<QScreen*> screenList = QGuiApplication::screens();
    int id = 0;
    for(QScreen *screen : screenList) {
        ScreenInfoScreen* screenInfoScreen = new ScreenInfoScreen(screen->geometry(), id, this);
        m_screens.append(screenInfoScreen);
        emit screensChanged();
        id++;
    }
}

void ScreenInfo::apply() {
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
#ifdef Q_OS_LINUX
    Display* display;
    display = XOpenDisplay(NULL);
    XSynchronize(display, True);

    if(m_fullScreen) {
        rootWindow->setVisibility(QQuickWindow::Windowed);
        Atom fullmons = XInternAtom(display, "_NET_WM_FULLSCREEN_MONITORS", False);
        XEvent xev2;
        memset(&xev2, 0, sizeof(xev2));
        xev2.type = ClientMessage;
        xev2.xclient.window = parentItem->window()->winId();
        xev2.xclient.message_type = fullmons;
        xev2.xclient.format = 32;
        xev2.xclient.data.l[0] = m_topMost; /* your topmost monitor number */
        xev2.xclient.data.l[1] = m_bottomMost; /* bottommost */
        xev2.xclient.data.l[2] = m_leftMost; /* leftmost */
        xev2.xclient.data.l[3] = m_rightMost; /* rightmost */
        xev2.xclient.data.l[4] = 0; /* source indication */

        XSendEvent (display, DefaultRootWindow(display), False,
                    SubstructureRedirectMask | SubstructureNotifyMask, &xev2);
        rootWindow->setVisibility(QQuickWindow::FullScreen);
    } else {
        rootWindow->setVisibility(QQuickWindow::Windowed);
    }
    XFlush(display);
#endif
}

QQmlListProperty<ScreenInfoScreen> ScreenInfo::screens()
{
    return QQmlListProperty<ScreenInfoScreen>(this, m_screens);
}

bool ScreenInfo::fullScreen() const
{
    return m_fullScreen;
}

int ScreenInfo::topMost() const
{
    return m_topMost;
}

int ScreenInfo::bottomMost() const
{
    return m_bottomMost;
}

int ScreenInfo::leftMost() const
{
    return m_leftMost;
}

int ScreenInfo::rightMost() const
{
    return m_rightMost;
}

void ScreenInfo::setFullScreen(bool arg)
{
    if (m_fullScreen != arg) {
        m_fullScreen = arg;
        emit fullScreenChanged(arg);
    }
}

void ScreenInfo::setTopMost(int arg)
{
    if (m_topMost != arg) {
        m_topMost = arg;
        emit topMostChanged(arg);
    }
}

void ScreenInfo::setBottomMost(int arg)
{
    if (m_bottomMost != arg) {
        m_bottomMost = arg;
        emit bottomMostChanged(arg);
    }
}

void ScreenInfo::setLeftMost(int arg)
{
    if (m_leftMost != arg) {
        m_leftMost = arg;
        emit leftMostChanged(arg);
    }
}

void ScreenInfo::setRightMost(int arg)
{
    if (m_rightMost != arg) {
        m_rightMost = arg;
        emit rightMostChanged(arg);
    }
}
