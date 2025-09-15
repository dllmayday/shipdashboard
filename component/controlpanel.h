#pragma once
#include <QWidget>
#include <QTimer>

class ControlPanel : public QWidget {
    Q_OBJECT
public:
    explicit ControlPanel(QWidget *parent=nullptr);

    double heading() const { return m_heading; }   // 当前方向
    double speed() const { return m_speed; }       // 当前航速

signals:
    void headingChanged(double);
    void speedChanged(double);

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private:
    enum DragMode { None, Steering, Throttle };
    DragMode m_drag;

    double m_heading;   // 方向角（-180~180）
    double m_speed;     // 航速百分比（0~100）

    QRect steeringRect() const;
    QRect throttleRect() const;
};
