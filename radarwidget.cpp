#include "radarwidget.h"
#include <QPainter>
#include <QtMath>
#include <QRadialGradient>
#include <QLinearGradient>
#include <QRandomGenerator>
#include <QPainterPath>

RadarWidget::RadarWidget(QWidget *parent)
    : QWidget(parent), m_angle(0) {
    setMinimumSize(200,200);

    // 扫描线定时器
    m_timer = new QTimer(this);
    connect(m_timer, &QTimer::timeout, this, &RadarWidget::updateAngle);
    m_timer->start(50); // 20fps

    // 目标点初始化
    m_targets = {
        { QPoint(40,-30), 200, -15 },
        { QPoint(-50,60), 150, +10 },
        { QPoint(70,20), 180, -20 },
        { QPoint(-30,-50), 220, -10 }
    };

    // 目标闪烁定时器
    m_targetTimer = new QTimer(this);
    connect(m_targetTimer, &QTimer::timeout, this, &RadarWidget::updateTargets);
    m_targetTimer->start(120); // 大约8fps
}

void RadarWidget::updateAngle() {
    m_angle += 2;
    if (m_angle >= 360) m_angle -= 360;
    update();
}

void RadarWidget::updateTargets() {
    for (auto &t : m_targets) {
        t.alpha += t.delta;
        if (t.alpha > 255) { t.alpha = 255; t.delta = -QRandomGenerator::global()->bounded(5,20); }
        if (t.alpha < 80)  { t.alpha = 80;  t.delta = +QRandomGenerator::global()->bounded(5,20); }
    }
    update();
}

void RadarWidget::paintEvent(QPaintEvent *) {
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);

    int side = qMin(width(), height());
    p.translate(width()/2, height()/2);
    p.scale(side/220.0, side/220.0);

    // 背景黑色渐变
    QRadialGradient bg(0,0,110);
    bg.setColorAt(0, QColor(0,40,0));
    bg.setColorAt(1, QColor(0,0,0));
    p.setBrush(bg);
    p.setPen(Qt::NoPen);
    p.drawEllipse(QPoint(0,0),100,100);

    // 同心圆刻度
    p.setPen(QPen(QColor(0,150,0),1));
    for (int r=20; r<=100; r+=20) {
        p.drawEllipse(QPoint(0,0), r, r);
    }

    // 十字线
    p.drawLine(-100,0,100,0);
    p.drawLine(0,-100,0,100);

    // 扫描扇形
    p.save();
    p.rotate(m_angle);
    QPainterPath sector;
    sector.moveTo(0,0);
    sector.arcTo(-100,-100,200,200,0,30);
    sector.closeSubpath();

    QLinearGradient grad(0,0,100,0);
    grad.setColorAt(0, QColor(0,255,0,150));
    grad.setColorAt(1, QColor(0,255,0,0));
    p.setBrush(grad);
    p.setPen(Qt::NoPen);
    p.drawPath(sector);
    p.restore();

    // 目标点闪烁
    for (const auto &t : m_targets) {
        QColor c(0,255,0,t.alpha);
        p.setBrush(c);
        p.setPen(Qt::NoPen);
        p.drawEllipse(t.pos,4,4);
    }

    // 玻璃反光
    QLinearGradient glass(0,-100,0,100);
    glass.setColorAt(0, QColor(255,255,255,40));
    glass.setColorAt(1, QColor(255,255,255,10));
    p.setBrush(glass);
    p.setPen(Qt::NoPen);
    p.drawEllipse(QPoint(0,0),100,100);
}
