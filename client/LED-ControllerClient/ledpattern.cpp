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
    int16_t ot = onTime;
    vec.append(static_cast<uint8_t>(ot & 0xff));
    vec.append(static_cast<uint8_t>((ot >> 8) & 0xff));
    vec.append(nextPattern);
    for (auto&& i : leds) {
        i.toByteVector(vec);
    }
    return sizeInBytes();
}

int LEDPattern::fromByteVector(const QVector<uint8_t> &vec, int &pos)
{
    numLEDs = vec[pos++];
    int16_t ot;
    ot = vec[pos++];
    ot |= (static_cast<int16_t>(vec[pos++])) << 8;
    onTime = ot;
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

bool LEDPattern::operator!=(const LEDPattern &rhs)
{
    if (onTime != rhs.onTime || nextPattern != rhs.nextPattern || numLEDs != rhs.numLEDs) {
        return true;
    }
    for (int i = 0; i < leds.size(); ++i) {
        if (leds[i].rgb() != rhs.leds[i].rgb()) {
            return true;
        }
    }
    return false;
}

int LEDPattern::getOnTime() const
{
    return onTime;
}

void LEDPattern::setOnTime(int value)
{
    onTime = value;
}
