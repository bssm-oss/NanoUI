#include "UILabel.h"
#include <string.h>
#include <Arduino.h>

namespace NanoUI {

void UILabel::draw(IDriver* driver) {
    if (!driver || !visible) return;

    // Clear previous text to prevent overlap on redraw
    int16_t clearW = (width  > 0) ? width  : MAX_TEXT_LEN * 6 * fontSize;
    int16_t clearH = (height > 0) ? height : 8 * fontSize + 2;
    driver->fillRect(x, y, clearW, clearH, bgColor);

    Serial.printf("[Lbl %s] bgColor=0x%04X textColor=0x%04X x=%d y=%d text='%s'\n",
                  id, (unsigned)bgColor, (unsigned)textColor, (int)x, (int)y, text);

    // drawPixel path test: 4 bright-red pixels above the label
    // If these appear on screen, writePixelRaw works; if not, per-pixel path is broken
    for (int16_t i = 0; i < 4; i++) {
        driver->drawPixel(x + i, y - 2, 0xF800);
    }

    driver->setTextColor(textColor);
    driver->setTextSize(fontSize);

    int16_t textW = strlen(text) * 6 * fontSize;
    int16_t textX = x;

    if (align == 'c') {
        textX = x + (width - textW) / 2;
    } else if (align == 'r') {
        textX = x + (width - textW);
    }

    driver->setCursor(textX, y);
    driver->print(text);
    Serial.printf("[Lbl %s] print done\n", id);
}

bool UILabel::handleTouch(int16_t touchX, int16_t touchY, bool pressed) {
    // Labels are not interactive
    (void)touchX;
    (void)touchY;
    (void)pressed;
    return false;
}

} // namespace NanoUI
