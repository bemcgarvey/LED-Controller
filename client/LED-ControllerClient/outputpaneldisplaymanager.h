#ifndef OUTPUTPANELDISPLAYMANAGER_H
#define OUTPUTPANELDISPLAYMANAGER_H

#include "ledoutput.h"
#include "patterndisplay.h"

#include <QLabel>
#include <QObject>
#include <QSpinBox>
#include <QVector>

class OutputPanelDisplayManager : public QObject
{
    Q_OBJECT
public:
    OutputPanelDisplayManager(LEDOutput *model,  QSpinBox *nLED, QSpinBox *nPattern, QFrame *labels, QFrame *patterns);
    void setModel(LEDOutput *model);
private:
    LEDOutput *config;
    QSpinBox *nLEDSpinBox;
    QSpinBox *nPatternsSpinBox;
    QVector<PatternDisplay *> patternDisplays;
    QVector<QLabel *> patternLabels;
    LEDOutput *outputConfig;
    QFrame *patternsFrame;
private slots:
    void on_nPatternSpinBoxChange(int value);
    void on_nLEDsSpinBoxChange(int value);
signals:
    void sizeChanged(void);
};

#endif // OUTPUTPANELDISPLAYMANAGER_H
