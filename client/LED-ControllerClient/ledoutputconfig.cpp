#include "ledoutputconfig.h"

LEDOutputConfig::LEDOutputConfig() :
    QObject()
{
    setNumLEDs(0);
    setNumPatterns(1);
}

LEDOutputConfig::LEDOutputConfig(int nleds, int nPatterns) :
    QObject()
{
    setNumLEDs(nleds);
    setNumPatterns(nPatterns);
    //NOTE Test code
    if (numLEDs >= 4 && numPatterns > 0) {
        patterns[0][0].setColor(0xff0000);
        patterns[0][1].setColor(0x00ff00);
        patterns[0][2].setColor(0x0000ff);
        patterns[0][3].setColor(0xffffff);
    }
}

int LEDOutputConfig::getNumLEDs() const
{
    return numLEDs;
}

void LEDOutputConfig::setNumLEDs(int value)
{
    numLEDs = value;
    for (auto&& i : patterns) {
        i.setNumLEDs(numLEDs);
    }
    emit sizeChanged(sizeInBytes());
}

int LEDOutputConfig::sizeInBytes()
{
    int size = 2;
    for (auto&& i : patterns) {
        size += i.sizeInBytes();
    }
    return size;
}

int LEDOutputConfig::toByteVector(QVector<uint8_t> &vec)
{
    vec.append(numLEDs);
    vec.append(numPatterns);
    for (auto&& i : patterns) {
        i.toByteVector(vec);
    }
    return sizeInBytes();
}

int LEDOutputConfig::getNumPatterns() const
{
    return numPatterns;
}

void LEDOutputConfig::setNumPatterns(int value)
{
    numPatterns = value;
    patterns.resize(numPatterns);
    for (auto&& i : patterns) {
        i.setNumLEDs(numLEDs);
    }
    emit sizeChanged(sizeInBytes());
}

LEDPattern &LEDOutputConfig::operator[](int i)
{
    return patterns[i];
}

