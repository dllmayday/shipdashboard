#include "gaugewidget.h"
#include <QPainter>
#include <QtMath>
#include <QLinearGradient>
#include <QRadialGradient>

GaugeWidget::GaugeWidget(QWidget *parent)
    : QWidget(parent), m_min(0), m_max(100), m_value(0), m_label("Gauge") {
    setMinimumSize(200, 200);

    // 动画初始化
    m_anim = new QPropertyAnimation(this, "value");
    m_anim->setDuration(600);         // 动画时长 0.6s
    m_anim->setEasingCurve(QEasingCurve::InOutCubic);
}

void GaugeWidget::setRange(double min, double max) {
    m_min = min; m_max = max;
    update();
}

void GaugeWidget::setValue(double v) {
    m_value = qBound(m_min, v, m_max);
    update();
}

void GaugeWidget::setTargetValue(double v) {
    m_anim->stop();
    m_anim->setStartValue(m_value);
    m_anim->setEndValue(qBound(m_min, v, m_max));
    m_anim->start();
}

void GaugeWidget::setLabel(const QString &label) {
    m_label = label;
    update();
}

void GaugeWidget::paintEvent(QPaintEvent *) {
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);

    int side = qMin(width(), height());
    p.translate(width()/2, height()/2);
    p.scale(side / 220.0, side / 220.0);

    // ===== 背景渐变 =====
    QRadialGradient bgGrad(0, 0, 120);
    bgGrad.setColorAt(0, QColor(240,240,240));
    bgGrad.setColorAt(1, QColor(100,100,100));
    p.setBrush(bgGrad);
    p.setPen(QPen(Qt::black, 3));
    p.drawEllipse(QPoint(0,0), 100, 100);

    // ===== 刻度 =====
    p.setPen(QPen(Qt::white, 2));
    for (int i=0; i<=10; i++) {
        double angle = -120 + i * 240.0 / 10.0;
        p.save();
        p.rotate(angle);
        p.drawLine(0, -85, 0, -100);
        p.restore();
    }

    // ===== 指针 =====
    double ratio = (m_value - m_min) / (m_max - m_min);
    double angle = -120 + ratio * 240;

    p.save();
    p.rotate(angle);
    QPolygon needle;
    needle << QPoint(0, -80) << QPoint(6, 0) << QPoint(-6, 0);
    QLinearGradient needleGrad(0, -80, 0, 0);
    needleGrad.setColorAt(0, Qt::red);
    needleGrad.setColorAt(1, QColor(80,0,0));
    p.setBrush(needleGrad);
    p.setPen(Qt::NoPen);
    p.drawPolygon(needle);
    p.restore();

    // 中心金属圆
    QRadialGradient centerGrad(0,0,10);
    centerGrad.setColorAt(0, QColor(200,200,200));
    centerGrad.setColorAt(1, QColor(80,80,80));
    p.setBrush(centerGrad);
    p.setPen(Qt::black);
    p.drawEllipse(QPoint(0,0), 8, 8);

    // ===== 玻璃高光 =====
    QLinearGradient glassGrad(0, -100, 0, 100);
    glassGrad.setColorAt(0, QColor(255,255,255,120));
    glassGrad.setColorAt(0.5, QColor(255,255,255,40));
    glassGrad.setColorAt(1, QColor(255,255,255,10));
    p.setBrush(glassGrad);
    p.setPen(Qt::NoPen);
    p.drawEllipse(QPoint(0,0), 100, 100);

    // ===== 文本 =====
    QFont font = p.font();
    font.setBold(true);
    font.setPointSize(12);
    p.setFont(font);
    p.setPen(Qt::white);
    p.drawText(-60, 120, QString("%1: %2").arg(m_label).arg(m_value, 0, 'f', 0));
}
