#ifndef ILI9341_DRIVER_H
#define ILI9341_DRIVER_H

#include "IDriver.h"
#include <Adafruit_ILI9341.h>

namespace NanoUI {

/**
 * @brief ILI9341 display driver implementation
 * 
 * Wraps the Adafruit_ILI9341 library to conform to the IDriver interface.
 * Supports 320x240 resolution on SPI-connected ILI9341 displays.
 */
class ILI9341Driver : public IDriver {
private:
    Adafruit_ILI9341* _tft;
    bool _ownsTft;

public:
    /**
     * @brief Construct with explicit pin numbers
     * @param cs Chip select pin
     * @param dc Data/command pin
     * @param rst Reset pin (use -1 if connected to reset)
     */
    ILI9341Driver(int8_t cs, int8_t dc, int8_t rst = -1);

    /**
     * @brief Construct with an existing Adafruit_ILI9341 instance
     * @param tft Pointer to existing instance
     */
    explicit ILI9341Driver(Adafruit_ILI9341* tft);

    ~ILI9341Driver();

    void init() override;
    void fillScreen(uint16_t color) override;
    void fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) override;
    void drawRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) override;
    void fillCircle(int16_t x, int16_t y, int16_t r, uint16_t color) override;
    void drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color) override;
    void drawPixel(int16_t x, int16_t y, uint16_t color) override;
    void drawRGBBitmap(int16_t x, int16_t y, int16_t w, int16_t h, const uint16_t* data) override;
    void setCursor(int16_t x, int16_t y) override;
    void setTextColor(uint16_t color) override;
    void setTextSize(uint8_t size) override;
    void print(const char* str) override;
    void print(int n) override;
    int16_t width() const override;
    int16_t height() const override;
    void setRotation(uint8_t rotation) override;

    /**
     * @brief Get the underlying Adafruit_ILI9341 instance
     * @return Pointer to the TFT object
     */
    Adafruit_ILI9341* getTFT() const { return _tft; }
};

} // namespace NanoUI

#endif // ILI9341_DRIVER_H
