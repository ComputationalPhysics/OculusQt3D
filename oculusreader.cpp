#include "oculusreader.h"
#include <QDebug>

OculusReader::OculusReader():
    m_camera(NULL)
{
    pManager = *DeviceManager::Create();
        DeviceEnumerator<SensorDevice> isensor = pManager->EnumerateDevices<SensorDevice>();
        DeviceEnumerator<SensorDevice> oculusSensor;
        DeviceEnumerator<SensorDevice> oculusSensor2;

        while(isensor)
        {
            DeviceInfo di;
            if (isensor.GetDeviceInfo(&di))
            {
                if (strstr(di.ProductName, "Tracker"))
                {
                    if (!oculusSensor)
                        oculusSensor = isensor;
                    else if (!oculusSensor2)
                        oculusSensor2 = isensor;
                }
            }

            isensor.Next();
        }

        if (oculusSensor)
        {
            pSensor = *oculusSensor.CreateDevice();

            if (pSensor)
                pSensor->SetRange(SensorRange(4 * 9.81f, 8 * Math<float>::Pi, 1.0f), true);

            if (oculusSensor2)
            {
                // Second Oculus sensor, useful for comparing firmware behavior & settings.
                pSensor2 = *oculusSensor2.CreateDevice();

                if (pSensor2)
                    pSensor2->SetRange(SensorRange(4 * 9.81f, 8 * Math<float>::Pi, 1.0f), true);
            }
        }

        oculusSensor.Clear();
        oculusSensor2.Clear();

        if (pSensor)
            SFusion.AttachToSensor(pSensor);
        if (pSensor2)
            SFusion2.AttachToSensor(pSensor2);
        m_isFirst = true;
        timer.setInterval(16);
        connect(&timer, SIGNAL(timeout()),this,SLOT(readSensors()));
        timer.start();
}

void OculusReader::readSensors() {
    if(m_camera == NULL) {
        qDebug() << "Camera pointer not set";
        return;
    }

    Quatf orient = SFusion.GetOrientation();
    if(m_isFirst) {
        upVectorStart = m_camera->upVector();
        viewVectorStart = m_camera->center() - m_camera->eye();
    }

    if(!m_isFirst) {
        QQuaternion qorient(orient.w, orient.z, orient.y, -orient.x); // høyre venstre riktig

        QVector3D upVector = qorient.rotatedVector(upVectorStart);
        QVector3D viewVector = qorient.rotatedVector(viewVectorStart);

        m_camera->setUpVector(upVector);
        m_camera->setCenter(m_camera->eye() + viewVector);
    }

    m_isFirst = false;
}
