#ifndef LEVERGATE_H
#define LEVERGATE_H

#include <QWidget>
#include <QPropertyAnimation>

class LeverGate : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(double angle READ angle WRITE setAngle NOTIFY angleChanged)

public:
    explicit LeverGate(QWidget *parent = nullptr);

    // angle in degrees: negative = up, positive = down
    double angle() const { return m_angle; }
    void setAngle(double a);

    QSize sizeHint() const override { return QSize(120, 200); }

signals:
    void angleChanged(double newAngle);

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private:
    double m_angle = 0.0; // current lever angle
    bool m_dragging = false;
    int m_pressY = 0;

    QPropertyAnimation *m_returnAnim = nullptr;
};

#endif // LEVERGATE_H
