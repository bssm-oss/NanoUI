#ifndef IDRIVER_H
#define IDRIVER_H

#include <stdint.h>

namespace NanoUI {

/**
 * @brief Abstract display driver interface
 * 
 * Defines the contract that all display drivers must implement.
 * This allows NanoUI to work with different display controllers
 * (ILI9341, ST7789, etc.) without code changes.
 */
class IDriver {
public:
    virtual ~IDriver() {}

    /**
     * @brief Initialize the display hardware
     */
    virtual void init() = 0;

    /**
     * @brief Fill the entire screen with a color
     * @param color RGB565 color value
     */
    virtual void fillScreen(uint16_t color) = 0;

    /**
     * @brief Draw a filled rectangle
     * @param x Top-left X coordinate
     * @param y Top-left Y coordinate
     * @param w Width in pixels
     * @param h Height in pixels
     * @param color RGB565 color value
     */
    virtual void fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) = 0;

    /**
     * @brief Draw a rectangle outline
     * @param x Top-left X coordinate
     * @param y Top-left Y coordinate
     * @param w Width in pixels
     * @param h Height in pixels
     * @param color RGB565 color value
     */
    virtual void drawRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) = 0;

    /**
     * @brief Draw a filled circle
     * @param x Center X coordinate
     * @param y Center Y coordinate
     * @param r Radius in pixels
     * @param color RGB565 color value
     */
    virtual void fillCircle(int16_t x, int16_t y, int16_t r, uint16_t color) = 0;

    /**
     * @brief Draw a line
     * @param x0 Start X coordinate
     * @param y0 Start Y coordinate
     * @param x1 End X coordinate
     * @param y1 End Y coordinate
     * @param color RGB565 color value
     */
    virtual void drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color) = 0;

    /**
     * @brief Draw a single pixel
     * @param x X coordinate
     * @param y Y coordinate
     * @param color RGB565 color value
     */
    virtual void drawPixel(int16_t x, int16_t y, uint16_t color) = 0;

    /**
     * @brief Draw a bitmap image
     * @param x Top-left X coordinate
     * @param y Top-left Y coordinate
     * @param w Width in pixels
     * @param h Height in pixels
     * @param data Pointer to RGB565 image data in PROGMEM
     */
    virtual void drawRGBBitmap(int16_t x, int16_t y, int16_t w, int16_t h, const uint16_t* data) = 0;

    /**
     * @brief Set the text cursor position
     * @param x X coordinate
     * @param y Y coordinate
     */
    virtual void setCursor(int16_t x, int16_t y) = 0;

    /**
     * @brief Set the text color
     * @param color RGB565 color value
     */
    virtual void setTextColor(uint16_t color) = 0;

    /**
     * @brief Set the text size multiplier
     * @param size Size multiplier (1 = default)
     */
    virtual void setTextSize(uint8_t size) = 0;

    /**
     * @brief Print a string at the current cursor position
     * @param str Null-terminated string
     */
    virtual void print(const char* str) = 0;

    /**
     * @brief Print an integer at the current cursor position
     * @param n Integer value
     */
    virtual void print(int n) = 0;

    /**
     * @brief Get display width in pixels
     * @return Width
     */
    virtual int16_t width() const = 0;

    /**
     * @brief Get display height in pixels
     * @return Height
     */
    virtual int16_t height() const = 0;

    /**
     * @brief Set display rotation
     * @param rotation Rotation value (0-3)
     */
    virtual void setRotation(uint8_t rotation) = 0;
};

} // namespace NanoUI

#endif // IDRIVER_H
