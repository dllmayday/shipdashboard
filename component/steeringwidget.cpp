#include "steeringwidget.h"
#include <QPainter>
#include <QMouseEvent>
#include <QtMath>

SteeringWidget::SteeringWidget(QWidget *parent)
    : QWidget(parent), m_drag(None), m_heading(0) {
    setMinimumSize(300,300);
    m_wheel = QPixmap(":/images/steering_wheel.png");
}

QRect SteeringWidget::steeringRect() const {
    QRect fullRect = rect();
    QSize wheelSize = m_wheel.size();
    if (wheelSize.isEmpty())
        return fullRect;
    double scale = qMin(fullRect.width() / (double)wheelSize.width(),
                        fullRect.height() / (double)wheelSize.height());
    QPoint center = fullRect.center();
    int w = wheelSize.width() * scale;
    int h = wheelSize.height() * scale;
    return QRect(center.x() - w/2, center.y() - h/2, w, h);
}

void SteeringWidget::paintEvent(QPaintEvent *) {
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);
    p.setRenderHint(QPainter::SmoothPixmapTransform);
    p.fillRect(rect(), QColor(20,30,40));
    QRect srect = steeringRect();
    p.setPen(Qt::green);
    p.drawText(srect.adjusted(0, 0, 0, -srect.height()/2), Qt::AlignHCenter | Qt::AlignTop,
               QString("Heading: %1°").arg(m_heading,0,'f',1));
    p.save();
    p.translate(srect.center());
    p.rotate(m_heading);
    QSize wheelSize = m_wheel.size();
    double scale = qMin(srect.width()/(double)wheelSize.width(),
                        srect.height()/(double)wheelSize.height());
    p.scale(scale, scale);
    p.drawPixmap(-wheelSize.width()/2, -wheelSize.height()/2, m_wheel);
    p.restore();
}

void SteeringWidget::mousePressEvent(QMouseEvent *ev) {
    if (steeringRect().contains(ev->pos()))
        m_drag = Steering;
}

void SteeringWidget::mouseMoveEvent(QMouseEvent *ev) {
    if (m_drag == Steering) {
        QPoint c = steeringRect().center();
        double dx = ev->pos().x() - c.x();
        double dy = ev->pos().y() - c.y();
        m_heading = qRadiansToDegrees(qAtan2(dy,dx)) - 90;
        if (m_heading < -180) m_heading += 360;
        if (m_heading > 180) m_heading -= 360;
        emit headingChanged(m_heading);
        update();
    }
}

void SteeringWidget::mouseReleaseEvent(QMouseEvent *) {
    m_drag = None;
}
