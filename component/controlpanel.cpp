#include "controlpanel.h"
#include <QPainter>
#include <QMouseEvent>
#include <QtMath>

ControlPanel::ControlPanel(QWidget *parent)
    : QWidget(parent), m_drag(None), m_heading(0), m_speed(50) {
    setMinimumSize(700,300);
    // 加载舵盘图片（建议透明背景的 PNG）
    m_wheel = QPixmap(":/images/steering_wheel.png"); // 放到 qrc 资源里
}

QRect ControlPanel::steeringRect() const {

    QRect fullRect = rect();
    QRect leftHalf = fullRect.adjusted(0, 0, -fullRect.width()/2, 0); // 左半区

    QSize wheelSize = m_wheel.size();
    if (wheelSize.isEmpty())
        return leftHalf; // 防止图片未加载

    // 计算缩放比例
    double scale = qMin(leftHalf.width() / (double)wheelSize.width(),
                        leftHalf.height() / (double)wheelSize.height());

    // 返回缩放后的舵盘实际矩形
    QPoint center = leftHalf.center();
    int w = wheelSize.width() * scale;
    int h = wheelSize.height() * scale;
    return QRect(center.x() - w/2, center.y() - h/2, w, h);

}

QRect ControlPanel::throttleRect() const {
    // 油门杆固定尺寸 60x250，右半区居中
    int throttleWidth = 60;
    int throttleHeight = 250;
    QRect fullRect = rect();

    int x = fullRect.width()/2 + (fullRect.width()/2 - throttleWidth)/2; // 右半区水平居中
    int y = (fullRect.height() - throttleHeight)/2;                       // 垂直居中

    return QRect(x, y, throttleWidth, throttleHeight);
}

void ControlPanel::paintEvent(QPaintEvent *) {
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);
    p.setRenderHint(QPainter::SmoothPixmapTransform);

    // 背景
    p.fillRect(rect(), QColor(20,30,40));

    // 舵盘（使用PNG）
    QRect fullRect = rect(); // 假设你在 paintEvent 中使用

    // 左半边绘制舵盘
    QRect srect = fullRect.adjusted(0, 0, -fullRect.width()/2, 0); // 左半区

    // 舵盘文字（上方）
    p.setPen(Qt::green);
    p.drawText(srect.adjusted(0, 0, 0, -srect.height()/2), Qt::AlignHCenter | Qt::AlignTop,
               QString("Heading: %1°").arg(m_heading,0,'f',1));

    p.save();
    p.translate(srect.center());    // 移动到舵盘中心
    p.rotate(m_heading);            // 按角度旋转
    QSize wheelSize = m_wheel.size();
    double scale = qMin(srect.width()/(double)wheelSize.width(),
                        srect.height()/(double)wheelSize.height());
    p.scale(scale, scale);
    p.drawPixmap(-wheelSize.width()/2, -wheelSize.height()/2, m_wheel);
    p.restore();

    // 右半边绘制油门杆（固定尺寸 60x250）
    int throttleWidth = 60;
    int throttleHeight = 250;
    QRect trect(
        fullRect.width()/2 + (fullRect.width()/2 - throttleWidth)/2,  // 右半区水平居中
        (fullRect.height() - throttleHeight)/2,                       // 垂直居中
        throttleWidth,
        throttleHeight
    );

    // 油门文字（上方）
    p.setPen(Qt::green);
    p.drawText(fullRect.adjusted(fullRect.width()/2, 0,0,  -fullRect.height()/2), Qt::AlignHCenter | Qt::AlignTop,
               QString("Speed: %1%").arg(m_speed,0,'f',0));

    // 绘制油门杆背景
    p.setBrush(QColor(50,50,50));
    p.setPen(QPen(Qt::white,2));
    p.drawRoundedRect(trect,5,5);

    // 绘制油门旋钮
    int y = trect.bottom() - (m_speed/100.0)*trect.height();
    QRect knob(trect.left()-5, y-10, trect.width()+10, 20);

    QLinearGradient lg(knob.topLeft(), knob.bottomRight());
    lg.setColorAt(0,QColor(200,200,200));
    lg.setColorAt(1,QColor(120,120,120));
    p.setBrush(lg);
    p.setPen(QPen(Qt::black,1));
    p.drawRoundedRect(knob,5,5);

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
