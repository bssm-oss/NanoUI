#include "TouchHandler.h"
#include <Arduino.h>

namespace NanoUI {

TouchHandler::TouchHandler() 
    : _cal{300, 3800, 300, 3800, false}, 
      _displayWidth(320), _displayHeight(240),
      _wasPressed(false), _lastX(0), _lastY(0) {
}

void TouchHandler::setDisplaySize(int16_t w, int16_t h) {
    _displayWidth = w;
    _displayHeight = h;
}

void TouchHandler::setCalibration(const TouchCalibration& cal) {
    _cal = cal;
}

bool TouchHandler::update() {
    int16_t rawX, rawY, rawZ;
    bool pressed = readRaw(rawX, rawY, rawZ);

    if (pressed) {
        mapCoordinates(rawX, rawY, _lastX, _lastY);
    }

    bool changed = (pressed != _wasPressed);
    _wasPressed = pressed;
    return changed;
}

bool TouchHandler::isPressed() const {
    return _wasPressed;
}

bool TouchHandler::wasReleased() const {
    // This is checked externally by comparing previous state
    return false;
}

void TouchHandler::mapCoordinates(int16_t rawX, int16_t rawY, int16_t& outX, int16_t& outY) {
    long mappedX, mappedY;

    if (_cal.swapXY) {
        mappedX = map((long)rawY, _cal.yMin, _cal.yMax, 0, _displayWidth - 1);
        mappedY = map((long)rawX, _cal.xMin, _cal.xMax, 0, _displayHeight - 1);
    } else {
        mappedX = map((long)rawX, _cal.xMin, _cal.xMax, 0, _displayWidth - 1);
        mappedY = map((long)rawY, _cal.yMin, _cal.yMax, 0, _displayHeight - 1);
    }

    // Clamp values
    if (mappedX < 0) mappedX = 0;
    if (mappedX >= _displayWidth) mappedX = _displayWidth - 1;
    if (mappedY < 0) mappedY = 0;
    if (mappedY >= _displayHeight) mappedY = _displayHeight - 1;

    outX = (int16_t)mappedX;
    outY = (int16_t)mappedY;
}

} // namespace NanoUI
