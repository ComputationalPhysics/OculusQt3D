#ifndef MDSTATE_H
#define MDSTATE_H
#include <QVector3D>
#include <QArray>
#include <QColor4ub>
#include <QVector2D>
#include <QGLVertexBundle>
#include <QMap>
#include <iostream>
#include <compphys/multibillboard/databundle.h>

using std::cout;
using std::endl;
class MDState : public QObject
{
    Q_PROPERTY(bool showWater READ showWater WRITE setShowWater NOTIFY showWaterChanged)

public:
    MDState();
    ~MDState();
//    const QArray<QVector3D> &getPositions();
//    const QArray<QColor4ub> &getColors();
//    const QArray<QVector2D> &getSizes();
    void addAtom(QVector3D positions, char *atomType, bool addPeriodicCopy = false, QVector3D systemSize = QVector3D(0,0,0));
    void addAtoms(QArray<QVector3D> positions, QArray<char *>atomTypes, bool addPeriodicCopy = false, QVector3D systemSize = QVector3D(0,0,0));
    int getNumberOfAtoms();
//    void reserveMemory(int numberOfAtoms);
    bool showWater() const
    {
        return m_showWater;
    }
    void buildVertexBundle();
//    QGLVertexBundle* vertexBundle();
    QArray<DataBundle> *dataBundles();
public slots:
    void setShowWater(bool arg)
    {
//        if (m_showWater != arg) {
//            m_showWater = arg;
//            // emit showWaterChanged(arg);
//            for(int i=0; i<getNumberOfAtoms(); i++) {
//                if(strcmp(m_atomTypes[i],"H") == 0 || strcmp(m_atomTypes[i],"O") == 0) {
//                    m_colors[i].setAlphaF(m_showWater);
//                }
//            }
//            buildVertexBundle();
//        }
    }
signals:
    void showWaterChanged(bool arg);
private:
    QMap<QString, DataBundle*> m_atomTypeToDataBundle;
//    QMap<QString, QArray<QVector3D> > m_positions;
//    QArray<QColor4ub> m_colors;
//    QArray<QVector2D> m_sizes;
    QArray<DataBundle> m_dataBundles;
//    QArray<char*> m_atomTypes;
//    QGLVertexBundle m_vertexBundle;
    QMap<QString, QColor> colorMap;
    QMap<QString, QVector2D> sizeMap;
    bool m_showWater;
};

#endif // EXAMPLEDATASOURCE_H
