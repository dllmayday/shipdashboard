#include <QApplication>
#include <QHBoxLayout>
#include <QTimer>
#include <QtMath>

#include "gaugewidget.h"
#include "compasswidget.h"
#include "adiwidget.h"
#include "radarwidget.h"
#include "depthgauge.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    QWidget w;
    w.setWindowTitle("船舶驾驶台 3D 仿真仪表盘");
    w.resize(1000, 500);

    GaugeWidget *speed = new GaugeWidget;
    speed->setRange(0, 40);
    speed->setLabel("Speed (knots)");

    GaugeWidget *depth = new GaugeWidget;
    depth->setRange(0, 500);
    depth->setLabel("Depth (m)");

    CompassWidget *compass = new CompassWidget;
    ADIWidget *adi = new ADIWidget;
    RadarWidget *radar = new RadarWidget;

    DepthGauge *gauge = new DepthGauge();
    gauge->setMaxDepth(200);
    gauge->setDepth(150);

    QHBoxLayout *layout = new QHBoxLayout(&w);
    layout->addWidget(speed);
    layout->addWidget(depth);
    layout->addWidget(compass);
    layout->addWidget(adi);
    layout->addWidget(radar);
    layout->addWidget(gauge);

    // 模拟数据
    QTimer *timer = new QTimer(&w);
    QObject::connect(timer, &QTimer::timeout, [=]() {
        static double t = 0; t+=0.1;
        speed->setTargetValue(20+10*qSin(t));
        depth->setTargetValue(200+150*qCos(t/2));
        compass->setTargetHeading(fmod(t*20,360));
        adi->setTargetPitch(10*qSin(t/3));
        adi->setTargetRoll(30*qCos(t/4));
    });
    timer->start(100);

    w.show();
    return a.exec();
}
