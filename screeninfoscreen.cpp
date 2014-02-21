#include "screeninfoscreen.h"

ScreenInfoScreen::ScreenInfoScreen(QRectF geometry, QObject *parent) :
    QObject(parent),
    m_geometry(geometry)
{
    emit geometryChanged(geometry);
}

ScreenInfoScreen::ScreenInfoScreen(QObject *parent) :
    QObject(parent)
{

}
