#ifndef XPT2046_TOUCH_HANDLER_H
#define XPT2046_TOUCH_HANDLER_H

#include "TouchHandler.h"
#include <SPI.h>

namespace NanoUI {

/**
 * XPT2046 SPI resistive touch controller handler.
 * SPI bus must be initialized (SPI.begin) before calling setTouchHandler or beginParallel.
 */
class XPT2046TouchHandler : public TouchHandler {
public:
    // csPin: T_CS GPIO; spi: SPI bus instance (default = SPI)
    XPT2046TouchHandler(int8_t csPin, SPIClass& spi = SPI);

    bool readRaw(int16_t& x, int16_t& y, int16_t& z) override;

private:
    int8_t    _cs;
    SPIClass& _spi;

    uint16_t xfer(uint8_t cmd);
};

} // namespace NanoUI
#endif
