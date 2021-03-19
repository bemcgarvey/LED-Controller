#ifndef LEDPATTERN_H
#define LEDPATTERN_H

#include <QVector>
#include "ledrgb.h"

class LEDPattern
{
public:
    LEDPattern();
    LEDPattern(int numleds);
    void setNumLEDs(int nleds);
    int getNumLEDs(void);
    int sizeInBytes(void);
    int toByteVector(QVector<uint8_t>& vec);
    int fromByteVector(const QVector<uint8_t> &vec, int &pos);
    LEDrgb& operator[](int i);
    int getOnTime() const;
    void setOnTime(int value);
    int getNextPattern() const;
    void setNextPattern(int value);
    bool operator!=(const LEDPattern& rhs);
private:
    int numLEDs; //0 - 255
    int onTime;  //in 0.1s increments -1 = stay on
    int nextPattern;  //0 - 2 for A - C
    QVector<LEDrgb> leds;
};

#endif // LEDPATTERN_H
