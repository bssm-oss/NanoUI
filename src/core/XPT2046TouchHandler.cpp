#include "XPT2046TouchHandler.h"
#include <Arduino.h>

namespace NanoUI {

// XPT2046 command bytes (12-bit, differential mode)
static const uint8_t CMD_X  = 0xD0;
static const uint8_t CMD_Y  = 0x90;
static const uint8_t CMD_Z1 = 0xB0;
static const uint8_t CMD_Z2 = 0xC0;

static const SPISettings XPT_SPI(1000000, MSBFIRST, SPI_MODE0);

XPT2046TouchHandler::XPT2046TouchHandler(int8_t csPin, SPIClass& spi)
    : _cs(csPin), _spi(spi)
{
    pinMode(_cs, OUTPUT);
    digitalWrite(_cs, HIGH);

    // Default calibration for 320x240 landscape
    TouchCalibration cal;
    cal.xMin  = 300;
    cal.xMax  = 3800;
    cal.yMin  = 300;
    cal.yMax  = 3800;
    cal.swapXY = false;
    setCalibration(cal);
}

uint16_t XPT2046TouchHandler::xfer(uint8_t cmd) {
    _spi.transfer(cmd);
    uint16_t hi = _spi.transfer(0x00);
    uint16_t lo = _spi.transfer(0x00);
    return (hi << 8 | lo) >> 3; // 12-bit result
}

bool XPT2046TouchHandler::readRaw(int16_t& x, int16_t& y, int16_t& z) {
    _spi.beginTransaction(XPT_SPI);
    digitalWrite(_cs, LOW);

    // Read pressure to check if touched
    xfer(CMD_Z1);
    int16_t z1 = xfer(CMD_Z1);
    int16_t z2 = xfer(CMD_Z2);

    // Read position (average 3 samples for stability)
    int32_t rx = 0, ry = 0;
    for (int i = 0; i < 3; i++) {
        rx += xfer(CMD_X);
        ry += xfer(CMD_Y);
    }

    digitalWrite(_cs, HIGH);
    _spi.endTransaction();

    x = rx / 3;
    y = ry / 3;
    z = z1;

    // z1 > 100 means touched (pressure threshold)
    return (z1 > 100 && z2 < 4000);
}

} // namespace NanoUI
