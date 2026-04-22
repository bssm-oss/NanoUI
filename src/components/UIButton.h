#ifndef UI_BUTTON_H
#define UI_BUTTON_H

#include "UIComponent.h"
#include "../drivers/IDriver.h"
#include <string.h>

namespace NanoUI {

/**
 * @brief Interactive button component
 * 
 * Supports press actions, custom styling with rounded corners,
 * and navigation targets.
 */
class UIButton : public UIComponent {
public:
    enum class ActionType {
        Callback,
        Navigate
    };

    char label[MAX_TEXT_LEN];
    uint16_t bgColor;
    uint16_t textColor;
    uint8_t radius;
    uint8_t fontSize;
    ActionType actionType;
    char actionName[MAX_ID_LEN];
    char navigateTarget[MAX_ID_LEN];
    bool isPressed;

    UIButton() 
        : bgColor(0x07E0), textColor(0xFFFF), radius(4), fontSize(2),
          actionType(ActionType::Callback), isPressed(false) {
        label[0] = '\0';
        actionName[0] = '\0';
        navigateTarget[0] = '\0';
    }

    const char* getType() const override { return "button"; }

    void setText(const char* text) override {
        if (text) {
            strncpy(label, text, MAX_TEXT_LEN - 1);
            label[MAX_TEXT_LEN - 1] = '\0';
            needsRedraw = true;
        }
    }

    void setAction(const char* action) {
        if (action) {
            strncpy(actionName, action, MAX_ID_LEN - 1);
            actionName[MAX_ID_LEN - 1] = '\0';
            actionType = ActionType::Callback;
        }
    }

    void setTarget(const char* target) {
        if (target) {
            strncpy(navigateTarget, target, MAX_ID_LEN - 1);
            navigateTarget[MAX_ID_LEN - 1] = '\0';
            actionType = ActionType::Navigate;
        }
    }

    void draw(IDriver* driver) override;
    bool handleTouch(int16_t touchX, int16_t touchY, bool pressed) override;
};

} // namespace NanoUI

#endif // UI_BUTTON_H
