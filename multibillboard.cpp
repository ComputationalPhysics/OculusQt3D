#include "multibillboard.h"

#include <QGLBuilder>
#include <QQuickEffect>
#include <qmath.h>
#include <mts0_io.h>
MultiBillboard::MultiBillboard(QQuickItem *parent) :
    QQuickItem3D(parent),
    m_sortPoints(DefaultSorting),
    m_mts0_io(NULL),
    drawCalls(0),
    count(0),
    m_showWater(true)
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

#define SI_TYPE 1
#define A_TYPE 2
#define H_TYPE 3
#define O_TYPE 4
#define NA_TYPE 5
#define CL_TYPE 6
#define X_TYPE 7

void MultiBillboard::drawItem(QGLPainter *painter) {
    Timestep *timestep = m_mts0_io->currentTimestepObject;
    if(timestep == NULL) return;

    vector<float> system_size = timestep->get_lx_ly_lz();
    int numAtoms = timestep->positions.size();
    if(visibleAtomIndices.size() < numAtoms) visibleAtomIndices.resize(numAtoms);

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

    QVector<QColor4ub> atom_colors;
    for(int atom_type=0; atom_type<7; atom_type++) {
        int r = color_list[atom_type][0];
        int g = color_list[atom_type][1];
        int b = color_list[atom_type][2];
        QColor4ub color(r,g,b,255);
        atom_colors.push_back(color);
    }

    QVector3D cameraPosition = m_camera->eye();
    double dr2_max = 100000;
    double water_dr2_max = 2000;

    if(!(drawCalls % 5)) {
        count = 0;

        for(int i = 0; i < timestep->positions.size(); i++) {
            center = QVector3D(timestep->positions[i][0],timestep->positions[i][1], timestep->positions[i][2]) - system_center;
            int atom_type = timestep->atom_types.at(i);
            bool is_water = (atom_type == H_TYPE || atom_type == O_TYPE);
            if(is_water && !m_showWater) continue;

            QVector3D relativeDistance = center - cameraPosition;
            double dr2 = relativeDistance.lengthSquared();
            if(dr2 < 50) continue;
            if(dr2 > dr2_max) continue;
            if(is_water && dr2 > water_dr2_max) continue;

            if(painter->isCullable(center)) {
                continue;
            }

            visibleAtomIndices[count++] = i;
        }
    }

    indexArray.clear();
    vertexArray.clear();
    normalArray.clear();
    colorArray.clear();
    textureArray.clear();

    vertexArray.reserve(4*count);
    indexArray.reserve(6*count);
    normalArray.reserve(4*count);
    colorArray.reserve(4*count);
    textureArray.reserve(4*count);

    for(int i = 0; i < count; i++) {
        int index = visibleAtomIndices[i];
        center = QVector3D(timestep->positions[index][0],timestep->positions[index][1], timestep->positions[index][2]) - system_center;

        int atom_type = timestep->atom_types.at(index);
        double size = atom_radii[atom_type]*2.0;

        v1 = center - right * (size * 0.5);
        v2 = center + right * size * 0.5;
        v3 = center + right * size * 0.5 + up * size;
        v4 = center - right * size * 0.5 + up * size;

        vertexArray.append(v1.x(),v1.y(),v1.z());
        vertexArray.append(v2.x(),v2.y(),v2.z());
        vertexArray.append(v3.x(),v3.y(),v3.z());
        vertexArray.append(v4.x(),v4.y(),v4.z());

        normalArray.append(normal.x(), normal.y(), normal.z());
        normalArray.append(normal.x(), normal.y(), normal.z());
        normalArray.append(normal.x(), normal.y(), normal.z());
        normalArray.append(normal.x(), normal.y(), normal.z());

        colorArray.append(atom_colors[atom_type]);
        colorArray.append(atom_colors[atom_type]);
        colorArray.append(atom_colors[atom_type]);
        colorArray.append(atom_colors[atom_type]);

        indexArray.append(4*i + 0, 4*i + 1, 4*i + 2);
        indexArray.append(4*i + 2, 4*i + 3, 4*i + 0);

        textureArray.append(t1.x(), t1.y());
        textureArray.append(t2.x(), t2.y());
        textureArray.append(t3.x(), t3.y());
        textureArray.append(t4.x(), t4.y());
    }

    triangles.appendTexCoordArray(textureArray);
    triangles.appendNormalArray(normalArray);
    triangles.appendColorArray(colorArray);
    triangles.appendVertexArray(vertexArray);
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
