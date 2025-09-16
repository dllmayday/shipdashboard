#include <QApplication>
#include <QVBoxLayout>
#include <QDebug>
#include "controlpanel.h"
#include "radarwidget.h"
#include "gaugewidget.h"
#include "compasswidget.h"
#include "adiwidget.h"
#include "radarwidget.h"
#include "levergate.h"


int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    QWidget w;
    w.setWindowTitle("驾驶台仿真");
    w.resize(1000, 600);
    w.setStyleSheet("QWidget { background-image: url(:/images/background.png); }");

    QWidget*  w_dashboard = new QWidget();
    GaugeWidget *speed = new GaugeWidget;
    speed->setRange(0, 40);
    speed->setLabel("Speed (knots)");

    GaugeWidget *depth = new GaugeWidget;
    depth->setRange(0, 500);
    depth->setLabel("Depth (m)");

    CompassWidget *compass = new CompassWidget;
    ADIWidget *adi = new ADIWidget;
    RadarWidget *radar = new RadarWidget;

    QHBoxLayout *dashboard_layout = new QHBoxLayout(w_dashboard);
    dashboard_layout->addWidget(speed);
    dashboard_layout->addWidget(depth);
    dashboard_layout->addWidget(compass);
    dashboard_layout->addWidget(adi);
    dashboard_layout->addWidget(radar);


    QVBoxLayout *layout = new QVBoxLayout(&w);

    layout->addWidget(w_dashboard,1);

    ControlPanel *ctrl = new ControlPanel;
    layout->addWidget(ctrl,0);

    w.show();
    return a.exec();
}
