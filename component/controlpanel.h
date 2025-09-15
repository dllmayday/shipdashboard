#ifndef CONTROLPANEL_H
#define CONTROLPANEL_H

#include <QWidget>
#include <QPixmap>

class ControlPanel : public QWidget {
    Q_OBJECT
public:
    explicit ControlPanel(QWidget *parent=nullptr);

signals:
    void headingChanged(double);
    void speedChanged(double);

protected:
    void paintEvent(QPaintEvent*) override;
    void mousePressEvent(QMouseEvent*) override;
    void mouseMoveEvent(QMouseEvent*) override;
    void mouseReleaseEvent(QMouseEvent*) override;

private:
    enum DragMode { None, Steering, Throttle };
    DragMode m_drag;
    double m_heading;   // 舵盘角度
    double m_speed;     // 航速（百分比）
    QPixmap m_wheel;    // 舵盘 PNG 图片

    QRect steeringRect() const;
    QRect throttleRect() const;
};

#endif // CONTROLPANEL_H
