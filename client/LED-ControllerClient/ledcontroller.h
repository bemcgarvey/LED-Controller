#ifndef DEVICE_H
#define DEVICE_H

#include "ledoutput.h"

class LEDController
{
public:
    LEDController();
    int toByteVector(QVector<uint8_t>& vec);
    bool fromByteVector(const QVector<uint8_t>& vec, const int startPos);
    enum RCActions {RC_DO_NOTHING = 0, RC_OFF, RC_PATTERN_A, RC_PATTERN_B, RC_PATTERN_C, RC_NEXT_PATTERN};
    int sizeInBytes(void);
    LEDOutput& operator[](int i);
    int getRCAction(int i);
    void setRCAction(int i, int action);
private:
    int numOutputs;
    QVector<LEDOutput> outputs;
    QVector<RCActions> actions;
    int maxMemory;
};

#endif // DEVICE_H
