#include "mdstate.h"

MDState::MDState()
{
    colorMap.insert("Si", QColor4ub(230,230,0));
    sizeMap.insert("Si",QSizeF(1.11, 1.11));

    colorMap.insert("A", QColor4ub(0,0,255));
    sizeMap.insert("A",QSizeF(0.66, 0.66));

    colorMap.insert("H", QColor4ub(255,255,255));
    sizeMap.insert("H",QSizeF(0.35, 0.35));

    colorMap.insert("O", QColor4ub(255,0,0));
    sizeMap.insert("O",QSizeF(0.66, 0.66));

    colorMap.insert("Na", QColor4ub(9,92,0));
    sizeMap.insert("Na",QSizeF(1.86, 1.86));

    colorMap.insert("Cl", QColor4ub(95,216,250));
    sizeMap.insert("Cl",QSizeF(1.02, 1.02));

    colorMap.insert("N", QColor4ub(255,0,0));
    sizeMap.insert("N",QSizeF(0.66, 0.66));
}

void MDState::reserveMemory(int numberOfAtoms) {
    m_positions.reserve(numberOfAtoms);
    m_colors.reserve(numberOfAtoms);
    m_sizes.reserve(numberOfAtoms);
}

void MDState::addAtom(QVector3D positions, char *atomName) {
    m_positions.push_back(positions);

    QMap<std::string, QColor4ub>::const_iterator colorIterator = colorMap.find(atomName);
    if(colorIterator != colorMap.end()) m_colors.push_back(colorIterator.value());
    else m_colors.push_back(QColor4ub(255,0,0));

    QMap<std::string, QSizeF>::const_iterator sizeIterator = sizeMap.find(atomName);
    if(sizeIterator != sizeMap.end()) m_sizes.push_back(sizeIterator.value());
    else m_sizes.push_back(QSizeF(1.0,1.0));
}

const QArray<QVector3D> &MDState::getPositions() {
    return m_positions;
}

const QArray<QColor4ub> &MDState::getColors() {
    return m_colors;
}

const QArray<QSizeF> &MDState::getSizes() {
    return m_sizes;
}
