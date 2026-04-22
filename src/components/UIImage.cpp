#include "UIImage.h"
#include <string.h>

namespace NanoUI {

ImageSymbol UIImage::_registry[MAX_IMAGE_SYMBOLS];
uint8_t UIImage::_symbolCount = 0;

bool UIImage::registerSymbol(const char* name, const uint16_t* data, int16_t w, int16_t h) {
    if (_symbolCount >= MAX_IMAGE_SYMBOLS || !name || !data) return false;
    strncpy(_registry[_symbolCount].name, name, MAX_ID_LEN - 1);
    _registry[_symbolCount].name[MAX_ID_LEN - 1] = '\0';
    _registry[_symbolCount].data = data;
    _registry[_symbolCount].width = w;
    _registry[_symbolCount].height = h;
    _symbolCount++;
    return true;
}

const uint16_t* UIImage::findSymbol(const char* name) {
    for (uint8_t i = 0; i < _symbolCount; i++) {
        if (strcmp(_registry[i].name, name) == 0) {
            return _registry[i].data;
        }
    }
    return nullptr;
}

void UIImage::resolveSymbol() {
    imageData = findSymbol(src);
}

void UIImage::draw(IDriver* driver) {
    if (!driver || !visible) return;

    if (imageData) {
        driver->drawRGBBitmap(x, y, width, height, imageData);
    } else {
        // Draw placeholder rectangle if image not found
        driver->drawRect(x, y, width, height, 0xF800);
        driver->setCursor(x + 2, y + 2);
        driver->setTextColor(0xF800);
        driver->setTextSize(1);
        driver->print("?");
    }
}

bool UIImage::handleTouch(int16_t touchX, int16_t touchY, bool pressed) {
    (void)touchX;
    (void)touchY;
    (void)pressed;
    return false;
}

} // namespace NanoUI
