#include "multibillboard.h"

#include <QGLBuilder>
#include <QQuickEffect>
#include <qmath.h>
#include <mts0_io.h>

MultiBillboard::MultiBillboard(QQuickItem *parent) :
    QQuickItem3D(parent),
    m_sortPoints(DefaultSorting),
    m_mts0_io(NULL),
    drawCalls(0)
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

// #define OLD

void MultiBillboard::drawItem(QGLPainter *painter) {
    Timestep *timestep = m_mts0_io->currentTimestepObject;
    if(timestep == NULL) return;
    vector<float> system_size = timestep->get_lx_ly_lz();
    int numAtoms = timestep->positions.size();

    if(++drawCalls % 2) {
        setFps(1000.0/elapsedTimer.elapsed());
        elapsedTimer.restart();
    }

    const QMatrix4x4 &modelViewMatrix = painter->modelViewMatrix();
    QVector3D right;
    right.setX(modelViewMatrix(0,0));
    right.setY(modelViewMatrix(0,1));
    right.setZ(modelViewMatrix(0,2));
    QVector3D up;
    up.setX(modelViewMatrix(1,0));
    up.setY(modelViewMatrix(1,1));
    up.setZ(modelViewMatrix(1,2));

    QVector3D v1;
    QVector3D v2;
    QVector3D v3;
    QVector3D v4;
    QVector2D t1(0,0);
    QVector2D t2(1,0);
    QVector2D t3(1,1);
    QVector2D t4(0,1);

    int color_list[7][3] = {{1,1,1},{230,230,0},{0,0,255},{255,255,255},{255,0,0},{9,92.0,0},{95,216,250}};
    double atom_radii[7] = {0, 1.11, 0.66, 0.35, 0.66, 1.86, 1.02};

    QGeometryData triangles;
    triangles.reserve(4*timestep->positions.size());

    QVector3D system_center(system_size[0]/2.0, system_size[1]/2.0, system_size[2]/2.0);
    QVector3D center;
    QVector3D normal = QVector3D::crossProduct(right,up);
    int count = 0;
    indexArray.clear();
    vectorArray.clear();
    normalArray.clear();
    colorArray.clear();
    textureArray.clear();

    indexArray.reserve(6*numAtoms);
    vectorArray.reserve(4*numAtoms);
    normalArray.reserve(4*numAtoms);
    colorArray.reserve(4*numAtoms);
    textureArray.reserve(4*numAtoms);

    QVector<QColor4ub> atom_colors;
    for(int atom_type=0; atom_type<7; atom_type++) {
        int r = color_list[atom_type][0];
        int g = color_list[atom_type][1];
        int b = color_list[atom_type][2];
        QColor4ub color(r,g,b,255);
        atom_colors.push_back(color);
    }


    for(int i = 0; i < timestep->positions.size(); i++) {
        center = QVector3D(timestep->positions[i][0],timestep->positions[i][1], timestep->positions[i][2]) - system_center;

        if(painter->isCullable(center)) {
            continue;
        }

        int atom_type = timestep->atom_types.at(i);

        double size = atom_radii[atom_type]*2.0;

        v1 = center - right * (size * 0.5);
        v2 = center + right * size * 0.5;
        v3 = center + right * size * 0.5 + up * size;
        v4 = center - right * size * 0.5 + up * size;

        vectorArray.append(v1.x(),v1.y(),v1.z());
        vectorArray.append(v2.x(),v2.y(),v2.z());
        vectorArray.append(v3.x(),v3.y(),v3.z());
        vectorArray.append(v4.x(),v4.y(),v4.z());

        normalArray.append(normal.x(), normal.y(), normal.z());
        normalArray.append(normal.x(), normal.y(), normal.z());
        normalArray.append(normal.x(), normal.y(), normal.z());
        normalArray.append(normal.x(), normal.y(), normal.z());

        colorArray.append(atom_colors[atom_type]);
        colorArray.append(atom_colors[atom_type]);
        colorArray.append(atom_colors[atom_type]);
        colorArray.append(atom_colors[atom_type]);

        indexArray.append(4*count + 0, 4*count + 1, 4*count + 2);
        indexArray.append(4*count + 2, 4*count + 3, 4*count + 0);

        textureArray.append(t1.x(), t1.y());
        textureArray.append(t2.x(), t2.y());
        textureArray.append(t3.x(), t3.y());
        textureArray.append(t4.x(), t4.y());
        count++;
    }

    triangles.appendTexCoordArray(textureArray);
    triangles.appendNormalArray(normalArray);
    triangles.appendColorArray(colorArray);
    triangles.appendVertexArray(vectorArray);
    triangles.appendIndices(indexArray);

    system_size.clear();
    glEnable(GL_BLEND);
    triangles.draw(painter,0,triangles.indexCount());
    glDisable(GL_BLEND);
    setVisibleAtoms(count);
}

MultiBillboard::~MultiBillboard()
{

}
