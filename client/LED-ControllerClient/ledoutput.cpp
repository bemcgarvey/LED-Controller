#include "ledoutput.h"

LEDOutput::LEDOutput() :
    QObject()
{
    setNumLEDs(0);
    setNumPatterns(1);
}

LEDOutput::LEDOutput(int nleds, int nPatterns) :
    QObject()
{
    setNumLEDs(nleds);
    setNumPatterns(nPatterns);
}

int LEDOutput::getNumLEDs() const
{
    return numLEDs;
}

void LEDOutput::setNumLEDs(int value)
{
    numLEDs = value;
    for (auto&& i : patterns) {
        i.setNumLEDs(numLEDs);
    }
    emit sizeChanged(sizeInBytes());
}

int LEDOutput::sizeInBytes()
{
    int size = 2;
    for (auto&& i : patterns) {
        size += i.sizeInBytes();
    }
    return size;
}

int LEDOutput::toByteVector(QVector<uint8_t> &vec)
{
    vec.append(numLEDs);
    vec.append(numPatterns);
    for (auto&& i : patterns) {
        i.toByteVector(vec);
    }
    return sizeInBytes();
}

int LEDOutput::getNumPatterns() const
{
    return numPatterns;
}

void LEDOutput::setNumPatterns(int value)
{
    numPatterns = value;
    patterns.resize(numPatterns);
    for (auto&& i : patterns) {
        i.setNumLEDs(numLEDs);
    }
    emit sizeChanged(sizeInBytes());
}

LEDPattern &LEDOutput::operator[](int i)
{
    return patterns[i];
}

