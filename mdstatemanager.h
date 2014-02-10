#ifndef MDSTATEMANAGER_H
#define MDSTATEMANAGER_H
#include <QArray>
#include <string>
#include <compphys/multibillboard/datasource.h>
#include <Qt3DQuick>
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

    int m_playBackSpeed;
    int m_currentTimestep;
    int m_timeDirection;

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

public:
    MDStateManager();
    Q_INVOKABLE void loadMts0(QString foldername, int numberOfTimesteps, QVector3D numberOfCPUs);
    Q_INVOKABLE bool loadXyz(QString filename);
    int getNumberOfTimesteps();

    virtual const QArray<QVector3D> &getPositions();
    virtual const QArray<QColor4ub> &getColors();
    virtual const QArray<QVector2D > &getSizes();

    int playBackSpeed() const
    {
        return m_playBackSpeed;
    }

    int currentTimestep() const
    {
        return m_currentTimestep;
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

signals:
    void playBackSpeedChanged(int arg);
    void currentTimestepChanged(int arg);
};

#endif // MDSTATEMANAGER_H
