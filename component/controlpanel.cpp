
#include "controlpanel.h"
#include <QHBoxLayout>


ControlPanel::ControlPanel(QWidget *parent)
    : QWidget(parent) {

    setStyleSheet("background: #F0F0F0;");
    setMinimumSize(700,300);
    m_steering = new SteeringWidget(this);
    m_throttle = new ThrottleWidget(this);

    // Add control button panel
    m_controlPanel = new ControlButtonPanel(this);


    auto *layout = new QHBoxLayout(this);
    layout->setContentsMargins(0,0,0,0);
    layout->addWidget(m_steering, 1);
    layout->addWidget(m_throttle, 1);
    layout->addWidget(m_controlPanel,1);

    connect(m_steering, &SteeringWidget::headingChanged, this, &ControlPanel::headingChanged);
    connect(m_throttle, &ThrottleWidget::speedChanged, this, &ControlPanel::speedChanged);
}

// ...已移除自绘和鼠标事件逻辑，全部交由子组件处理...
