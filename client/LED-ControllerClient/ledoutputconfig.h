#ifndef LEDOUTPUTCONFIG_H
#define LEDOUTPUTCONFIG_H

#include <QVector>
#include <QObject>
#include "ledpattern.h"

class LEDOutputConfig : public QObject
{
    Q_OBJECT
public:
    LEDOutputConfig();
    LEDOutputConfig(int leds, int patterns);
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
signals:
    void sizeChanged(int newSize);
};

#endif // LEDOUTPUTCONFIG_H
