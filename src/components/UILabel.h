#ifndef UI_LABEL_H
#define UI_LABEL_H

#include "UIComponent.h"
#include "../drivers/IDriver.h"
#include <string.h>

namespace NanoUI {

/**
 * @brief Static text label component
 * 
 * Displays read-only text with configurable color, size, and alignment.
 */
class UILabel : public UIComponent {
public:
    char text[MAX_TEXT_LEN];
    uint16_t textColor;
    uint8_t fontSize;
    char align; // 'l'=left, 'c'=center, 'r'=right

    UILabel() : textColor(0xFFFF), fontSize(2), align('l') {
        text[0] = '\0';
    }

    const char* getType() const override { return "label"; }

    void setText(const char* newText) override {
        if (newText) {
            strncpy(text, newText, MAX_TEXT_LEN - 1);
            text[MAX_TEXT_LEN - 1] = '\0';
            needsRedraw = true;
        }
    }

    void draw(IDriver* driver) override;
    bool handleTouch(int16_t touchX, int16_t touchY, bool pressed) override;
};

} // namespace NanoUI

#endif // UI_LABEL_H
