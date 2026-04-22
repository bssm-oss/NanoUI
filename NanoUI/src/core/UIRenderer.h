#ifndef UI_RENDERER_H
#define UI_RENDERER_H

#include "../drivers/IDriver.h"
#include "UIScreen.h"

namespace NanoUI {

/**
 * @brief Screen and component renderer
 * 
 * Handles drawing screens and their components to the display.
 * Manages dirty-region tracking for efficient redraws.
 */
class UIRenderer {
private:
    IDriver* _driver;

public:
    explicit UIRenderer(IDriver* driver) : _driver(driver) {}

    /**
     * @brief Render an entire screen
     * @param screen Pointer to the screen to render
     */
    void renderScreen(UIScreen* screen);

    /**
     * @brief Render a single component
     * @param comp Pointer to the component
     */
    void renderComponent(UIComponent* comp);

    /**
     * @brief Set the display driver
     * @param driver Pointer to the driver
     */
    void setDriver(IDriver* driver) { _driver = driver; }

    /**
     * @brief Get the current display driver
     * @return Pointer to the driver
     */
    IDriver* getDriver() const { return _driver; }
};

} // namespace NanoUI

#endif // UI_RENDERER_H
