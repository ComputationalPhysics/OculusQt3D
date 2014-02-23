#ifndef SCREENINFOSCREEN_H
#define SCREENINFOSCREEN_H

#include <QObject>
#include <QRectF>

class ScreenInfoScreen : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QRectF geometry READ geometry NOTIFY geometryChanged)
public:
    ScreenInfoScreen(QRectF geometry, QObject *parent = 0);
    ScreenInfoScreen(QObject *parent = 0);


    QRectF geometry() const
    {
        return m_geometry;
    }
signals:
    void geometryChanged(QRectF arg);
private:
    QRectF m_geometry;
};

#endif // SCREENINFOSCREEN_H
