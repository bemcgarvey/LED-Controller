#include "ledpattern.h"

LEDPattern::LEDPattern() :
    numLEDs(0), onTime(-1), nextPattern(0), leds(0)
{

}

LEDPattern::LEDPattern(int numleds) :
    numLEDs(numleds), onTime(-1), nextPattern(0), leds(numleds)
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
    //BUG when onTime == -1 this is not right
    vec.append((onTime >> 8) & 0xff);
    vec.append(onTime & 0xff);
    vec.append(nextPattern);
    for (auto&& i : leds) {
        i.toByteVector(vec);
    }
    return sizeInBytes();
}

int LEDPattern::fromByteVector(const QVector<uint8_t> &vec, int &pos)
{
    numLEDs = vec[pos++];
    //BUG when onTime == -1 this is not right
    onTime = vec[pos++];
    onTime <<= 8;
    onTime += vec[pos++];
    nextPattern = vec[pos++];
    if (leds.size() != numLEDs) {
        leds.resize(numLEDs); //Should be unnecassary but just to be safe
    }
    for (int i = 0; i < numLEDs; ++i) {
        leds[i].fromByteVector(vec, pos);
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
