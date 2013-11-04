#include "oculusreader.h"

OculusReader::OculusReader()
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

    timer.setInterval(16);
    connect(&timer, SIGNAL(timeout()),this,SLOT(readSensors()));
    timer.start();
}

void OculusReader::readSensors() {
    Quatf orient = SFusion.GetOrientation();
    m_phi = atan2(2*(orient.x*orient.y + orient.z*orient.w), (1 - 2*(orient.y*orient.y + orient.z*orient.z)));
    m_theta = asin(2*(orient.x*orient.z - orient.w*orient.y));
    m_psi = atan2(2*(orient.x*orient.w + orient.y*orient.z), (1 - 2*(orient.z*orient.z + orient.w*orient.w)));
    emit psiChanged(m_psi);
    emit thetaChanged(m_theta);
    emit phiChanged(m_phi);
}
