#include "ledpatterndisplay.h"

#include <QBrush>
#include <QMessageBox>
#include <QMouseEvent>
#include <QPainter>

LEDPatternDisplay::LEDPatternDisplay(QWidget *parent) :
    QFrame(parent), pattern(nullptr)
{

}

void LEDPatternDisplay::setPattern(LEDPattern *p)
{
    pattern = p;
}

void LEDPatternDisplay::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::RightButton) {
        QMessageBox::information(this, "Mouse", "Right");
    } else if (event->button() == Qt::LeftButton) {
        //QMessageBox::information(this, "Mouse", "Left");
    }
}

void LEDPatternDisplay::mouseDoubleClickEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
    QMessageBox::information(this, "Mouse", "Double");
}

void LEDPatternDisplay::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    painter.drawRect(0, 0, width() - 1, height() - 1);

    if (pattern == nullptr) {
        return;
    }
    for (int i = 0; i < pattern->getNumLEDs(); ++i) {
        QBrush brush((*pattern)[i].rgb());
        painter.setBrush(brush);
        painter.drawEllipse(25 * i, 0, 20, 20);
    }
}

