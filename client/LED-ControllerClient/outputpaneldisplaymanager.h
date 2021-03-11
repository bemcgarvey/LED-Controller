#ifndef OUTPUTPANELDISPLAYMANAGER_H
#define OUTPUTPANELDISPLAYMANAGER_H

#include "ledoutputconfig.h"
#include "ledpatterndisplay.h"

#include <QLabel>
#include <QObject>
#include <QSpinBox>
#include <QVector>

class OutputPanelDisplayManager : public QObject
{
    Q_OBJECT
public:
    OutputPanelDisplayManager(LEDOutputConfig *model,  QSpinBox *nLED, QSpinBox *nPattern, QFrame *labels, QFrame *patterns);
    void setModel(LEDOutputConfig *model);
private:
    LEDOutputConfig *config;
    QSpinBox *nLEDSpinBox;
    QSpinBox *nPatternsSpinBox;
    QVector<LEDPatternDisplay *> patternDisplays;
    QVector<QLabel *> patternLabels;
    LEDOutputConfig *outputConfig;
    QFrame *patternsFrame;
private slots:
    void on_nPatternSpinBoxChange(int value);
    void on_nLEDsSpinBoxChange(int value);
};

#endif // OUTPUTPANELDISPLAYMANAGER_H
