#include <QApplication>
#include <QVBoxLayout>
#include "controlpanel.h"
#include "radarwidget.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    QWidget w;

    QVBoxLayout *layout = new QVBoxLayout(&w);
    RadarWidget *radar = new RadarWidget;
    ControlPanel *ctrl = new ControlPanel;

    layout->addWidget(radar,1);
    layout->addWidget(ctrl,0);

    w.show();
    return a.exec();
}
