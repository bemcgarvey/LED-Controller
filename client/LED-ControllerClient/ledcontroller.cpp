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

int LEDController::toByteVector(QVector<uint8_t> &vec) const
{
    vec.append(numOutputs);
    for (int i = 0; i < 6; ++i) {
        vec.append(static_cast<uint8_t>(actions[i]));
    }
    for (auto&& i : outputs) {
        i.toByteVector(vec);
    }
    return sizeInBytes();
}

bool LEDController::fromByteVector(const QVector<uint8_t> &vec)
{
    numOutputs = vec[0];
    int pos = 1;
    for (int i = 0; i < 6; ++i) {
        actions[i] = static_cast<RCActions>(vec[pos++]);
    }
    for (int i = 0; i < numOutputs; ++i) {
        outputs[i].fromByteVector(vec, pos);
    }

    return true;
}

int LEDController::sizeInBytes() const
{
    int size = 7;
    for (auto&& i : outputs) {
        size += i.sizeInBytes();
    }
    return size;
}

LEDOutput &LEDController::operator[](int i)
{
    return outputs[i];
}

int LEDController::getRCAction(int i) const
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

void LEDController::setMaxMemory(int value)
{
    maxMemory = value;
}

int LEDController::getNumOutputs() const
{
    return numOutputs;
}
