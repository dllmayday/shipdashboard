#include "adiwidget.h"
#include <QPainter>
#include <QtMath>
#include <QLinearGradient>
#include <QRadialGradient>

ADIWidget::ADIWidget(QWidget *parent)
    : QWidget(parent), m_pitch(0), m_roll(0) {
    setMinimumSize(200, 200);

    m_animPitch = new QPropertyAnimation(this, "pitch");
    m_animPitch->setDuration(800);
    m_animPitch->setEasingCurve(QEasingCurve::InOutCubic);

    m_animRoll = new QPropertyAnimation(this, "roll");
    m_animRoll->setDuration(800);
    m_animRoll->setEasingCurve(QEasingCurve::InOutCubic);
}

void ADIWidget::setPitch(double p) {
    m_pitch = qBound(-30.0, p, 30.0);
    update();
}
void ADIWidget::setRoll(double r) {
    m_roll = qBound(-90.0, r, 90.0);
    update();
}
void ADIWidget::setTargetPitch(double p) {
    m_animPitch->stop();
    m_animPitch->setStartValue(m_pitch);
    m_animPitch->setEndValue(qBound(-30.0, p, 30.0));
    m_animPitch->start();
}
void ADIWidget::setTargetRoll(double r) {
    m_animRoll->stop();
    m_animRoll->setStartValue(m_roll);
    m_animRoll->setEndValue(qBound(-90.0, r, 90.0));
    m_animRoll->start();
}

void ADIWidget::paintEvent(QPaintEvent *) {
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);

    int side = qMin(width(), height());
    p.translate(width()/2, height()/2);
    p.scale(side/220.0, side/220.0);

    // 背景外框
    QRadialGradient bg(0,0,110);
    bg.setColorAt(0, QColor(200,200,200));
    bg.setColorAt(1, QColor(50,50,50));
    p.setBrush(bg);
    p.setPen(QPen(Qt::black, 3));
    p.drawEllipse(QPoint(0,0),100,100);

    // 姿态球
    p.save();
    p.rotate(m_roll);

    int offset = int(m_pitch * 2); // 俯仰上下移动
    QRect rect(-200, -200+offset, 400, 400);

    // 天空渐变
    QLinearGradient sky(rect.topLeft(), rect.center());
    sky.setColorAt(0, QColor(80,150,255));
    sky.setColorAt(1, QColor(150,200,255));
    p.fillRect(rect.adjusted(0,0,0,-200), sky);

    // 地面渐变
    QLinearGradient ground(rect.center(), rect.bottomLeft());
    ground.setColorAt(0, QColor(200,120,50));
    ground.setColorAt(1, QColor(120,70,30));
    p.fillRect(rect.adjusted(0,200,0,0), ground);

    // 地平线
    p.setPen(QPen(Qt::white,2));
    p.drawLine(-200,0+offset,200,0+offset);

    p.restore();

    // 固定指针（白色小刻度）
    p.setPen(QPen(Qt::white,3));
    p.drawLine(-20,0,20,0);
    p.drawLine(0,-20,0,20);

    // 中心按钮
    QRadialGradient cg(0,0,10);
    cg.setColorAt(0, QColor(220,220,220));
    cg.setColorAt(1, QColor(50,50,50));
    p.setBrush(cg);
    p.setPen(Qt::black);
    p.drawEllipse(QPoint(0,0),8,8);

    // 玻璃反光
    QLinearGradient glass(0,-100,0,100);
    glass.setColorAt(0, QColor(255,255,255,100));
    glass.setColorAt(1, QColor(255,255,255,20));
    p.setBrush(glass);
    p.setPen(Qt::NoPen);
    p.drawEllipse(QPoint(0,0),100,100);
}
