#include "compasswidget.h"
#include <QPainter>
#include <QtMath>
#include <QRadialGradient>
#include <QLinearGradient>

CompassWidget::CompassWidget(QWidget *parent)
    : QWidget(parent), m_heading(0) {
    setMinimumSize(200, 200);

    m_anim = new QPropertyAnimation(this, "heading");
    m_anim->setDuration(800);
    m_anim->setEasingCurve(QEasingCurve::InOutCubic);
}

void CompassWidget::setHeading(double h) {
    m_heading = fmod(h, 360.0);
    if (m_heading < 0) m_heading += 360.0;
    update();
}

void CompassWidget::setTargetHeading(double h) {
    m_anim->stop();
    m_anim->setStartValue(m_heading);
    m_anim->setEndValue(fmod(h, 360.0));
    m_anim->start();
}

void CompassWidget::paintEvent(QPaintEvent *) {
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);

    int side = qMin(width(), height());
    p.translate(width()/2, height()/2);
    p.scale(side / 220.0, side / 220.0);

    // 背景渐变
    QRadialGradient bgGrad(0,0,110);
    bgGrad.setColorAt(0, QColor(240,240,240));
    bgGrad.setColorAt(1, QColor(60,60,60));
    p.setBrush(bgGrad);
    p.setPen(QPen(Qt::black, 3));
    p.drawEllipse(QPoint(0,0), 100, 100);

    // 刻度
    p.setPen(Qt::white);
    for (int i=0; i<36; i++) {
        p.save();
        p.rotate(i*10);
        p.drawLine(0,-85, 0,-100);
        if (i%9==0) { // 每90度画大刻度并标文字
            p.drawText(-15, -70, 30, 30, Qt::AlignCenter, QString::number(i*10));
        }
        p.restore();
    }

    // 航向指针
    p.save();
    p.rotate(m_heading);
    QPolygon needle;
    needle << QPoint(0, -80) << QPoint(6, 0) << QPoint(-6, 0);
    QLinearGradient grad(0,-80,0,0);
    grad.setColorAt(0, Qt::red);
    grad.setColorAt(1, QColor(80,0,0));
    p.setBrush(grad);
    p.setPen(Qt::NoPen);
    p.drawPolygon(needle);
    p.restore();

    // 中心按钮
    QRadialGradient cg(0,0,10);
    cg.setColorAt(0, QColor(220,220,220));
    cg.setColorAt(1, QColor(50,50,50));
    p.setBrush(cg);
    p.setPen(Qt::black);
    p.drawEllipse(QPoint(0,0), 8, 8);

    // 玻璃反光
    QLinearGradient glass(0,-100,0,100);
    glass.setColorAt(0, QColor(255,255,255,120));
    glass.setColorAt(1, QColor(255,255,255,30));
    p.setBrush(glass);
    p.setPen(Qt::NoPen);
    p.drawEllipse(QPoint(0,0), 100, 100);
}
