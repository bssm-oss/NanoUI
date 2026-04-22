#ifndef UI_COMPONENT_H
#define UI_COMPONENT_H

#include <stdint.h>

namespace NanoUI {

/**
 * @brief Maximum length for component ID strings
 */
#define MAX_ID_LEN 16

/**
 * @brief Maximum length for label/text strings
 */
#define MAX_TEXT_LEN 32

/**
 * @brief Event structure passed to callbacks
 */
struct UIEvent {
    char id[MAX_ID_LEN];
    int value;
    bool toggled;
};

/**
 * @brief Base class for all UI components
 * 
 * All visual elements (buttons, labels, sliders, etc.) inherit from this class.
 * Provides common properties like position, size, visibility, and enabled state.
 */
class UIComponent {
public:
    char id[MAX_ID_LEN];
    int16_t x;
    int16_t y;
    int16_t width;
    int16_t height;
    bool visible;
    bool enabled;
    bool needsRedraw;

    UIComponent() : x(0), y(0), width(0), height(0), visible(true), enabled(true), needsRedraw(true) {
        id[0] = '\0';
    }

    virtual ~UIComponent() {}

    /**
     * @brief Get the type of this component as a string
     * @return Component type name (e.g., "button", "label")
     */
    virtual const char* getType() const = 0;

    /**
     * @brief Render this component to the display
     * @param driver Pointer to the display driver interface
     */
    virtual void draw(class IDriver* driver) = 0;

    /**
     * @brief Handle touch events within this component's bounds
     * @param touchX X coordinate of touch
     * @param touchY Y coordinate of touch
     * @param pressed True if touch is currently pressed
     * @return True if the touch was handled by this component
     */
    virtual bool handleTouch(int16_t touchX, int16_t touchY, bool pressed) = 0;

    /**
     * @brief Check if a point is inside this component's bounding box
     * @param px X coordinate
     * @param py Y coordinate
     * @return True if the point is inside
     */
    bool contains(int16_t px, int16_t py) const {
        return visible && px >= x && px < x + width && py >= y && py < y + height;
    }

    /**
     * @brief Set the text/label of this component
     * @param text New text string
     */
    virtual void setText(const char* text) {}

    /**
     * @brief Set the numeric value of this component
     * @param value New value
     */
    virtual void setValue(int value) {}

    /**
     * @brief Get the current numeric value of this component
     * @return Current value
     */
    virtual int getValue() const { return 0; }
};

} // namespace NanoUI

#endif // UI_COMPONENT_H
