#include "UILabel.h"
#include <string.h>

namespace NanoUI {

void UILabel::draw(IDriver* driver) {
    if (!driver || !visible) return;

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
}

bool UILabel::handleTouch(int16_t touchX, int16_t touchY, bool pressed) {
    // Labels are not interactive
    (void)touchX;
    (void)touchY;
    (void)pressed;
    return false;
}

} // namespace NanoUI
