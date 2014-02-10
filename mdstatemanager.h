#ifndef MDSTATEMANAGER_H
#define MDSTATEMANAGER_H
#include <QArray>
#include <string>
#include <datasource.h>

using std::string;
class MDState;

class MDStateManager : public DataSource
{
    Q_OBJECT
    Q_PROPERTY(QVector3D numberOfProcessors READ numberOfProcessors WRITE setNumberOfProcessors NOTIFY numberOfProcessorsChanged)
    QVector3D m_numberOfProcessors;

protected:

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
