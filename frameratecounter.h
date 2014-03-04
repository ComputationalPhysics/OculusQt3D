#ifndef FRAMERATECOUNTER_H
#define FRAMERATECOUNTER_H

#include <QQuickItem3D>
#include <QElapsedTimer>
#include <QGLAbstractSurface>

class FrameRateCounter : public QQuickItem3D
{
    Q_OBJECT
    Q_PROPERTY(double fps READ fps NOTIFY fpsChanged)
public:
    explicit FrameRateCounter(QQuickItem *parent = 0);

    double fps() const;

signals:

    void fpsChanged(double arg);

public slots:


    // QQuickItem3D interface
protected:
    void drawItem(QGLPainter *painter);

private:
    double m_fps;
    QElapsedTimer m_fpsTimer;
    QGLAbstractSurface *m_usedSurface;
};

#endif // FRAMERATECOUNTER_H
