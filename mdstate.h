#ifndef MDSTATE_H
#define MDSTATE_H
#include <QVector3D>
#include <QArray>
#include <QColor4ub>
#include <QVector2D>
#include <QMap>
#include <iostream>

using std::cout;
using std::endl;
class MDState : public QObject
{
    Q_PROPERTY(bool showWater READ showWater WRITE setShowWater NOTIFY showWaterChanged)
private:
    QArray<QVector3D> m_positions;
    QArray<QColor4ub> m_colors;
    QArray<QVector2D > m_sizes;
    QMap<std::string,QColor4ub> colorMap;
    QMap<std::string,QVector2D> sizeMap;
    bool m_showWater;

public:
    MDState();
    ~MDState();
    const QArray<QVector3D> &getPositions();
    const QArray<QColor4ub> &getColors();
    const QArray<QVector2D> &getSizes();
    void addAtom(QVector3D positions, char *atomType);
    void addAtoms(QArray<QVector3D> positions, QArray<char *>atomTypes);
    int getNumberOfAtoms();
    void reserveMemory(int numberOfAtoms);
    bool showWater() const
    {
        return m_showWater;
    }
public slots:
    void setShowWater(bool arg)
    {
        if (m_showWater != arg) {
            m_showWater = arg;
            // emit showWaterChanged(arg);
            for(int i=0; i<getNumberOfAtoms(); i++) {
                m_colors[i].setAlphaF(m_showWater);
            }
        }
    }
signals:
    void showWaterChanged(bool arg);
};

#endif // EXAMPLEDATASOURCE_H
