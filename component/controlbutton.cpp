#pragma execution_character_set("utf-8")

#include "controlbutton.h"
#include "qpainter.h"
#include "qpainterpath.h"
#include "qevent.h"
#include "qtimer.h"
#include "qdebug.h"


LightButton::LightButton(QWidget *parent) : QWidget(parent)
{
    setMinimumSize(QSize(64,64));
    setMaximumSize(QSize(96,96));
    text = "";
    textColor = QColor(255, 255, 255);
    alarmColor = QColor(255, 107, 107);
    normalColor = QColor(10, 10, 10);

    pressedColor = QColor(80, 160, 255); // 鼠标按下时高亮色，可自定义
    isPressed = false;

    borderOutColorStart = QColor(255, 255, 255);
    borderOutColorEnd = QColor(166, 166, 166);

    borderInColorStart = QColor(166, 166, 166);
    borderInColorEnd = QColor(255, 255, 255);

    bgColor = QColor(100, 184, 255);

    showRect = false;
    showOverlay = true;
    overlayColor = QColor(255, 255, 255);

    canMove = false;
    this->installEventFilter(this);

    isAlarm = false;
    timerAlarm = new QTimer(this);
    connect(timerAlarm, SIGNAL(timeout()), this, SLOT(alarm()));
    timerAlarm->setInterval(500);
}

bool LightButton::eventFilter(QObject *watched, QEvent *event)
{
    if (canMove) {
        static QPoint lastPoint;
        static bool pressed = false;
        QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);

        if (mouseEvent->type() == QEvent::MouseButtonPress) {
            if (this->rect().contains(mouseEvent->pos()) && (mouseEvent->button() == Qt::LeftButton)) {
                lastPoint = mouseEvent->pos();
                pressed = true;
            }
        } else if (mouseEvent->type() == QEvent::MouseMove && pressed) {
            int dx = mouseEvent->pos().x() - lastPoint.x();
            int dy = mouseEvent->pos().y() - lastPoint.y();
            this->move(this->x() + dx, this->y() + dy);
        } else if (mouseEvent->type() == QEvent::MouseButtonRelease && pressed) {
            pressed = false;
        }
    }

    return QWidget::eventFilter(watched, event);
}

void LightButton::paintEvent(QPaintEvent *)
{
    int width = this->width();
    int height = this->height();
    int side = qMin(width*0.7, height*0.7);

    //绘制准备工作,启用反锯齿,平移坐标轴中心,等比例缩放
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);

    QColor useBgColor = bgColor;
    if (isPressed) {
        useBgColor = pressedColor;
    }

    if (showRect) {
        // 3D立体矩形：按下时整体下移，阴影变浅，高光变弱
        QRect rect = this->rect();
        int pressOffset = isPressed ? rect.height() / 10 : 0;
        QRect mainRect = rect.adjusted(0, pressOffset, 0, pressOffset);

        // 阴影
        QLinearGradient shadowGradient(rect.topLeft(), rect.bottomLeft());
        if (isPressed) {
            shadowGradient.setColorAt(0, QColor(0,0,0,20));
            shadowGradient.setColorAt(1, QColor(0,0,0,60));
        } else {
            shadowGradient.setColorAt(0, QColor(0,0,0,40));
            shadowGradient.setColorAt(1, QColor(0,0,0,120));
        }
        painter.setPen(Qt::NoPen);
        painter.setBrush(shadowGradient);
        painter.drawRoundedRect(rect, 5, 5);

        // 高光
        QLinearGradient highlightGradient(rect.topLeft(), rect.bottomLeft());
        if (isPressed) {
            highlightGradient.setColorAt(0, QColor(255,255,255,80));
            highlightGradient.setColorAt(0.4, QColor(255,255,255,20));
            highlightGradient.setColorAt(1, QColor(255,255,255,0));
        } else {
            highlightGradient.setColorAt(0, QColor(255,255,255,180));
            highlightGradient.setColorAt(0.4, QColor(255,255,255,60));
            highlightGradient.setColorAt(1, QColor(255,255,255,0));
        }
        painter.setBrush(highlightGradient);
        painter.drawRoundedRect(rect.adjusted(2,2,-2,-rect.height()/2 + pressOffset), 5, 5);

        // 主色
        painter.setBrush(useBgColor);
        painter.drawRoundedRect(mainRect.adjusted(0,0,0, -rect.height()/4), 5, 5);

        // 绘制文字
        if (!text.isEmpty()) {
            QFont font;
            font.setPixelSize(side - 20);
            painter.setFont(font);
            painter.setPen(textColor);
            painter.drawText(mainRect, Qt::AlignCenter, text);
        }
    } else {
        // 3D立体圆形：按下时整体下移，阴影变浅，高光变弱
        int pressOffset = isPressed ? 10 : 0;
        painter.translate(width / 2, height / 2.5 + pressOffset);
        painter.scale(side / 200.0, side / 200.0);

        // 阴影
        QRadialGradient shadowGradient(0, 40, 100, 0, 40);
        if (isPressed) {
            shadowGradient.setColorAt(0.7, QColor(240,240,240,150));
            shadowGradient.setColorAt(1, QColor(240,240,240,240));
        } else {
            shadowGradient.setColorAt(0.7, QColor(240,240,240,120));
            shadowGradient.setColorAt(1, QColor(240,240,240,200));
        }
        painter.save();
        painter.setPen(Qt::NoPen);
        painter.setBrush(shadowGradient);
        painter.drawEllipse(-99, -60, 198, 198); // 偏下
        painter.restore();

        // 外边框
        drawBorderOut(&painter);
        // 内边框
        drawBorderIn(&painter);

        // 主色
        painter.save();
        painter.setPen(Qt::NoPen);
        painter.setBrush(useBgColor);
        painter.drawEllipse(-80, -80, 160, 160);
        painter.restore();

        // 高光
        QRadialGradient highlightGradient(-40, -40, 100, -40, -40);
        if (isPressed) {
            highlightGradient.setColorAt(0, QColor(255,255,255,80));
            highlightGradient.setColorAt(0.5, QColor(255,255,255,20));
            highlightGradient.setColorAt(1, QColor(255,255,255,0));
        } else {
            highlightGradient.setColorAt(0, QColor(255,255,255,180));
            highlightGradient.setColorAt(0.5, QColor(255,255,255,60));
            highlightGradient.setColorAt(1, QColor(255,255,255,0));
        }
        painter.save();
        painter.setPen(Qt::NoPen);
        painter.setBrush(highlightGradient);
        painter.drawEllipse(-80, -80, 160, 100);
        painter.restore();

        // 居中文字
        drawText(&painter);
        // 遮罩层
        drawOverlay(&painter);
    }
}
void LightButton::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        isPressed = true;
        update();
    }
    QWidget::mousePressEvent(event);
}

