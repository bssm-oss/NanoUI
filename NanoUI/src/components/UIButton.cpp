#include "UIButton.h"
#include <string.h>

namespace NanoUI {

void UIButton::draw(IDriver* driver) {
    if (!driver || !visible) return;

    uint16_t drawBg = isPressed ? (uint16_t)(((bgColor >> 1) & 0x7BEF) | 0x0000) : bgColor;

    // Draw rounded rectangle background
    driver->fillRect(x + radius, y, width - 2 * radius, height, drawBg);
    driver->fillRect(x, y + radius, width, height - 2 * radius, drawBg);
    driver->fillCircle(x + radius, y + radius, radius, drawBg);
    driver->fillCircle(x + width - radius - 1, y + radius, radius, drawBg);
    driver->fillCircle(x + radius, y + height - radius - 1, radius, drawBg);
    driver->fillCircle(x + width - radius - 1, y + height - radius - 1, radius, drawBg);

    // Draw text centered
    driver->setTextColor(textColor);
    driver->setTextSize(fontSize);

    int16_t textW = strlen(label) * 6 * fontSize;
    int16_t textH = 8 * fontSize;
    int16_t textX = x + (width - textW) / 2;
    int16_t textY = y + (height - textH) / 2;

    driver->setCursor(textX, textY);
    driver->print(label);
}

bool UIButton::handleTouch(int16_t touchX, int16_t touchY, bool pressed) {
    if (!enabled || !visible) return false;

    bool inside = contains(touchX, touchY);
    if (inside && pressed) {
        if (!isPressed) {
            isPressed = true;
            needsRedraw = true;
        }
        return true;
    }

    if (isPressed && !pressed) {
        isPressed = false;
        needsRedraw = true;
        return inside;
    }

    if (isPressed && !inside) {
        isPressed = false;
        needsRedraw = true;
    }

    return false;
}

} // namespace NanoUI
