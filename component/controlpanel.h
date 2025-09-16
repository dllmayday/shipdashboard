#ifndef CONTROLPANEL_H
#define CONTROLPANEL_H

#include <QWidget>
#include "steeringwidget.h"
#include "throttlewidget.h"
#include "controlbuttonpanel.h"

class ControlPanel : public QWidget {
    Q_OBJECT
public:
    explicit ControlPanel(QWidget *parent=nullptr);

signals:
    void headingChanged(double);
    void speedChanged(double);

private:
    SteeringWidget *m_steering;
    ThrottleWidget *m_throttle;
    ControlButtonPanel *m_controlPanel;
};

#endif // CONTROLPANEL_H
