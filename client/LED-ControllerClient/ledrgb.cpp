#include "ledrgb.h"

LEDrgb::LEDrgb() :
    red(0), green(0), blue(0)
{

}

LEDrgb::LEDrgb(int r, int g, int b) :
    red(r), green(g), blue(b)
{

}

int LEDrgb::getGreen() const
{
    return green;
}

void LEDrgb::setGreen(int value)
{
    green = value;
}

int LEDrgb::getRed() const
{
    return red;
}

void LEDrgb::setRed(int value)
{
    red = value;
}

int LEDrgb::getBlue() const
{
    return blue;
}

void LEDrgb::setBlue(int value)
{
    blue = value;
}

void LEDrgb::setColor(int c)
{
    blue = c & 0xff;
    c >>= 8;
    green = c & 0xff;
    c >>= 8;
    red = c & 0xff;
}

int LEDrgb::rgb()
{
    int color = red;
    color <<= 8;
    color |= green;
    color <<= 8;
    color |= blue;
    return color;
}

int LEDrgb::sizeInBytes()
{
    return 3;
}

int LEDrgb::toByteVector(QVector<uint8_t> &vec)
{
    vec.append(green);
    vec.append(red);
    vec.append(blue);
    return sizeInBytes();
}

int LEDrgb::fromByteVector(const QVector<uint8_t> &vec, int &pos)
{
    green = vec[pos++];
    red = vec[pos++];
    blue = vec[pos++];
    return 3;
}
