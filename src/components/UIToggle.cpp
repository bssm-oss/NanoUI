#include "UIToggle.h"

namespace NanoUI {

void UIToggle::draw(IDriver* driver) {
    if (!driver || !visible) return;

    uint16_t trackColor = toggled ? activeColor : inactiveColor;
    int16_t thumbRadius = height / 2 - 2;
    int16_t thumbX = toggled ? (x + width - thumbRadius - 2) : (x + thumbRadius + 2);
    int16_t thumbY = y + height / 2;

    // Draw track background
    driver->fillRect(x + thumbRadius, y + 2, width - 2 * thumbRadius, height - 4, trackColor);
    driver->fillCircle(x + thumbRadius, y + height / 2, height / 2 - 2, trackColor);
    driver->fillCircle(x + width - thumbRadius, y + height / 2, height / 2 - 2, trackColor);

    // Draw thumb
    driver->fillCircle(thumbX, thumbY, thumbRadius - 1, 0xFFFF);
}

bool UIToggle::handleTouch(int16_t touchX, int16_t touchY, bool pressed) {
    if (!enabled || !visible) return false;

    bool inside = contains(touchX, touchY);

    if (pressed && inside) {
        if (!isPressed) {
            toggled = !toggled;
            needsRedraw = true;
            isPressed = true;
        }
        return true;
    }

    if (!pressed && isPressed) {
        isPressed = false;
        return inside;
    }

    return false;
}

} // namespace NanoUI
