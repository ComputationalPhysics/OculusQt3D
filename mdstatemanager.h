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
    Q_PROPERTY(QVector3D numberOfProcessors READ numberOfProcessors WRITE setNumberOfProcessors NOTIFY numberOfProcessorsChanged)
    QVector3D m_numberOfProcessors;

protected:
    void readData(ifstream *file, void *value);
    void readMts(char *filename, QArray<char*> &atomTypesThisCPU, QArray<int> &atomIdsThisCPU, QArray<QVector3D > &positionsThisCPU, QVector3D &systemSize);
    MDState *loadAtoms(string mts0_directory);
    int m_currentTimestep;
    MDState *m_currentState;
    QArray<MDState*> m_states;
    QVector3D m_systemSize;

public:
    MDStateManager();
    void loadMts0(string foldername);
    bool loadXyz(string filename);

    virtual const QArray<QVector3D> &getPositions();
    virtual const QArray<QColor4ub> &getColors();
    virtual const QArray<QSizeF > &getSizes();

    QVector3D numberOfProcessors() const
    {
        return m_numberOfProcessors;
    }
public slots:
    void setNumberOfProcessors(QVector3D arg)
    {
        if (m_numberOfProcessors != arg) {
            m_numberOfProcessors = arg;
            emit numberOfProcessorsChanged(arg);
        }
    }
signals:
    void numberOfProcessorsChanged(QVector3D arg);
};

#endif // MDSTATEMANAGER_H
