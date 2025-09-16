#include "throttlewidget.h"
#include <QPainter>
#include <QMouseEvent>

ThrottleWidget::ThrottleWidget(QWidget *parent)
    : QWidget(parent), m_drag(None), m_speed(50) {
    setMinimumSize(100,300);
}

QRect ThrottleWidget::throttleRect() const {
    int throttleWidth = 60;
    int throttleHeight = 250;
    QRect fullRect = rect();
    int x = (fullRect.width() - throttleWidth)/2;
    int y = (fullRect.height() - throttleHeight)/2;
    return QRect(x, y, throttleWidth, throttleHeight);
}

void ThrottleWidget::paintEvent(QPaintEvent *) {
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);
    p.fillRect(rect(), QColor(20,30,40));
    QRect trect = throttleRect();
    p.setPen(Qt::green);
    QRect textRect(trect.left()-50, trect.top() - 30, trect.width()+100, 30);
    p.drawText(textRect, Qt::AlignHCenter | Qt::AlignVCenter,
               QString("Speed: %1%").arg(m_speed,0,'f',0));
    p.setBrush(QColor(50,50,50));
    p.setPen(QPen(Qt::white,2));
    p.drawRoundedRect(trect,5,5);
    int y = trect.bottom() - (m_speed/100.0)*trect.height();
    QRect knob(trect.left()-5, y-10, trect.width()+10, 20);
    QLinearGradient lg(knob.topLeft(), knob.bottomRight());
    lg.setColorAt(0,QColor(200,200,200));
    lg.setColorAt(1,QColor(120,120,120));
    p.setBrush(lg);
    p.setPen(QPen(Qt::black,1));
    p.drawRoundedRect(knob,5,5);
}

void ThrottleWidget::mousePressEvent(QMouseEvent *ev) {
    if (throttleRect().contains(ev->pos()))
        m_drag = Throttle;
}

void ThrottleWidget::mouseMoveEvent(QMouseEvent *ev) {
    if (m_drag == Throttle) {
        QRect t = throttleRect();
        double ratio = (t.bottom()-ev->pos().y())/(double)t.height();
        if (ratio<0) ratio=0; if (ratio>1) ratio=1;
        m_speed = ratio*100;
        emit speedChanged(m_speed);
        update();
    }
}

void ThrottleWidget::mouseReleaseEvent(QMouseEvent *) {
    m_drag = None;
}