void LightButton::mouseReleaseEvent(QMouseEvent *event)
{
    if (isPressed) {
        isPressed = false;
        update();
    }
    QWidget::mouseReleaseEvent(event);
}

void LightButton::drawBorderOut(QPainter *painter)
{
    int radius = 99;
    painter->save();
    painter->setPen(Qt::NoPen);
    QLinearGradient borderGradient(0, -radius, 0, radius);
    borderGradient.setColorAt(0, borderOutColorStart);
    borderGradient.setColorAt(1, borderOutColorEnd);
    painter->setBrush(borderGradient);
    painter->drawEllipse(-radius, -radius, radius * 2, radius * 2);
    painter->restore();
}

void LightButton::drawBorderIn(QPainter *painter)
{
    int radius = 90;
    painter->save();
    painter->setPen(Qt::NoPen);
    QLinearGradient borderGradient(0, -radius, 0, radius);
    borderGradient.setColorAt(0, borderInColorStart);
    borderGradient.setColorAt(1, borderInColorEnd);
    painter->setBrush(borderGradient);
    painter->drawEllipse(-radius, -radius, radius * 2, radius * 2);
    painter->restore();
}

void LightButton::drawBg(QPainter *painter)
{
    int radius = 80;
    painter->save();
    painter->setPen(Qt::NoPen);
    painter->setBrush(bgColor);
    painter->drawEllipse(-radius, -radius, radius * 2, radius * 2);
    painter->restore();
}

void LightButton::drawText(QPainter *painter)
{
    if (text.isEmpty()) {
        return;
    }

    int radius = 100;
    painter->save();

    QFont font;
    font.setPixelSize(85);
    painter->setFont(font);
    painter->setPen(textColor);
    QRect rect(-radius, -radius, radius * 2, radius * 2);
    painter->drawText(rect, Qt::AlignCenter, text);
    painter->restore();
}

void LightButton::drawOverlay(QPainter *painter)
{
    if (!showOverlay) {
        return;
    }

    int radius = 80;
    painter->save();
    painter->setPen(Qt::NoPen);

    QPainterPath smallCircle;
    QPainterPath bigCircle;
    radius -= 1;
    smallCircle.addEllipse(-radius, -radius, radius * 2, radius * 2);
    radius *= 2;
    bigCircle.addEllipse(-radius, -radius + 140, radius * 2, radius * 2);

    //高光的形状为小圆扣掉大圆的部分
    QPainterPath highlight = smallCircle - bigCircle;

    QLinearGradient linearGradient(0, -radius / 2, 0, 0);
    overlayColor.setAlpha(100);
    linearGradient.setColorAt(0.0, overlayColor);
    overlayColor.setAlpha(30);
    linearGradient.setColorAt(1.0, overlayColor);
    painter->setBrush(linearGradient);
    painter->rotate(-20);
    painter->drawPath(highlight);

    painter->restore();
}

QString LightButton::getText() const
{
    return this->text;
}

QColor LightButton::getTextColor() const
{
    return this->textColor;
}

QColor LightButton::getAlarmColor() const
{
    return this->alarmColor;
}

QColor LightButton::getNormalColor() const
{
    return this->normalColor;
}

QColor LightButton::getBorderOutColorStart() const
{
    return this->borderOutColorStart;
}

