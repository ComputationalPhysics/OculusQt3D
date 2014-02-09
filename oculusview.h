#ifndef OCULUSVIEW_H
#define OCULUSVIEW_H

#include <qtquick2applicationviewer/qtquick2applicationviewer.h>

class OculusView : public QtQuick2ApplicationViewer
{
    Q_OBJECT
public:
    explicit OculusView(QWindow *parent = 0);

    void fullScreenAllMonitors();
signals:

public slots:

};

#endif // OCULUSVIEW_H
