#ifndef LEDRGB_H
#define LEDRGB_H

#include <QVector>

class LEDrgb
{
public:
    LEDrgb();
    LEDrgb(int r, int g, int b);
    int getGreen() const;
    void setGreen(int value);
    int getRed() const;
    void setRed(int value);
    int getBlue() const;
    void setBlue(int value);
    void setColor(int c);
    int rgb(void) const;
    static int sizeInBytes(void);
    int toByteVector(QVector<uint8_t> &vec) const;
    int fromByteVector(const QVector<uint8_t> &vec, int &pos);
private:
    int red;
    int green;
    int blue;
};

#endif // LEDRGB_H
