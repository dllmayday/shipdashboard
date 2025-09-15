#pragma once
#include <QWidget>
#include <QTimer>
#include <QVector>

class RadarWidget : public QWidget {
    Q_OBJECT
public:
    explicit RadarWidget(QWidget *parent=nullptr);

protected:
    void paintEvent(QPaintEvent *event) override;

private slots:
    void updateAngle();
    void updateTargets();

private:
    struct Target {
        QPoint pos;
        int alpha;   // 亮度 (0~255)
        int delta;   // 每次更新的变化方向
    };

    double m_angle;   // 扫描角度
    QTimer *m_timer;
    QTimer *m_targetTimer;
    QVector<Target> m_targets;
};
