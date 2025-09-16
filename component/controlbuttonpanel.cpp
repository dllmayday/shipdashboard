#include "controlbuttonpanel.h"
#include "controlbutton.h"
#include <QVBoxLayout>
#include <QHBoxLayout>

ControlButtonPanel::ControlButtonPanel(QWidget *parent)
    : QWidget(parent)
{
    auto *layout = new QVBoxLayout(this);
    auto *row1 = new QHBoxLayout;
    auto *row2 = new QHBoxLayout;

    LightButton *btnAccelerate = new LightButton(this);
    btnAccelerate->setText(tr("加速"));
    LightButton *btnDecelerate = new LightButton( this);
    btnDecelerate->setText(tr("减速"));
    LightButton *btnAscend = new LightButton(this);
    btnAscend->setText(tr("上浮"));
    LightButton *btnDescend = new LightButton(this);
    btnDescend->setText(tr("下潜"));

    row1->addWidget(btnAccelerate);
    row1->addWidget(btnDecelerate);
    row2->addWidget(btnAscend);
    row2->addWidget(btnDescend);

    layout->addLayout(row1);
    layout->addLayout(row2);
    setLayout(layout);
}
