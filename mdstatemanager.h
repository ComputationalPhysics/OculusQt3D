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

protected:
    void readData(ifstream *file, void *value);
    void readMts(char *filename, QArray<char*> &atomTypesThisCPU, QArray<int> &atomIdsThisCPU, QArray<QVector3D > &positionsThisCPU, QVector3D &systemSize);
    void reset();
    MDState *loadTimestepMts0(string mts0_directory, QVector3D numberOfCPUs);
    int m_currentTimestep;
    MDState *m_currentState;
    QArray<MDState*> m_states;
    QVector3D m_systemSize;
    QArray<QColor4ub> tmpQColor4ubArray;
    QArray<QVector3D> tmpQVector3DArray;
    QArray<QSizeF> tmpQSizeFArray;

public:
    MDStateManager();
    void loadMts0(string foldername, int numberOfTimesteps, QVector3D numberOfCPUs);
    bool loadXyz(string filename);

    virtual const QArray<QVector3D> &getPositions();
    virtual const QArray<QColor4ub> &getColors();
    virtual const QArray<QSizeF > &getSizes();
};

#endif // MDSTATEMANAGER_H
