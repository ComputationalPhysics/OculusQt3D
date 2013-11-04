#ifndef OCULUSREADER_H
#define OCULUSREADER_H
#include <OVR.h>
#include <QObject>
#include <QTimer>

using namespace OVR;

class OculusReader : public QObject
{
    Q_OBJECT
    Q_PROPERTY(double phi READ phi WRITE setPhi NOTIFY phiChanged)
    Q_PROPERTY(double theta READ theta WRITE setTheta NOTIFY thetaChanged)
    Q_PROPERTY(double psi READ psi WRITE setPsi NOTIFY psiChanged)
private:
    double m_phi;
    double m_theta;
    double m_psi;
    QTimer timer;

public:
    OculusReader();
    virtual ~OculusReader() { }
    Ptr<DeviceManager> pManager;
    Ptr<SensorDevice>  pSensor;
    Ptr<SensorDevice>  pSensor2;
    SensorFusion       SFusion;
    SensorFusion       SFusion2;
    double phi() const
    {
        return m_phi;
    }
    double theta() const
    {
        return m_theta;
    }

    double psi() const
    {
        return m_psi;
    }

public slots:
    void setPhi(double arg)
    {
        if (m_phi != arg) {
            m_phi = arg;
            emit phiChanged(arg);
        }
    }
    void setTheta(double arg)
    {
        if (m_theta != arg) {
            m_theta = arg;
            emit thetaChanged(arg);
        }
    }

    void setPsi(double arg)
    {
        if (m_psi != arg) {
            m_psi = arg;
            emit psiChanged(arg);
        }
    }

    void readSensors();
signals:
    void phiChanged(double arg);
    void thetaChanged(double arg);
    void psiChanged(double arg);
};
#endif //OCULUSREADER_H
