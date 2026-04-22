#ifndef UI_SLIDER_H
#define UI_SLIDER_H

#include "UIComponent.h"
#include "../drivers/IDriver.h"
#include <string.h>

namespace NanoUI {

/**
 * @brief Horizontal slider component
 * 
 * Allows selecting a value within a range by dragging a thumb.
 * Supports custom track and thumb colors.
 */
class UISlider : public UIComponent {
public:
    int minValue;
    int maxValue;
    int currentValue;
    uint16_t trackColor;
    uint16_t thumbColor;
    char actionName[MAX_ID_LEN];
    bool isDragging;

    UISlider() 
        : minValue(0), maxValue(100), currentValue(50),
          trackColor(0x7BEF), thumbColor(0xFFE0), isDragging(false) {
        actionName[0] = '\0';
    }

    const char* getType() const override { return "slider"; }

    void setValue(int value) override {
        if (value < minValue) value = minValue;
        if (value > maxValue) value = maxValue;
        currentValue = value;
        needsRedraw = true;
    }

    int getValue() const override {
        return currentValue;
    }

    void setAction(const char* action) {
        if (action) {
            strncpy(actionName, action, MAX_ID_LEN - 1);
            actionName[MAX_ID_LEN - 1] = '\0';
        }
    }

    void draw(IDriver* driver) override;
    bool handleTouch(int16_t touchX, int16_t touchY, bool pressed) override;

private:
    int16_t valueToX() const;
    int xToValue(int16_t touchX) const;
};

} // namespace NanoUI

#endif // UI_SLIDER_H
