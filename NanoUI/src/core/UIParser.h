#ifndef UI_PARSER_H
#define UI_PARSER_H

#include <ArduinoJson.h>
#include "../components/UIComponent.h"

namespace NanoUI {

/**
 * @brief Maximum number of components per screen
 */
#ifndef MAX_COMPONENTS_PER_SCREEN
#define MAX_COMPONENTS_PER_SCREEN 20
#endif

/**
 * @brief Maximum number of screens
 */
#ifndef MAX_SCREENS
#define MAX_SCREENS 5
#endif

/**
 * @brief Forward declaration
 */
class UIScreen;

/**
 * @brief JSON UI definition parser
 * 
 * Parses .ui.json files/streams and constructs UIScreen and UIComponent objects.
 * Uses ArduinoJson streaming parser to minimize memory usage on Arduino Uno/Nano.
 */
class UIParser {
public:
    UIParser();

    /**
     * @brief Parse a JSON string from PROGMEM/flash memory
     * @param jsonStr Null-terminated JSON string
     * @param screens Output array to store parsed screens
     * @param maxScreens Maximum number of screens the array can hold
     * @return Number of screens successfully parsed
     */
    int parseFromFlash(const char* jsonStr, UIScreen* screens, int maxScreens);

    /**
     * @brief Parse a JSON file from SD card (if available)
     * @param path File path on SD card
     * @param screens Output array to store parsed screens
     * @param maxScreens Maximum number of screens the array can hold
     * @return Number of screens successfully parsed, or -1 on file error
     */
    int parseFromSD(const char* path, UIScreen* screens, int maxScreens);

    /**
     * @brief Convert a hex color string to RGB565
     * @param hexStr Color in "#RRGGBB" format
     * @return RGB565 color value, default black on error
     */
    static uint16_t parseColor(const char* hexStr);

    /**
     * @brief Get the last error message
     * @return Error description string
     */
    const char* getLastError() const { return _lastError; }

private:
    char _lastError[48];

    bool parseScreen(JsonObjectConst screenObj, UIScreen& screen);
    UIComponent* parseComponent(JsonObjectConst compObj);
    void parseStyle(JsonObjectConst styleObj, UIComponent* comp);
};

} // namespace NanoUI

#endif // UI_PARSER_H
