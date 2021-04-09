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
    int getNumLEDs(void) const;
    int sizeInBytes(void) const;
    int toByteVector(QVector<uint8_t>& vec) const;
    void toByteArray(QByteArray &array) const;
    void fromByteArray(const QByteArray &array);
    int fromByteVector(const QVector<uint8_t> &vec, int &pos);
    LEDrgb& operator[](int i);
    int getOnTime() const;
    void setOnTime(int value);
    int getNextPattern() const;
    void setNextPattern(int value);
    bool operator!=(const LEDPattern& rhs) const;
    enum {PATTERN_A = 0x00, PATTERN_B = 0x01, PATTERN_C = 0x02, ROTATE_OUT = 0x40
          , ROTATE_IN = 0x80, BOUNCE = 0xC0};
private:
    int numLEDs; //0 - 255
    int onTime;  //in 0.1s increments -1 = stay on
    int nextPattern;  //0 - 2 for A - C, 255 for rotate
    QVector<LEDrgb> leds;
};

#endif // LEDPATTERN_H
