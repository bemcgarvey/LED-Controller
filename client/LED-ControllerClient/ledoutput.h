#ifndef LEDOUTPUT_H
#define LEDOUTPUT_H

#include <QVector>
#include "ledpattern.h"

class LEDOutput
{
public:
    LEDOutput();
    LEDOutput(int leds, int patterns);
    int getNumLEDs() const;
    void setNumLEDs(int value);
    int sizeInBytes(void);
    int toByteVector(QVector<uint8_t>& vec);
    int getNumPatterns() const;
    void setNumPatterns(int value);
    LEDPattern& operator[](int i);
private:
    QVector<LEDPattern> patterns;
    int numLEDs;
    int numPatterns;
};

#endif // LEDOUTPUT_H
