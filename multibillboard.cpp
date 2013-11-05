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
    Timestep *timestep = m_mts0_io->current_timestep_object;
    if(timestep == NULL) return;
    vector<float> system_size = timestep->get_lx_ly_lz();

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
    vector<int> atom_types;
//    atom_types.resize(timestep->atom_types.size(), 2);

//    if(m_sortPoints == BackToFront) {
//        QMultiMap<double, QVector3D> sortedPoints;
//        for(int i = 0; i < timestep->positions.size(); i++) {
//            vector<float> &r = timestep->positions.at(i);
//            const QVector3D &center = QVector3D(r[0], r[1], r[2]);
//            const QVector4D &depthVector = painter->modelViewMatrix() * center;
//            double depth = depthVector.z();
//            sortedPoints.insert(depth, center);
//            atom_types.insert(atom_types.begin() + depth, timestep->atom_types.at(i));
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

    // float color_list[7][3] = {{1,1,1},{230.0/255,230.0/255,0},{0,0,1},{1.0,1.0,1.0},{1,0,0},{9.0/255,92.0/255,0},{95.0/255,216.0/255,250.0/255}};
    int color_list[7][3] = {{1,1,1},{230,230,0},{0,0,255},{255,255,255},{255,0,0},{9,92.0,0},{95,216,250}};

    QVector3D system_center(system_size[0]/2.0, system_size[1]/2.0, system_size[2]/2.0);
    // for(int i = 0; i < m_points.length(); i++) {
    for(int i = 0; i < timestep->positions.size(); i++) {
        QVector3D center = QVector3D(timestep->positions[i][0],timestep->positions[i][1], timestep->positions[i][2]) - system_center;
        // QVector3D center = m_points.at(i) - system_center;
        // float atom_type = float(round(atom_types.at(i)));
        int atom_type = timestep->atom_types.at(i);

        if(painter->isCullable(center)) {
            continue;
        }

        double size = 1.0;
        a = center - right * (size * 0.5);
        b = center + right * size * 0.5;
        c = center + right * size * 0.5 + up * size;
        d = center - right * size * 0.5 + up * size;
        quad.appendVertex(a,b,c,d);
        quad.appendTexCoord(ta, tb, tc, td);
        quad.appendAttribute(atom_type,atom_type,atom_type,atom_type, QGL::CustomVertex0);
        int r = color_list[atom_type][0];
        int g = color_list[atom_type][1];
        int b = color_list[atom_type][2];
        QColor4ub color(r,g,b,255);
        quad.appendColor(color, color, color, color);
//        quad.appendAttribute(atom_type, QGL::CustomVertex0);
//        quad.appendAttribute(atom_type, QGL::CustomVertex0);
//        quad.appendAttribute(atom_type, QGL::CustomVertex0);
    }
    //    }
    atom_types.clear();
    builder.addQuads(quad);
    QGLSceneNode* geometry = builder.finalizedSceneNode();
    if(m_geometry) {
        delete m_geometry;
    }
    m_geometry = geometry;
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER,0.9);
    m_geometry->draw(painter);
    glDisable(GL_BLEND);
    glDisable(GL_ALPHA_TEST);
}

MultiBillboard::~MultiBillboard()
{
}
