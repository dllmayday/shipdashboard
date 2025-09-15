#include "controlpanel.h"
#include <QPainter>
#include <QMouseEvent>
#include <QtMath>

ControlPanel::ControlPanel(QWidget *parent)
    : QWidget(parent), m_drag(None), m_heading(0), m_speed(0) {
    setMinimumSize(400,200);
}

QRect ControlPanel::steeringRect() const {
    return QRect(20,20,160,160); // 左边舵盘区域
}

QRect ControlPanel::throttleRect() const {
    return QRect(width()-80,20,40,160); // 右边油门杆区域
}

void ControlPanel::paintEvent(QPaintEvent *) {
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);

    // 背景
    p.fillRect(rect(), QColor(20,30,40));

    // 舵盘（Steering Wheel）
    QRect srect = steeringRect();
    p.save();
    p.translate(srect.center());
    p.scale(srect.width()/2.0/100.0, srect.height()/2.0/100.0);

    // 圆盘
    QRadialGradient grad(0,0,100);
    grad.setColorAt(0,QColor(80,80,80));
    grad.setColorAt(1,QColor(30,30,30));
    p.setBrush(grad);
    p.setPen(QPen(Qt::white,2));
    p.drawEllipse(QPoint(0,0),100,100);

    // 舵盘把手
    p.rotate(m_heading);
    p.setPen(QPen(Qt::yellow,6));
    p.drawLine(0,-80,0,80);
    p.drawLine(-80,0,80,0);
    p.restore();

    // 舵盘文字
    p.setPen(Qt::white);
    p.drawText(srect.adjusted(0,170,0,0), Qt::AlignHCenter, QString("Heading: %1°").arg(m_heading,0,'f',1));

    // 油门杆（Throttle）
    QRect trect = throttleRect();
    p.setBrush(QColor(50,50,50));
    p.setPen(QPen(Qt::white,2));
    p.drawRoundedRect(trect,5,5);

    int y = trect.bottom() - (m_speed/100.0)*trect.height();
    QRect knob(trect.left()-5, y-10, trect.width()+10,20);

    QLinearGradient lg(knob.topLeft(), knob.bottomRight());
    lg.setColorAt(0,QColor(200,200,200));
    lg.setColorAt(1,QColor(120,120,120));
    p.setBrush(lg);
    p.setPen(QPen(Qt::black,1));
    p.drawRoundedRect(knob,5,5);

    // 航速文字
    p.setPen(Qt::white);
    p.drawText(trect.adjusted(-60,170,60,0), Qt::AlignHCenter, QString("Speed: %1%").arg(m_speed,0,'f',0));
}

void ControlPanel::mousePressEvent(QMouseEvent *ev) {
    if (steeringRect().contains(ev->pos()))
        m_drag = Steering;
    else if (throttleRect().contains(ev->pos()))
        m_drag = Throttle;
}

void ControlPanel::mouseMoveEvent(QMouseEvent *ev) {
    if (m_drag == Steering) {
        QPoint c = steeringRect().center();
        double dx = ev->pos().x() - c.x();
        double dy = ev->pos().y() - c.y();
        m_heading = qRadiansToDegrees(qAtan2(dy,dx)) - 90;
        if (m_heading < -180) m_heading += 360;
        if (m_heading > 180) m_heading -= 360;
        emit headingChanged(m_heading);
        update();
    } else if (m_drag == Throttle) {
        QRect t = throttleRect();
        double ratio = (t.bottom()-ev->pos().y())/(double)t.height();
        if (ratio<0) ratio=0; if (ratio>1) ratio=1;
        m_speed = ratio*100;
        emit speedChanged(m_speed);
        update();
    }
}

void ControlPanel::mouseReleaseEvent(QMouseEvent *) {
    m_drag = None;
}