QColor LightButton::getBorderOutColorEnd() const
{
    return this->borderOutColorEnd;
}

QColor LightButton::getBorderInColorStart() const
{
    return this->borderInColorStart;
}

QColor LightButton::getBorderInColorEnd() const
{
    return this->borderInColorEnd;
}

QColor LightButton::getBgColor() const
{
    return this->bgColor;
}

bool LightButton::getCanMove() const
{
    return this->canMove;
}

bool LightButton::getShowRect() const
{
    return this->showRect;
}

bool LightButton::getShowOverlay() const
{
    return this->showOverlay;
}

QColor LightButton::getOverlayColor() const
{
    return this->overlayColor;
}

QSize LightButton::sizeHint() const
{
    return QSize(100, 100);
}

QSize LightButton::minimumSizeHint() const
{
    return QSize(10, 10);
}

void LightButton::setText(const QString &text)
{
    if (this->text != text) {
        this->text = text;
        this->update();
    }
}

void LightButton::setTextColor(const QColor &textColor)
{
    if (this->textColor != textColor) {
        this->textColor = textColor;
        this->update();
    }
}

void LightButton::setAlarmColor(const QColor &alarmColor)
{
    if (this->alarmColor != alarmColor) {
        this->alarmColor = alarmColor;
        this->update();
    }
}

void LightButton::setNormalColor(const QColor &normalColor)
{
    if (this->normalColor != normalColor) {
        this->normalColor = normalColor;
        this->update();
    }
}

void LightButton::setBorderOutColorStart(const QColor &borderOutColorStart)
{
    if (this->borderOutColorStart != borderOutColorStart) {
        this->borderOutColorStart = borderOutColorStart;
        this->update();
    }
}

void LightButton::setBorderOutColorEnd(const QColor &borderOutColorEnd)
{
    if (this->borderOutColorEnd != borderOutColorEnd) {
        this->borderOutColorEnd = borderOutColorEnd;
        this->update();
    }
}

void LightButton::setBorderInColorStart(const QColor &borderInColorStart)
{
    if (this->borderInColorStart != borderInColorStart) {
        this->borderInColorStart = borderInColorStart;
        this->update();
    }
}

void LightButton::setBorderInColorEnd(const QColor &borderInColorEnd)
{
    if (this->borderInColorEnd != borderInColorEnd) {
        this->borderInColorEnd = borderInColorEnd;
        this->update();
    }
}

void LightButton::setBgColor(const QColor &bgColor)
{
    if (this->bgColor != bgColor) {
        this->bgColor = bgColor;
        this->update();
    }
}

void LightButton::setCanMove(bool canMove)
{
    if (this->canMove != canMove) {
        this->canMove = canMove;
        this->update();
    }
}

void LightButton::setShowRect(bool showRect)
{
    if (this->showRect != showRect) {
        this->showRect = showRect;
        this->update();
    }
}

void LightButton::setShowOverlay(bool showOverlay)
{
    if (this->showOverlay != showOverlay) {
        this->showOverlay = showOverlay;
        this->update();
    }
}

void LightButton::setOverlayColor(const QColor &overlayColor)
{
    if (this->overlayColor != overlayColor) {
        this->overlayColor = overlayColor;
        this->update();
    }
}

void LightButton::setGreen()
{
    textColor = QColor(255, 255, 255);
    setBgColor(QColor(0, 166, 0));
}

void LightButton::setRed()
{
    textColor = QColor(255, 255, 255);
    setBgColor(QColor(255, 0, 0));
}

void LightButton::setYellow()
{
    textColor = QColor(25, 50, 7);
    setBgColor(QColor(238, 238, 0));
}

void LightButton::setBlack()
{
    textColor = QColor(255, 255, 255);
    setBgColor(QColor(10, 10, 10));
}

void LightButton::setGray()
{
    textColor = QColor(255, 255, 255);
    setBgColor(QColor(129, 129, 129));
}

void LightButton::setBlue()
{
    textColor = QColor(255, 255, 255);
    setBgColor(QColor(0, 0, 166));
}

void LightButton::setLightBlue()
{
    textColor = QColor(255, 255, 255);
    setBgColor(QColor(100, 184, 255));
}

void LightButton::setLightRed()
{
    textColor = QColor(255, 255, 255);
    setBgColor(QColor(255, 107, 107));
}

void LightButton::setLightGreen()
{
    textColor = QColor(255, 255, 255);
    setBgColor(QColor(24, 189, 155));
}

void LightButton::startAlarm()
{
    if (!timerAlarm->isActive()) {
        timerAlarm->start();
    }
}

void LightButton::stopAlarm()
{
    if (timerAlarm->isActive()) {
        timerAlarm->stop();
    }
}

void LightButton::alarm()
{
    if (isAlarm) {
        textColor = QColor(255, 255, 255);
        bgColor = normalColor;
    } else {
        textColor = QColor(255, 255, 255);
        bgColor = alarmColor;
    }

    this->update();
    isAlarm = !isAlarm;
}
