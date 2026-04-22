#ifndef UI_IMAGE_H
#define UI_IMAGE_H

#include "UIComponent.h"
#include "../drivers/IDriver.h"
#include <string.h>

namespace NanoUI {

/**
 * @brief Maximum number of registered image symbols
 */
#ifndef MAX_IMAGE_SYMBOLS
#define MAX_IMAGE_SYMBOLS 10
#endif

/**
 * @brief Image symbol registry entry
 */
struct ImageSymbol {
    char name[MAX_ID_LEN];
    const uint16_t* data;
    int16_t width;
    int16_t height;
};

/**
 * @brief Image component that displays bitmaps from PROGMEM
 * 
 * Images must be pre-registered as symbols before parsing.
 * Use registerSymbol() to map a name to PROGMEM RGB565 data.
 */
class UIImage : public UIComponent {
public:
    char src[MAX_ID_LEN];
    const uint16_t* imageData;

    UIImage() : imageData(nullptr) {
        src[0] = '\0';
    }

    const char* getType() const override { return "image"; }

    void setSource(const char* source) {
        if (source) {
            strncpy(src, source, MAX_ID_LEN - 1);
            src[MAX_ID_LEN - 1] = '\0';
            resolveSymbol();
            needsRedraw = true;
        }
    }

    void draw(IDriver* driver) override;
    bool handleTouch(int16_t touchX, int16_t touchY, bool pressed) override;

    /**
     * @brief Register an image symbol globally
     * @param name Symbol name used in JSON
     * @param data Pointer to PROGMEM RGB565 data
     * @param w Image width
     * @param h Image height
     * @return True if registered successfully
     */
    static bool registerSymbol(const char* name, const uint16_t* data, int16_t w, int16_t h);

    /**
     * @brief Find registered symbol data by name
     * @param name Symbol name
     * @return Pointer to image data, or nullptr if not found
     */
    static const uint16_t* findSymbol(const char* name);

private:
    void resolveSymbol();
    static ImageSymbol _registry[MAX_IMAGE_SYMBOLS];
    static uint8_t _symbolCount;
};

} // namespace NanoUI

#endif // UI_IMAGE_H
