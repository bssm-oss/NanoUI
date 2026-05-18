#include "UIButton.h"
#include <string.h>
#include <Arduino.h>

namespace NanoUI {

void UIButton::draw(IDriver* driver) {
    if (!driver || !visible) return;

    uint16_t drawFill = isPressed ? (uint16_t)((fillColor >> 1) & 0x7BEF) : fillColor;

    Serial.printf("[Btn %s] fillColor=0x%04X drawFill=0x%04X bgColor=0x%04X x=%d y=%d w=%d h=%d r=%d\n",
                  id, (unsigned)fillColor, (unsigned)drawFill, (unsigned)UIComponent::bgColor,
                  (int)x, (int)y, (int)width, (int)height, (int)radius);

    // Erase previous button area using screen background color
    driver->fillRect(x, y, width, height, UIComponent::bgColor);

    // Draw rounded rectangle using horizontal scanlines (fillRect only — no Adafruit_GFX fillCircle)
    int16_t r = radius;
    for (int16_t dy = 0; dy < height; dy++) {
        int16_t indent = 0;
        if (r > 0) {
            if (dy < r) {
                int16_t dist = r - dy;
                // indent = r - floor(sqrt(r^2 - dist^2))
                int16_t sq = (int16_t)(r * r - dist * dist);
                int16_t root = 0;
                while ((root + 1) * (root + 1) <= sq) root++;
                indent = r - root;
            } else if (dy >= height - r) {
                int16_t dist = dy - (height - r - 1);
                int16_t sq = (int16_t)(r * r - dist * dist);
                int16_t root = 0;
                while ((root + 1) * (root + 1) <= sq) root++;
                indent = r - root;
            }
        }
        driver->fillRect(x + indent, y + dy, width - 2 * indent, 1, drawFill);
    }

    // Draw text centered (transparent bg — button face already filled above)
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
