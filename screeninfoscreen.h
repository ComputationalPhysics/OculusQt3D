#ifndef SCREENINFOSCREEN_H
#define SCREENINFOSCREEN_H

#include <QObject>
#include <QRectF>

class ScreenInfoScreen : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QRectF geometry READ geometry NOTIFY geometryChanged)
    Q_PROPERTY(int id READ id NOTIFY idChanged)
public:
    ScreenInfoScreen(QRectF geometry, int id, QObject *parent = 0);
    ScreenInfoScreen(QObject *parent = 0);


    QRectF geometry() const
    {
        return m_geometry;
    }
    int id() const
    {
        return m_id;
    }

signals:
    void geometryChanged(QRectF arg);
    void idChanged(int arg);

private:
    QRectF m_geometry;
    int m_id;
};

#endif // SCREENINFOSCREEN_H
