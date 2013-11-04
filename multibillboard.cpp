#include "multibillboard.h"

#include <QGLBuilder>
#include <QQuickEffect>
#include <qmath.h>
#include <mts0_io.h>

MultiBillboard::MultiBillboard(QQuickItem *parent) :
    QQuickItem3D(parent),
    m_sortPoints(DefaultSorting)
{
    updatePoints();
}

void MultiBillboard::updatePoints() {
    m_points.clear();
    double spacing = 1;
    int nPerDim = 20;
    double frequency = 0.1;
    for(int i = 0; i < nPerDim; i++) {
        for(int j = 0; j < nPerDim; j++) {
            for(int k = 0; k < nPerDim; k++) {
                QVector3D center(-nPerDim/2 + i * spacing,-nPerDim/2 + j,-nPerDim/2 + k);
                QVector3D center2 = center;
                center.setX(center.x() + cos(2 * frequency * center2.z()));
                center.setZ(center.z() + sin(2 * frequency * center2.y()));
                m_points.push_back(center);
            }
        }
    }
}

void MultiBillboard::drawItem(QGLPainter *painter) {
    // Build the mesh
    QGLBuilder builder;
    builder.newSection(QGL::NoSmoothing);
    const QMatrix4x4 &modelViewMatrix = painter->modelViewMatrix();
    QVector3D right;
    right.setX(modelViewMatrix(0,0));
    right.setY(modelViewMatrix(0,1));
    right.setZ(modelViewMatrix(0,2));
    QVector3D up;
    up.setX(modelViewMatrix(1,0));
    up.setY(modelViewMatrix(1,1));
    up.setZ(modelViewMatrix(1,2));
    QGeometryData quad;

//    if(m_sortPoints == BackToFront) {
//        QMultiMap<double, QVector3D> sortedPoints;
//        for(int i = 0; i < m_points.length(); i++) {
//            const QVector3D &center = m_points.at(i);
//            const QVector4D &depthVector = painter->modelViewMatrix() * center;
//            double depth = depthVector.z();
//            sortedPoints.insert(depth, center);
//        }
//        m_points.clear();
//        QMapIterator<double, QVector3D> i(sortedPoints);
//        while(i.hasNext()) {
//            m_points.push_back(i.next().value());
//        }
//        sortedPoints.clear();
//    }

    QVector3D a;
    QVector3D b;
    QVector3D c;
    QVector3D d;
    QVector2D ta(0,0);
    QVector2D tb(0,1);
    QVector2D tc(1,1);
    QVector2D td(1,0);
    Timestep *timestep = m_mts0_io->current_timestep_object;
    if(timestep == NULL) return;
    vector<float> system_size = timestep->get_lx_ly_lz();

    for(int n=0; n<timestep->positions.size(); n++) {
    // for(int i = 0; i < m_points.length(); i++) {
        vector<float> &r = timestep->positions.at(n);
        float atom_type = float(timestep->atom_types.at(n));
        const QVector3D &center = QVector3D(r[0] - system_size[0]/2.0, r[1] - system_size[1]/2.0, r[2] - system_size[2]/2.0);
        if(painter->isCullable(center)) {
            continue;
        }
        double size = 0.2;
        a = center - right * (size * 0.5);
        b = center + right * size * 0.5;
        c = center + right * size * 0.5 + up * size;
        d = center - right * size * 0.5 + up * size;
        quad.appendVertex(a,b,c,d);
        quad.appendTexCoord(ta, tb, tc, td);
        quad.appendAttribute(atom_type,atom_type,atom_type,atom_type);
    }
    //    }
    builder.addQuads(quad);
    QGLSceneNode* geometry = builder.finalizedSceneNode();
    if(m_geometry) {
        delete m_geometry;
    }
    m_geometry = geometry;
    m_geometry->draw(painter);
}

MultiBillboard::~MultiBillboard()
{
}
