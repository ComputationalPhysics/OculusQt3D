#ifndef SCREENINFO_H
#define SCREENINFO_H

#include <QObject>
#include <QQmlListProperty>

class ScreenInfoScreen;

class ScreenInfo : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QQmlListProperty<ScreenInfoScreen> screens READ screens)
public:
    explicit ScreenInfo(QObject *parent = 0);

    QQmlListProperty<ScreenInfoScreen> screens();
signals:

public slots:

private:
    QList<ScreenInfoScreen*> m_screens;
};

#endif // SCREENINFO_H
