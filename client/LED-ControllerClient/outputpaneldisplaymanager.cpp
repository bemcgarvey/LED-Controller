#include "outputpaneldisplaymanager.h"

OutputPanelDisplayManager::OutputPanelDisplayManager(LEDOutput *model, QSpinBox *nLED, QSpinBox *nPattern, QFrame *labels, QFrame *patterns) :
    QObject()
{
    outputConfig = model;
    nLEDSpinBox = nLED;
    nPatternsSpinBox = nPattern;
    patternsFrame = patterns;
    for (auto&& i : labels->findChildren<QLabel *>()) {
        patternLabels.append(i);
    }
    for (auto&& i : patterns->findChildren<PatternDisplay *>()) {
        patternDisplays.append(i);
        i->setDisplayManager(this);
    }
    updateControls();
    connect(nPatternsSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), this, &OutputPanelDisplayManager::on_nPatternSpinBoxChange);
    connect(nLEDSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), this, &OutputPanelDisplayManager::on_nLEDsSpinBoxChange);
}

void OutputPanelDisplayManager::updateControls()
{
    nLEDSpinBox->setValue(outputConfig->getNumLEDs());
    nPatternsSpinBox->setValue(outputConfig->getNumPatterns());
    for (int i = 0; i < nPatternsSpinBox->value(); ++i) {
        patternLabels[i]->setVisible(true);
        patternDisplays[i]->setVisible(true);
        patternDisplays[i]->setPattern(&((*outputConfig)[i]));
    }
    for (int i = nPatternsSpinBox->value(); i < 3; ++i) {
        patternLabels[i]->setVisible(false);
        patternDisplays[i]->setVisible(false);
    }
}

void OutputPanelDisplayManager::setLEDsSpinBoxValue(int value)
{
    nLEDSpinBox->setValue(value);
}

void OutputPanelDisplayManager::on_nPatternSpinBoxChange(int value)
{
    outputConfig->setNumPatterns(value);
    for (int i = 0; i < value; ++i) {
        patternLabels[i]->setVisible(true);
        patternDisplays[i]->setVisible(true);
        patternDisplays[i]->setPattern(&((*outputConfig)[i]));
    }
    for (int i = value; i < 3; ++i) {
        patternLabels[i]->setVisible(false);
        patternDisplays[i]->setVisible(false);
    }
    emit sizeChanged();
}

void OutputPanelDisplayManager::on_nLEDsSpinBoxChange(int value)
{
    outputConfig->setNumLEDs(value);
    patternsFrame->update();
    emit sizeChanged();
}
