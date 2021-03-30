#include "colorpicker.h"
#include <QColorDialog>
#include <QMouseEvent>
#include <QPainter>
#include <QSettings>
#include "patterndisplay.h"

QVector<QColor> ColorPicker::colors(36, Qt::black);
bool ColorPicker::colorsLoaded = false;
bool ColorPicker::colorsChanged = false;

ColorPicker::ColorPicker(QWidget *parent) :
    QFrame(parent), selectedColor(0)
{
    border = lineWidth();
    rWidth = (width() - 2 * border) / 6;
    rWidth -= space;
    rHeight = (height() - 2 * border) / 6;
    rHeight -= space;
}

void ColorPicker::loadColors(void)
{
    if (!colorsLoaded) {
        QSettings settings;
        int size = settings.beginReadArray("colors");
        if (size > 0) {
            for (int i = 0; i < colors.size(); ++i) {
                settings.setArrayIndex(i);
                colors[i].setRgb(settings.value("color", 0).toInt());
            }
            settings.endArray();
        } else {
            resetColors();
        }
        colorsLoaded = true;
    }
}

void ColorPicker::saveColors(void)
{
    if (colorsChanged) {
        QSettings settings;
        settings.beginWriteArray("colors");
        for (int i = 0; i < colors.size(); ++i) {
            settings.setArrayIndex(i);
            settings.setValue("color", colors[i].rgb());
        }
        settings.endArray();
    }
}

void ColorPicker::resetColors()
{
    colors[0] = Qt::white;
    colors[1] = Qt::red;
    colors[2] = Qt::green;
    colors[3] = Qt::blue;
    colors[4] = Qt::yellow;
    colors[5] = Qt::magenta;
    colors[6] = Qt::cyan;
    for (int i = 7; i < colors.size(); ++i) {
        colors[i] = Qt::black;
    }
    colorsChanged = true;
}

void ColorPicker::mouseDoubleClickEvent(QMouseEvent *event)
{
    Q_UNUSED(event)
    QColor newColor = QColorDialog::getColor(colors[selectedColor], this, "Select Color");
    if (newColor.isValid() && newColor != colors[selectedColor]) {
        colors[selectedColor] = newColor;
        colorsChanged = true;
        emit colorChanged(newColor);
    }
}

void ColorPicker::paintEvent(QPaintEvent *event)
{
    QFrame::paintEvent(event);

    QPainter painter(this);
    QBrush brush(Qt::black);
    QPen pen(Qt::black);
    pen.setWidth(2);
    for (int r = 0; r < 6; ++r) {
        for (int c = 0; c < 6; ++c) {
            brush.setColor(colors[r * 6 + c]);
            painter.setBrush(brush);
            painter.drawRoundedRect(border + space + c * (rWidth + space), border + space + r * (rHeight + space), rWidth, rHeight, 3, 3);
            if(r * 6 + c == selectedColor) {
                if (colors[r * 6 + c] != Qt::black) {
                    pen.setColor(Qt::black);
                } else {
                    pen.setColor(Qt::yellow);
                }
                painter.save();
                painter.setPen(pen);
                painter.drawRoundedRect(border + space + c * (rWidth + space) + 2, border + space + r * (rHeight + space) + 2, rWidth - 4, rHeight - 4, 3, 3);
                painter.restore();
            }
        }
    }
}

QVector<QColor> ColorPicker::getColors() const
{
    return colors;
}

void ColorPicker::setColors(const QVector<QColor> &value)
{
    colors = value;
}

QColor ColorPicker::getColor() const
{
    return colors[selectedColor];
}


void ColorPicker::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        int c = (event->x() - border) / (rWidth + space);
        int r = (event->y() - border) / (rHeight + space);
        selectedColor = r * 6 + c;
        emit colorChanged(colors[selectedColor]);
        update();
    }
}

void ColorPicker::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event)
    border = lineWidth();
    rWidth = (width() - 2 * border) / 6;
    rWidth -= space;
    rHeight = (height() - 2 * border) / 6;
    rHeight -= space;
}

bool ColorPicker::getColorsChanged()
{
    return colorsChanged;
}

void ColorPicker::onPatternSelectionChange(int value)
{
    PatternDisplay *pat = dynamic_cast<PatternDisplay *>(sender());
    if (pat) {
        QColor c = colors[selectedColor];
        pat->setColor(value, c.red(), c.green(), c.blue());
    }
}

void ColorPicker::onRightClickPattern(int selection1, int selection2, PatternDisplay::RightClickActions action)
{
    PatternDisplay *pat = dynamic_cast<PatternDisplay *>(sender());
    if (pat) {
        if (action == PatternDisplay::RightClickActions::FILL) {
            QColor c2 = colors[selectedColor];
            if (selection1 <= selection2) {
                if (selection1 < 0) {
                    selection1 = 0;
                }
                for (int i = selection1; i <= selection2; ++i) {
                    pat->setColor(i, c2.red(), c2.green(), c2.blue());
                }
            } else {
                for (int i = selection1; i >= selection2; --i) {
                    pat->setColor(i, c2.red(), c2.green(), c2.blue());
                }
            }
            pat->update();
        } else if (action == PatternDisplay::RightClickActions::GRADIENT) {
            QColor c2 = colors[selectedColor];
            QColor c1 = pat->getColor(selection1);
            if (selection1 < selection2) {
                double steps = selection2 - selection1;
                double redStep = (c2.red() - c1.red()) / steps;
                double greenStep = (c2.green() - c1.green()) / steps;
                double blueStep = (c2.blue() - c1.blue()) / steps;
                pat->setColor(selection2, c2.red(), c2.green(), c2.blue());
                double r = c1.red();
                double g = c1.green();
                double b = c1.blue();
                for (int i = selection1 + 1; i < selection2; ++i) {
                    r += redStep;
                    g += greenStep;
                    b += blueStep;
                    pat->setColor(i, r, g, b);
                }
            } else if (selection1 > selection2) {
                double steps = selection1 - selection2;
                double redStep = (c2.red() - c1.red()) / steps;
                double greenStep = (c2.green() - c1.green()) / steps;
                double blueStep = (c2.blue() - c1.blue()) / steps;
                pat->setColor(selection2, c2.red(), c2.green(), c2.blue());
                double r = c1.red();
                double g = c1.green();
                double b = c1.blue();
                for (int i = selection1 - 1; i >= selection2; --i) {
                    r += redStep;
                    g += greenStep;
                    b += blueStep;
                    pat->setColor(i, r, g, b);
                }
            }
            pat->update();
        }
    }
}
