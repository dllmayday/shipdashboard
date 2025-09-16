#ifndef THROTTLEWIDGET_H
#define THROTTLEWIDGET_H

#include <QWidget>

class ThrottleWidget : public QWidget {
    Q_OBJECT
public:
    explicit ThrottleWidget(QWidget *parent = nullptr);
    double speed() const { return m_speed; }

signals:
    void speedChanged(double speed);

protected:
    void paintEvent(QPaintEvent *) override;
    void mousePressEvent(QMouseEvent *) override;
    void mouseMoveEvent(QMouseEvent *) override;
    void mouseReleaseEvent(QMouseEvent *) override;
    QRect throttleRect() const;

private:
    enum DragMode { None, Throttle };
    DragMode m_drag;
    double m_speed;
};

#endif // THROTTLEWIDGET_H
