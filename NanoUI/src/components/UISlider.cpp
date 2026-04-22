#include "UISlider.h"
#include <Arduino.h>

namespace NanoUI {

int16_t UISlider::valueToX() const {
    if (maxValue == minValue) return x + 8;
    long range = (long)maxValue - minValue;
    long pos = ((long)(currentValue - minValue) * (width - 16)) / range;
    return x + 8 + (int16_t)pos;
}

int UISlider::xToValue(int16_t touchX) const {
    if (width <= 16) return minValue;
    long relX = (long)(touchX - x - 8);
    if (relX < 0) relX = 0;
    if (relX > width - 16) relX = width - 16;
    long range = (long)maxValue - minValue;
    return minValue + (int)((relX * range) / (width - 16));
}

void UISlider::draw(IDriver* driver) {
    if (!driver || !visible) return;

    int16_t trackY = y + height / 2 - 2;
    int16_t thumbX = valueToX();
    int16_t thumbY = y + height / 2;
    int16_t thumbRadius = height / 2 - 2;

    // Draw track
    driver->fillRect(x + 8, trackY, width - 16, 4, trackColor);

    // Draw thumb
    driver->fillCircle(thumbX, thumbY, thumbRadius, thumbColor);
}

bool UISlider::handleTouch(int16_t touchX, int16_t touchY, bool pressed) {
    if (!enabled || !visible) return false;

    if (pressed) {
        if (contains(touchX, touchY) || isDragging) {
            int newValue = xToValue(touchX);
            if (newValue != currentValue) {
                currentValue = newValue;
                needsRedraw = true;
            }
            isDragging = true;
            return true;
        }
    } else {
        if (isDragging) {
            isDragging = false;
            return true;
        }
    }

    return false;
}

} // namespace NanoUI
