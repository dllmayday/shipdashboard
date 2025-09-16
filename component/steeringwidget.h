#ifndef STEERINGWIDGET_H
#define STEERINGWIDGET_H

#include <QWidget>
#include <QPixmap>

class SteeringWidget : public QWidget {
    Q_OBJECT
public:
    explicit SteeringWidget(QWidget *parent = nullptr);
    double heading() const { return m_heading; }

signals:
    void headingChanged(double heading);

protected:
    void paintEvent(QPaintEvent *) override;
    void mousePressEvent(QMouseEvent *) override;
    void mouseMoveEvent(QMouseEvent *) override;
    void mouseReleaseEvent(QMouseEvent *) override;
    QRect steeringRect() const;

private:
    enum DragMode { None, Steering };
    DragMode m_drag;
    double m_heading;
    QPixmap m_wheel;
};

#endif // STEERINGWIDGET_H
