#include "ledcontroller.h"

LEDController::LEDController()
{
    numOutputs = 6;
    for (int i = 0; i < numOutputs; ++i) {
        outputs.append(LEDOutput(0, 1));
    }
    for (int i = 0; i < 6; ++i) {
        actions.append(RCActions(RC_DO_NOTHING));
    }
    maxMemory = 0;
}

int LEDController::toByteVector(QVector<uint8_t> &vec)
{
    vec.append(numOutputs);
    for (auto&& i : outputs) {
        i.toByteVector(vec);
    }
    for (int i = 0; i < 6; ++i) {
        vec.append(static_cast<uint8_t>(actions[i]));
    }
    return sizeInBytes();
}

bool LEDController::fromByteVector(const QVector<uint8_t> &vec, const int startPos)
{

    return true;
}

int LEDController::sizeInBytes()
{
    int size = 8;
    for (auto&& i : outputs) {
        size += i.sizeInBytes();
    }
    return size;
}

LEDOutput &LEDController::operator[](int i)
{
    return outputs[i];
}

int LEDController::getRCAction(int i)
{
    return static_cast<int>(actions[i]);
}

void LEDController::setRCAction(int i, int action)
{
    actions[i] = static_cast<RCActions>(action);
}

int LEDController::getMaxMemory() const
{
    return maxMemory;
}

int LEDController::getNumOutputs() const
{
    return numOutputs;
}
