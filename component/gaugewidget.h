#pragma once
#include <QWidget>
#include <QPropertyAnimation>

class GaugeWidget : public QWidget {
    Q_OBJECT
    Q_PROPERTY(double value READ value WRITE setValue)

public:
    explicit GaugeWidget(QWidget *parent = nullptr);

    void setRange(double min, double max);
    void setTargetValue(double v);   // 平滑动画到目标值
    void setLabel(const QString &label);

    double value() const { return m_value; }
    void setValue(double v);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    double m_min, m_max;
    double m_value;
    QString m_label;

    QPropertyAnimation *m_anim;
};
