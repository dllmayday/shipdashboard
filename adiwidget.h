#pragma once
#include <QWidget>
#include <QPropertyAnimation>

class ADIWidget : public QWidget {
    Q_OBJECT
    Q_PROPERTY(double pitch READ pitch WRITE setPitch)
    Q_PROPERTY(double roll READ roll WRITE setRoll)

public:
    explicit ADIWidget(QWidget *parent=nullptr);

    double pitch() const { return m_pitch; }
    double roll() const { return m_roll; }

    void setPitch(double p);
    void setRoll(double r);
    void setTargetPitch(double p);
    void setTargetRoll(double r);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    double m_pitch; // 俯仰 [-30, 30]
    double m_roll;  // 横滚 [-90, 90]
    QPropertyAnimation *m_animPitch;
    QPropertyAnimation *m_animRoll;
};
