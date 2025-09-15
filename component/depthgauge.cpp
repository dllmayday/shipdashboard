#include "depthgauge.h"
#include <QPainter>
#include <QLinearGradient>
#include <QPainterPath>

DepthGauge::DepthGauge(QWidget *parent)
    : QWidget(parent), m_depth(0), m_maxDepth(1000) {
    setMinimumSize(100, 300); // 控件最小尺寸
}

void DepthGauge::setDepth(double d) {
    m_depth = qBound(0.0, d, m_maxDepth);
    update();
}

void DepthGauge::setMaxDepth(double d) {
    m_maxDepth = d;
    update();
}

void DepthGauge::paintEvent(QPaintEvent *) {
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);

    QRect fullRect = rect();

    // 顶部文字
    p.setPen(Qt::white);
    p.drawText(QRect(0, 0, rect().width(), 30),
               Qt::AlignCenter,
               QString("Depth: %1 m").arg(m_depth, 0, 'f', 1));

    // 圆角槽区域
    int gaugeTop = 30;
    int gaugeHeight = fullRect.height() - gaugeTop - 20;
    int gaugeWidth = 20;
    QRect gaugeRect((fullRect.width() - gaugeWidth) / 2,
                    gaugeTop,
                    gaugeWidth,
                    gaugeHeight);

    int radius = gaugeWidth / 2; // 圆角半径

    // --- 外层灰色边框 ---
    QRect outerRect = gaugeRect.adjusted(-3, -3, 3, 3);
    p.setPen(QPen(QColor(150, 150, 150), 3));
    p.setBrush(Qt::NoBrush);
    p.drawRoundedRect(outerRect, radius + 3, radius + 3);

    // --- 槽背景（深蓝渐变） ---
    QLinearGradient bgGrad(gaugeRect.topLeft(), gaugeRect.bottomLeft());
    bgGrad.setColorAt(0.0, QColor(0, 50, 120));   // 顶部浅蓝
    bgGrad.setColorAt(1.0, QColor(0, 20, 60));  // 底部深蓝

    p.setPen(QPen(Qt::white, 2));
    p.setBrush(bgGrad);
    p.drawRoundedRect(gaugeRect, radius, radius);

    // --- 水深部分（顶部圆弧 + 绿-青渐变） ---
    double ratio = m_depth / m_maxDepth;
    ratio = qBound(0.0, ratio, 1.0);
    int fillHeight = static_cast<int>(ratio * gaugeRect.height());

    if (fillHeight > 0) {
        QRect fillRect(gaugeRect.left(),
                       gaugeRect.bottom() - fillHeight,
                       gaugeRect.width(),
                       fillHeight);

        QLinearGradient waterGrad(fillRect.topLeft(), fillRect.bottomLeft());
        waterGrad.setColorAt(0.0, QColor(150, 255, 150));   // 顶部黄绿色
        waterGrad.setColorAt(1.0, QColor(0, 255, 180)); // 底部偏青绿

        p.setBrush(waterGrad);
        p.setPen(Qt::NoPen);

        // 顶部圆角
        QPainterPath waterPath;
        waterPath.addRoundedRect(fillRect, radius, radius);

        // 裁剪背景槽范围
        QPainterPath clipPath;
        clipPath.addRoundedRect(gaugeRect, radius, radius);
        p.setClipPath(clipPath);

        p.drawPath(waterPath);
    }
}



