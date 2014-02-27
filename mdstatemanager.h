#ifndef MDSTATEMANAGER_H
#define MDSTATEMANAGER_H
#include <QArray>
#include <string>
#include <compphys/multibillboard/datasource.h>
#include <Qt3DQuick>
#include "mdstate.h"

using std::ifstream;
using std::string;
using std::cerr;
using std::endl;
class MDState;

class MDStateManager : public DataSource
{
    Q_OBJECT
    Q_PROPERTY(int playBackSpeed READ playBackSpeed WRITE setPlayBackSpeed NOTIFY playBackSpeedChanged)
    Q_PROPERTY(int currentTimestep READ currentTimestep WRITE setCurrentTimestep NOTIFY currentTimestepChanged)
    Q_PROPERTY(bool showWater READ showWater WRITE setShowWater NOTIFY showWaterChanged)
    Q_PROPERTY(int numberOfAtoms READ numberOfAtoms WRITE setNumberOfAtoms NOTIFY numberOfAtomsChanged)
    Q_PROPERTY(QVector3D systemSize READ systemSize NOTIFY systemSizeChanged)
    Q_PROPERTY(bool periodic READ periodic WRITE setPeriodic NOTIFY periodicChanged)

public:
    MDStateManager();
    Q_INVOKABLE void loadMts0(QString foldername, int numberOfTimesteps, QVector3D numberOfCPUs);
    Q_INVOKABLE bool loadXyz(QString filename);
    int getNumberOfTimesteps();

//    virtual const QArray<QVector3D> &getPositions();
//    virtual const QArray<QColor4ub> &getColors();
//    virtual const QArray<QVector2D > &getSizes();

    virtual QArray<DataBundle> *dataBundles();

    int playBackSpeed() const
    {
        return m_playBackSpeed;
    }

    int currentTimestep() const
    {
        return m_currentTimestep;
    }

    bool showWater() const
    {
        return m_showWater;
    }

    int numberOfAtoms() const
    {
        return m_numberOfAtoms;
    }

    QVector3D systemSize() const
    {
        return m_systemSize;
    }

    QGLVertexBundle* vertexBundle();

    bool periodic() const
    {
        return m_periodic;
    }

public slots:
    void updateNextTimestep();
    void setPlayBackSpeed(int arg)
    {
        if (m_playBackSpeed != arg) {
            m_playBackSpeed = arg;
            emit playBackSpeedChanged(arg);
        }
    }
    void setCurrentTimestep(int arg)
    {
        if (m_currentTimestep != arg) {
            m_currentTimestep = arg;
            emit currentTimestepChanged(arg);
        }
    }

    void setShowWater(bool arg)
    {
        if (m_showWater != arg) {
            m_showWater = arg;
            // emit showWaterChanged(arg);
            for(int i=0; i<getNumberOfTimesteps(); i++) {
                m_states.at(i)->setShowWater(m_showWater);
            }
        }
    }

    void setNumberOfAtoms(int arg)
    {
        if (m_numberOfAtoms != arg) {
            m_numberOfAtoms = arg;
            emit numberOfAtomsChanged(arg);
        }
    }

    void setPeriodic(bool arg)
    {
        if (m_periodic != arg) {
            m_periodic = arg;
            emit periodicChanged(arg);
        }
    }

signals:
    void playBackSpeedChanged(int arg);
    void currentTimestepChanged(int arg);
    void showWaterChanged(bool arg);
    void numberOfAtomsChanged(int arg);
    void systemSizeChanged(QVector3D arg);
    void periodicChanged(bool arg);

protected:
    void readData(ifstream *file, void *value);
    void readMts(char *filename, QArray<char*> &atomTypesThisCPU, QArray<int> &atomIdsThisCPU, QArray<QVector3D > &positionsThisCPU, QVector3D &systemSize);
    void reset();
    MDState *loadTimestepMts0(string mts0_directory, QVector3D numberOfCPUs);

    QArray<MDState*> m_states;
    QVector3D m_systemSize;
    QArray<QColor4ub> tmpQColor4ubArray;
    QArray<QVector3D> tmpQVector3DArray;
    QArray<QVector2D> tmpQVector2DArray;
    QTimer timer;

private:
    int m_playBackSpeed;
    int m_currentTimestep;
    int m_timeDirection;
    bool m_showWater;
    int m_numberOfAtoms;
    bool m_periodic;
};

#endif // MDSTATEMANAGER_H
