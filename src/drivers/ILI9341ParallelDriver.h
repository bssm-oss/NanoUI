#ifndef ILI9341_PARALLEL_DRIVER_H
#define ILI9341_PARALLEL_DRIVER_H

#include "IDriver.h"
#include <Arduino.h>
#include <Adafruit_GFX.h>

namespace NanoUI {

class ILI9341ParallelDriver;

// Inner GFX helper: provides Adafruit_GFX algorithms (circle, text, etc.)
// and routes drawPixel back to the parallel hardware bus.
class _ParGFX : public Adafruit_GFX {
public:
    _ParGFX(ILI9341ParallelDriver* drv, int16_t w, int16_t h);
    void drawPixel(int16_t x, int16_t y, uint16_t color) override;
private:
    ILI9341ParallelDriver* _drv;
};

class ILI9341ParallelDriver : public IDriver {
public:
    // cs, dc, wr, rst: control pins (rst=-1 to skip HW reset)
    // d0–d7: data bus GPIO numbers (DB0–DB7)
    ILI9341ParallelDriver(int8_t cs, int8_t dc, int8_t wr, int8_t rst,
                          int8_t d0, int8_t d1, int8_t d2, int8_t d3,
                          int8_t d4, int8_t d5, int8_t d6, int8_t d7);
    ~ILI9341ParallelDriver();

    void init()       override;
    void fillScreen(uint16_t color) override;
    void fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) override;
    void drawRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) override;
    void fillCircle(int16_t x, int16_t y, int16_t r, uint16_t color) override;
    void drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color) override;
    void drawPixel(int16_t x, int16_t y, uint16_t color) override;
    void drawRGBBitmap(int16_t x, int16_t y, int16_t w, int16_t h, const uint16_t* data) override;
    void setCursor(int16_t x, int16_t y) override;
    void setTextColor(uint16_t color) override;
    void setTextBgColor(uint16_t bg) override;
    void setTextSize(uint8_t size) override;
    void print(const char* str) override;
    void print(int n) override;
    int16_t width()  const override;
    int16_t height() const override;
    void setRotation(uint8_t r) override;

    // Called by _ParGFX::drawPixel — writes a single pixel at current addr window
    void writePixelRaw(int16_t x, int16_t y, uint16_t color);

private:
    int8_t  _cs, _dc, _wr, _rst;
    int8_t  _db[8];
    int16_t _w, _h;
    uint8_t _rotation;

    int16_t  _curX, _curY;
    uint16_t _textColor;
    uint16_t _textBgColor;
    uint8_t  _textSize;

    // Precomputed GPIO bitmasks for each data-bus pin and control pins
    uint32_t _dbMask[8];
    uint32_t _csMask, _dcMask, _wrMask;

    // 256-entry LUT: for each byte value, the set/clear GPIO mask
    struct LUTEntry { uint32_t set, clr; };
    LUTEntry _lut[256];

    _ParGFX* _gfx;

    void buildLUT();
    inline void write8(uint8_t v);
    void writeCmd(uint8_t cmd);
    void writeData(uint8_t data);
    void writeBurst16(uint16_t color, int32_t count);
    void setAddrWindow(int16_t x0, int16_t y0, int16_t x1, int16_t y1);
    void initSequence();
};

} // namespace NanoUI
#endif // ILI9341_PARALLEL_DRIVER_H
