#include "ledpattern.h"

LEDPattern::LEDPattern() :
    numLEDs(0), leds(0)
{

}

LEDPattern::LEDPattern(int numleds) :
    numLEDs(numleds), leds(numleds)
{

}

void LEDPattern::setNumLEDs(int nleds)
{
    numLEDs = nleds;
    leds.resize(numLEDs);
}

int LEDPattern::getNumLEDs()
{
    return numLEDs;
}

int LEDPattern::sizeInBytes()
{
    return 4 + (numLEDs * LEDrgb::sizeInBytes());
}

int LEDPattern::toByteVector(QVector<uint8_t> &vec)
{
    vec.append(numLEDs);
    vec.append(onTime & 0xff);
    vec.append((onTime >> 8) & 0xff);
    vec.append(nextPattern);
    for (auto&& i : leds) {
        i.toByteVector(vec);
    }
    return sizeInBytes();
}

LEDrgb &LEDPattern::operator[](int i)
{
    return leds[i];
}

int LEDPattern::getNextPattern() const
{
    return nextPattern;
}

void LEDPattern::setNextPattern(int value)
{
    nextPattern = value;
}

int LEDPattern::getOnTime() const
{
    return onTime;
}

void LEDPattern::setOnTime(int value)
{
    onTime = value;
}
