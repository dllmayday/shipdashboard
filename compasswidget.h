#pragma once
#include <QWidget>
#include <QPropertyAnimation>

class CompassWidget : public QWidget {
    Q_OBJECT
    Q_PROPERTY(double heading READ heading WRITE setHeading)

public:
    explicit CompassWidget(QWidget *parent = nullptr);

    double heading() const { return m_heading; }
    void setHeading(double h);
    void setTargetHeading(double h);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    double m_heading; // 航向角度 [0, 360)
    QPropertyAnimation *m_anim;
};
