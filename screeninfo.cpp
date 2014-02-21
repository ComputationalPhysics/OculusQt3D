#include "screeninfo.h"
#include "screeninfoscreen.h"

#include <QGuiApplication>
#include <QScreen>

ScreenInfo::ScreenInfo(QObject *parent) :
    QObject(parent)
{
    QList<QScreen*> screenList = QGuiApplication::screens();
    for(QScreen *screen : screenList) {
        ScreenInfoScreen* screenInfoScreen = new ScreenInfoScreen(screen->geometry(), this);
        m_screens.append(screenInfoScreen);
    }
}

QQmlListProperty<ScreenInfoScreen> ScreenInfo::screens()
{
    return QQmlListProperty<ScreenInfoScreen>(this, m_screens);
}
