#ifndef SCREENINFO_H
#define SCREENINFO_H

#include <QObject>
#include <QQmlListProperty>
#ifdef Q_OS_LINUX
#include <X11/Xlib.h>
#include <X11/X.h>
#endif

class ScreenInfoScreen;

class ScreenInfo : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QQmlListProperty<ScreenInfoScreen> screens READ screens NOTIFY screensChanged)
public:
    explicit ScreenInfo(QObject *parent = 0);

    QQmlListProperty<ScreenInfoScreen> screens();

    Q_INVOKABLE void toggleFullscreen();
    void setupWindowInformation();
signals:
    void screensChanged();

public slots:

private:
    QList<ScreenInfoScreen*> m_screens;

#ifdef Q_OS_LINUX
    Display* display;
    XID x11root;
    XID x11w;
#endif
};

#endif // SCREENINFO_H
