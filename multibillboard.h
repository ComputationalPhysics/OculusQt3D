#ifndef MULTISPHERE2_H
#define MULTISPHERE2_H

#include <QQuickItem3D>
#include <QGLAbstractScene>
#include <QElapsedTimer>
#include <mts0_io.h>

class MultiBillboard : public QQuickItem3D
{
    Q_OBJECT
    Q_PROPERTY(SortMode sortPoints READ sortPoints WRITE setSortPoints NOTIFY sortPointsChanged)
    Q_PROPERTY(Mts0_io* mts0_io READ mts0_io WRITE setMts0_io NOTIFY mts0_ioChanged)

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

protected:
    void drawItem(QGLPainter *painter);
signals:

    void spacingChanged(double arg);

    void frequencyChanged(double arg);

    void sortPointsChanged(SortMode arg);

    void mts0_ioChanged(Mts0_io* arg);

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
            m_mts0_io = arg;
            emit mts0_ioChanged(arg);
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
};

#endif // MULTISPHERE2_H
