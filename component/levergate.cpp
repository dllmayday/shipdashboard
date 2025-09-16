#include "levergate.h"
#include <QPainter>
#include <QMouseEvent>
#include <QPropertyAnimation>
#include <QtMath>

LeverGate::LeverGate(QWidget *parent)
    : QWidget(parent)
{
    m_returnAnim = new QPropertyAnimation(this, "angle", this);
    m_returnAnim->setDuration(300);
    m_returnAnim->setEasingCurve(QEasingCurve::OutBack);
}

void LeverGate::setAngle(double a)
{
    double clamped = qBound(-45.0, a, 45.0); // 限制范围 -45° ~ 45°
    if (!qFuzzyCompare(clamped + 1.0, m_angle + 1.0)) {
        m_angle = clamped;
        update();
        emit angleChanged(m_angle);
    }
}

void LeverGate::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);

    p.fillRect(rect(), QColor(30, 35, 40)); // 背景

    // 基座
    QRect base(rect().center().x()-30, rect().bottom()-30, 60, 20);
    p.setBrush(QColor(100,100,100));
    p.setPen(Qt::NoPen);
    p.drawRoundedRect(base, 5, 5);

    // 拨杆 (pivot bottom-center)
    p.save();
    QPoint pivot(rect().center().x(), rect().bottom()-30);
    p.translate(pivot);
    p.rotate(m_angle);

    QRect lever(-8, -120, 16, 120); // 宽16，高120，从 pivot 向上
    QLinearGradient grad(lever.topLeft(), lever.bottomLeft());
    grad.setColorAt(0, QColor(180,180,180));
    grad.setColorAt(1, QColor(90,90,90));
    p.setBrush(grad);
    p.setPen(QPen(Qt::black, 1));
    p.drawRoundedRect(lever, 6, 6);

    // 杆顶的手柄
    QRect handle(-20, -140, 40, 25);
    QLinearGradient hgrad(handle.topLeft(), handle.bottomLeft());
    hgrad.setColorAt(0, QColor(220,220,220));
    hgrad.setColorAt(1, QColor(120,120,120));
    p.setBrush(hgrad);
    p.drawRoundedRect(handle, 10, 10);

    p.restore();
}

void LeverGate::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        m_dragging = true;
        m_pressY = event->pos().y();
        if (m_returnAnim->state() == QAbstractAnimation::Running)
            m_returnAnim->stop();
    }
}

void LeverGate::mouseMoveEvent(QMouseEvent *event)
{
    if (m_dragging) {
        int dy = m_pressY - event->pos().y();
        setAngle(dy / 2.0); // 拖动距离映射到角度
    }
}

void LeverGate::mouseReleaseEvent(QMouseEvent *event)
{
    if (m_dragging && event->button() == Qt::LeftButton) {
        m_dragging = false;
        // 松开时回到 0°
        m_returnAnim->stop();
        m_returnAnim->setStartValue(m_angle);
        m_returnAnim->setEndValue(0.0);
        m_returnAnim->start();
    }
}
