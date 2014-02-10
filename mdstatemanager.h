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

    int m_playBackSpeed;

protected:
    void readData(ifstream *file, void *value);
    void readMts(char *filename, QArray<char*> &atomTypesThisCPU, QArray<int> &atomIdsThisCPU, QArray<QVector3D > &positionsThisCPU, QVector3D &systemSize);
    void reset();
    MDState *loadTimestepMts0(string mts0_directory, QVector3D numberOfCPUs);
    int m_currentTimestep;
    int m_timeDirection;
    MDState *m_currentState;
    QArray<MDState*> m_states;
    QVector3D m_systemSize;
    QArray<QColor4ub> tmpQColor4ubArray;
    QArray<QVector3D> tmpQVector3DArray;
    QArray<QSizeF> tmpQSizeFArray;
    QTimer timer;

public:
    MDStateManager();
    void loadMts0(string foldername, int numberOfTimesteps, QVector3D numberOfCPUs);
    bool loadXyz(string filename);
    int getNumberOfTimesteps();

    virtual const QArray<QVector3D> &getPositions();
    virtual const QArray<QColor4ub> &getColors();
    virtual const QArray<QSizeF > &getSizes();

    int playBackSpeed() const
    {
        return m_playBackSpeed;
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
signals:
    void playBackSpeedChanged(int arg);
};

#endif // MDSTATEMANAGER_H
