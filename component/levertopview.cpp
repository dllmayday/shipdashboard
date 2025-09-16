#include "levertopview.h"
#include <QPainter>
#include <QMouseEvent>
#include <QtMath>

JoystickWidget::JoystickWidget(QWidget *parent)
    : QWidget(parent), m_position(0.0), m_dragging(false)
{
    setMinimumSize(60, 150); // 控件大小

    // 定时器用于松开回到中心动画
    m_timer = new QTimer(this);
    connect(m_timer, &QTimer::timeout, this, &JoystickWidget::onReturnToCenter);
}

void JoystickWidget::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);

    // 背景
    p.setBrush(QColor(230, 230, 230));
    p.drawRect(rect());

    int w = width();
    int h = height();
    int knobHeight = 40;

    // 绘制拨杆轨道（灰色条）
    p.setBrush(QColor(180, 180, 180));
    int trackW = w / 3;
    int trackX = (w - trackW) / 2;
    p.drawRect(trackX, 10, trackW, h - 20);

    // 绘制拨杆（圆形或椭圆）
    p.setBrush(QColor(100, 150, 250));
    int knobY = h/2 - knobHeight/2 + m_position;
    p.drawEllipse(trackX - 5, knobY, trackW + 10, knobHeight);
}

void JoystickWidget::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        m_dragging = true;
        m_lastY = event->y();
        m_timer->stop();
    }
}

void JoystickWidget::mouseMoveEvent(QMouseEvent *event)
{
    if(m_dragging)
    {
        int dy = event->y() - m_lastY;
        m_position += dy;
        m_lastY = event->y();

        // 限制拨杆范围
        int maxOffset = height()/2 - 20;
        if(m_position > maxOffset) m_position = maxOffset;
        if(m_position < -maxOffset) m_position = -maxOffset;

        update();
    }
}

void JoystickWidget::mouseReleaseEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
    if(m_dragging)
    {
        m_dragging = false;
        m_timer->start(15); // 开启回中动画
    }
}

void JoystickWidget::onReturnToCenter()
{
    // 简单线性回中
    if(qAbs(m_position) < 1)
    {
        m_position = 0;
        m_timer->stop();
    }
    else
    {
        m_position *= 0.8; // 缓慢回到中心
    }
    update();
}
