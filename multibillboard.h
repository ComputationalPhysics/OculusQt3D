#ifndef MULTISPHERE2_H
#define MULTISPHERE2_H

#include <QQuickItem3D>
#include <QGLAbstractScene>
#include <QElapsedTimer>
#include <mts0_io.h>
#include <QGLBuilder>

class MultiBillboard : public QQuickItem3D
{
    Q_OBJECT
    Q_PROPERTY(SortMode sortPoints READ sortPoints WRITE setSortPoints NOTIFY sortPointsChanged)
    Q_PROPERTY(Mts0_io* mts0_io READ mts0_io WRITE setMts0_io NOTIFY mts0_ioChanged)
    Q_PROPERTY(double fps READ fps WRITE setFps NOTIFY fpsChanged)
    Q_PROPERTY(int visibleAtoms READ visibleAtoms WRITE setVisibleAtoms NOTIFY visibleAtomsChanged)
    QElapsedTimer elapsedTimer;
    int drawCalls;
public:
    explicit MultiBillboard(QQuickItem *parent = 0);
    ~MultiBillboard();

    SortMode sortPoints() const
    {
        return m_sortPoints;
    }

    void updatePoints();
    Mts0_io* mts0_io() const
    {
        return m_mts0_io;
    }

    double fps() const
    {
        return m_fps;
    }

    int visibleAtoms() const
    {
        return m_visibleAtoms;
    }

protected:
    void drawItem(QGLPainter *painter);
signals:

    void spacingChanged(double arg);

    void frequencyChanged(double arg);

    void sortPointsChanged(SortMode arg);

    void mts0_ioChanged(Mts0_io* arg);

    void fpsChanged(double arg);

    void visibleAtomsChanged(int arg);

public slots:

    void setSortPoints(SortMode arg)
    {
        if (m_sortPoints != arg) {
            m_sortPoints = arg;
            emit sortPointsChanged(arg);
            updatePoints();
            update();
        }
    }

    void setMts0_io(Mts0_io* arg)
    {
        if (m_mts0_io != arg) {
            if(m_mts0_io != NULL) disconnect(m_mts0_io, SIGNAL(currentTimestepObjectChanged()),this,SLOT(update()));

            m_mts0_io = arg;
            connect(m_mts0_io, SIGNAL(currentTimestepObjectChanged()),this,SLOT(update()));
            emit mts0_ioChanged(arg);
        }
    }

    void setFps(double arg)
    {
        if (m_fps != arg) {
            m_fps = 0.1*arg + 0.9*m_fps;
            emit fpsChanged(arg);
        }
    }

    void setVisibleAtoms(int arg)
    {
        if (m_visibleAtoms != arg) {
            m_visibleAtoms = arg;
            emit visibleAtomsChanged(arg);
        }
    }

private:

    QGLSceneNode *m_topNode;
    bool m_sceneSet;
    QGLSceneNode* m_geometry;
    QGLAbstractScene *scene;

    QList<QVector3D> m_points;
    SortMode m_sortPoints;
    Mts0_io* m_mts0_io;
    double m_fps;
    int m_visibleAtoms;
};

#endif // MULTISPHERE2_H
