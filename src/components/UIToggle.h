#ifndef UI_TOGGLE_H
#define UI_TOGGLE_H

#include "UIComponent.h"
#include "../drivers/IDriver.h"
#include <string.h>

namespace NanoUI {

/**
 * @brief Toggle switch component
 * 
 * A binary on/off switch that can be tapped to change state.
 * Supports custom colors for active and inactive states.
 */
class UIToggle : public UIComponent {
public:
    bool toggled;
    uint16_t activeColor;
    uint16_t inactiveColor;
    char actionName[MAX_ID_LEN];
    bool isPressed;

    UIToggle() 
        : toggled(false), activeColor(0x07E0), inactiveColor(0x7BEF), isPressed(false) {
        actionName[0] = '\0';
    }

    const char* getType() const override { return "toggle"; }

    void setValue(int value) override {
        toggled = (value != 0);
        needsRedraw = true;
    }

    int getValue() const override {
        return toggled ? 1 : 0;
    }

    void setAction(const char* action) {
        if (action) {
            strncpy(actionName, action, MAX_ID_LEN - 1);
            actionName[MAX_ID_LEN - 1] = '\0';
        }
    }

    void draw(IDriver* driver) override;
    bool handleTouch(int16_t touchX, int16_t touchY, bool pressed) override;
};

} // namespace NanoUI

#endif // UI_TOGGLE_H
