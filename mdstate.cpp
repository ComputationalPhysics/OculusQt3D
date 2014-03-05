#include "mdstate.h"
#include <iostream>
// double color_list[7][3] = {{1,1,1},{230.0/255,230.0/255,0},{0,0,1},{1.0,1.0,1.0},{1,0,0},{9.0/255,92.0/255,0},{95.0/255,216.0/255,250.0/255}};
MDState::MDState(QObject *parent) :
    QObject(parent),
    m_showWater(1)
{
    colorMap.insert("Si", QColor(230,230,0));
    sizeMap.insert("Si",QVector2D(1.11, 1.11));

    colorMap.insert("A", QColor(0,0,255));
    sizeMap.insert("A",QVector2D(0.66, 0.66));

    colorMap.insert("H", QColor(255,255,255));
    sizeMap.insert("H",QVector2D(0.35, 0.35));

    colorMap.insert("O", QColor(255,0,0));
    sizeMap.insert("O",QVector2D(0.66, 0.66));

    colorMap.insert("Na", QColor(9,92,0));
    sizeMap.insert("Na",QVector2D(1.86, 1.86));

    colorMap.insert("Cl", QColor(95,216,250));
    sizeMap.insert("Cl",QVector2D(1.02, 1.02));

    colorMap.insert("N", QColor(95,216,250));
    sizeMap.insert("N",QVector2D(0.66, 0.66));

    colorMap.insert("C", QColor(95,75,85));
    sizeMap.insert("C",QVector2D(0.66, 0.66));
}

MDState::~MDState() {
}

int MDState::getNumberOfAtoms() {
    int sum = 0;
    for(DataBundle* bundle : m_dataBundles) {
        sum += bundle->positions().count();
    }
    return sum;
}

void MDState::addAtom(QVector3D positions, char *atomType, bool addPeriodicCopy, QVector3D systemSize) {
    if(strcmp(atomType,"Na") == 0 || strcmp(atomType,"Cl") == 0) return;

    if(!m_atomTypeToDataBundle.contains(atomType)) {
        DataBundle *bundle = new DataBundle();
        m_dataBundles.append(bundle);
        if(!(strcmp(atomType, "H") == 0 || strcmp(atomType, "O") == 0)) {
            m_dataBundlesNoWater.append(bundle);
        }
        m_atomTypeToDataBundle.insert(atomType,bundle);
        qDebug() << atomType;
        qDebug() << colorMap.value(atomType);
        bundle->setColor(colorMap.value(atomType, QColor(255, 255, 255, 255)));
        bundle->setSize(sizeMap.value(atomType, QVector2D(0.1, 0.1)));
    }

    DataBundle* bundle = m_atomTypeToDataBundle.value(atomType);
    Q_ASSERT(bundle);

    bundle->positions().append(positions);

    if(addPeriodicCopy) {
        for(int i=-1; i<=1; i++) {
            for(int j=-1; j<=1; j++) {
                for(int k=-1; k<=1; k++) {
                    if(i== 0 && j==0 && k==0) continue;
                    QVector3D positionCopy = positions + QVector3D(i,j,k)*systemSize;
                    addAtom(positionCopy, atomType);
                }
            }
        }
    }
}

void MDState::addAtoms(QArray<QVector3D> positions, QArray<char *>atomTypes, bool addPeriodicCopy, QVector3D systemSize) {
    // Increase length of all arrays
//    reserveMemory(getNumberOfAtoms() + positions.size());

    for(int i=0; i<positions.size(); i++) {
        addAtom(positions.at(i), atomTypes.at(i), addPeriodicCopy, systemSize);
    }

}

void MDState::buildVertexBundle() {
    for(DataBundle *bundle : m_dataBundles) {
        bundle->generateVertexBundle();
    }
}

QArray<DataBundle*> *MDState::dataBundles()
{
    if(m_showWater) {
        return &m_dataBundles;
    } else {
        return &m_dataBundlesNoWater;
    }
}
