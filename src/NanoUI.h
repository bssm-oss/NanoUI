#ifndef NANO_UI_H
#define NANO_UI_H

#include "drivers/IDriver.h"
#include "drivers/ILI9341Driver.h"
#include "core/UIParser.h"
#include "core/UIRenderer.h"
#include "core/UIScreen.h"
#include "core/TouchHandler.h"
#include "components/UIComponent.h"
#include "components/UIButton.h"
#include "components/UILabel.h"
#include "components/UIToggle.h"
#include "components/UISlider.h"
#include "components/UIImage.h"

namespace NanoUI {

/**
 * @brief Maximum depth of screen history stack
 */
#ifndef MAX_HISTORY_DEPTH
#define MAX_HISTORY_DEPTH 5
#endif

/**
 * @brief Maximum number of registered event callbacks
 */
#ifndef MAX_CALLBACKS
#define MAX_CALLBACKS 10
#endif

/**
 * @brief Callback entry for event handling
 */
struct CallbackEntry {
    char eventName[MAX_ID_LEN];
    void (*callback)(UIEvent);
};

/**
 * @brief Main NanoUI class - public API entry point
 * 
 * This is the primary interface for creating and managing JSON-based
 * touch UIs on Arduino with ILI9341 displays.
 */
class NanoUI {
public:
    NanoUI();
    ~NanoUI();

    /**
     * @brief Initialize the display and touch hardware
     * @param tft_cs TFT chip select pin
     * @param tft_dc TFT data/command pin
     * @param tft_rst TFT reset pin (-1 if not used)
     * @param touch_cs Touch controller chip select pin
     */
    void begin(int tft_cs, int tft_dc, int tft_rst, int touch_cs);

    /**
     * @brief Load UI definition from SD card file
     * @param path Absolute path to .ui.json file
     * @return True if loaded successfully
     */
    bool loadFromSD(const char* path);

    /**
     * @brief Load UI definition from PROGMEM/flash string
     * @param jsonStr Null-terminated JSON string in flash memory
     * @return True if loaded successfully
     */
    bool loadFromFlash(const char* jsonStr);

    /**
     * @brief Show a screen by its ID
     * @param screenId Screen identifier from JSON
     */
    void show(const char* screenId);

    /**
     * @brief Go back to the previous screen using history stack
     */
    void back();

    /**
     * @brief Register an event callback
     * @param eventName Event name (e.g., "startMotor", "setLED")
     * @param callback Function pointer to call when event fires
     */
    void on(const char* eventName, void(*callback)(UIEvent));

    /**
     * @brief Update touch and redraw - call in loop()
     */
    void update();

    /**
     * @brief Set text of a label component
     * @param id Component ID
     * @param text New text string
     */
    void setText(const char* id, const char* text);

    /**
     * @brief Set numeric value of a component
     * @param id Component ID
     * @param value New value
     */
    void setValue(const char* id, int value);

    /**
     * @brief Set visibility of a component
     * @param id Component ID
     * @param visible True to show, false to hide
     */
    void setVisible(const char* id, bool visible);

    /**
     * @brief Set enabled state of a component
     * @param id Component ID
     * @param enabled True to enable interaction
     */
    void setEnabled(const char* id, bool enabled);

    /**
     * @brief Find a component by ID on the current screen
     * @param id Component ID
     * @return Pointer to component, or nullptr if not found
     */
    UIComponent* find(const char* id);

    /**
     * @brief Get the currently active screen
     * @return Pointer to current screen, or nullptr
     */
    UIScreen* getCurrentScreen() const { return _currentScreen; }

    /**
     * @brief Set a custom touch handler
     * @param handler Pointer to touch handler instance
     */
    void setTouchHandler(TouchHandler* handler) { _touchHandler = handler; }

    /**
     * @brief Set a custom display driver
     * @param driver Pointer to driver instance
     */
    void setDriver(IDriver* driver);

private:
    IDriver* _driver;
    ILI9341Driver* _iliDriver;
    UIRenderer* _renderer;
    UIParser* _parser;
    TouchHandler* _touchHandler;
    bool _ownsTouchHandler;

    UIScreen _screens[MAX_SCREENS];
    int _screenCount;
    UIScreen* _currentScreen;

    UIScreen* _history[MAX_HISTORY_DEPTH];
    int _historyIndex;

    CallbackEntry _callbacks[MAX_CALLBACKS];
    int _callbackCount;

    bool _touchWasPressed;
    int16_t _lastTouchX;
    int16_t _lastTouchY;

    void pushHistory(UIScreen* screen);
    UIScreen* popHistory();
    UIScreen* findScreen(const char* id);
    void fireEvent(const char* id, int value, bool toggled);
    void fireEvent(UIComponent* comp);
    void renderCurrentScreen();
};

} // namespace NanoUI

#endif // NANO_UI_H
