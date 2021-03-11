#ifndef LEDPATTERNDISPLAY_H
#define LEDPATTERNDISPLAY_H

#include <QFrame>
#include "ledpattern.h"

class LEDPatternDisplay : public QFrame
{
    Q_OBJECT
public:
    LEDPatternDisplay(QWidget *parent = nullptr);
    void setPattern(LEDPattern *p);

    // QWidget interface
protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseDoubleClickEvent(QMouseEvent *event) override;
    void paintEvent(QPaintEvent *event) override;
private:
    LEDPattern *pattern;
};

#endif // LEDPATTERNDISPLAY_H
