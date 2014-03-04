#include "frameratecounter.h"

FrameRateCounter::FrameRateCounter(QQuickItem *parent) :
    QQuickItem3D(parent),
    m_fps(0.0),
    m_usedSurface(0)
{
    m_fpsTimer.restart();
}

double FrameRateCounter::fps() const
{
    return m_fps;
}

void FrameRateCounter::drawItem(QGLPainter *painter)
{
    Q_UNUSED(painter)

    if(!m_usedSurface) {
        m_usedSurface = painter->currentSurface();
    }
    if(m_usedSurface != painter->currentSurface()) {
        return; // If we are not painting to the original surface, we might be in the other 3D frame, so return
    }

    int sinceLastTime = m_fpsTimer.elapsed();
    if(sinceLastTime < 10) { // We are capped at 60 FPS, so anything lower is ignored
        return;
    }
    m_fpsTimer.restart();
    double currentFps = 1000.0 / sinceLastTime;
    m_fps = 0.8*m_fps + 0.2 * currentFps;
    emit fpsChanged(m_fps);
    sinceLastTime = 0.0;
}
