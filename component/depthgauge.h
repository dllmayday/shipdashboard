#ifndef DEPTHGAUGE_H
#define DEPTHGAUGE_H

#include <QWidget>

class DepthGauge : public QWidget {
    Q_OBJECT
public:
    explicit DepthGauge(QWidget *parent = nullptr);

    void setDepth(double d);
    void setMaxDepth(double d);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    double m_depth;    // 当前深度
    double m_maxDepth; // 最大深度
};

#endif // DEPTHGAUGE_H
