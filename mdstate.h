#ifndef MDSTATE_H
#define MDSTATE_H
#include <QVector3D>
#include <QArray>
#include <QColor4ub>
#include <QSizeF>
#include <QMap>

class MDState
{
private:
    QArray<QVector3D> m_positions;
    QArray<QColor4ub> m_colors;
    QArray<QSizeF > m_sizes;
    QMap<std::string,QColor4ub> colorMap;
    QMap<std::string,QSizeF> sizeMap;
public:
    MDState();
    ~MDState();
    const QArray<QVector3D> &getPositions();
    const QArray<QColor4ub> &getColors();
    const QArray<QSizeF> &getSizes();
    void addAtom(QVector3D positions, char *atomType);
    void addAtoms(QArray<QVector3D> positions, QArray<char *>atomTypes);
    int getNumberOfAtoms();
    void reserveMemory(int numberOfAtoms);
};

#endif // EXAMPLEDATASOURCE_H
