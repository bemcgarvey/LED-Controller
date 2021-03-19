#include "ledoutput.h"

LEDOutput::LEDOutput()
{
    setNumLEDs(0);
    setNumPatterns(1);
}

LEDOutput::LEDOutput(int nleds, int nPatterns)
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
}

int LEDOutput::sizeInBytes() const
{
    int size = 2;
    for (auto&& i : patterns) {
        size += i.sizeInBytes();
    }
    return size;
}

int LEDOutput::toByteVector(QVector<uint8_t> &vec) const
{
    vec.append(numLEDs);
    vec.append(numPatterns);
    for (auto&& i : patterns) {
        i.toByteVector(vec);
    }
    return sizeInBytes();
}

int LEDOutput::fromByteVector(const QVector<uint8_t> &vec, int &pos)
{
    numLEDs = vec[pos++];
    numPatterns = vec[pos++];
    setNumPatterns(numPatterns);
    for (int i = 0; i < numPatterns; ++i) {
        patterns[i].fromByteVector(vec, pos);
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
}

LEDPattern &LEDOutput::operator[](int i)
{
    return patterns[i];
}

