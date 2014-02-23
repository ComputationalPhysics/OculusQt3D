#include "screeninfoscreen.h"

ScreenInfoScreen::ScreenInfoScreen(QRectF geometry, int id, QObject *parent) :
    QObject(parent),
    m_geometry(geometry),
    m_id(id)
{
    emit geometryChanged(geometry);
}

ScreenInfoScreen::ScreenInfoScreen(QObject *parent) :
    QObject(parent),
    m_id(-1)
{

}
