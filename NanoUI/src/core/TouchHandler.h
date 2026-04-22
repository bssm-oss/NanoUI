#ifndef TOUCH_HANDLER_H
#define TOUCH_HANDLER_H

#include <stdint.h>

namespace NanoUI {

/**
 * @brief Touch calibration data
 */
struct TouchCalibration {
    int16_t xMin;
    int16_t xMax;
    int16_t yMin;
    int16_t yMax;
    bool swapXY;
};

/**
 * @brief Abstract touch handler interface
 * 
 * Handles touch input processing including calibration and coordinate mapping.
 * Implementations should wrap specific touch libraries (XPT2046, resistive, etc.).
 */
class TouchHandler {
private:
    TouchCalibration _cal;
    int16_t _displayWidth;
    int16_t _displayHeight;
    bool _wasPressed;
    int16_t _lastX;
    int16_t _lastY;

public:
    TouchHandler();

    /**
     * @brief Set display dimensions for coordinate mapping
     * @param w Display width
     * @param h Display height
     */
    void setDisplaySize(int16_t w, int16_t h);

    /**
     * @brief Set calibration parameters
     * @param cal Calibration data
     */
    void setCalibration(const TouchCalibration& cal);

    /**
     * @brief Update touch state - call this in loop()
     * @return True if touch state changed
     */
    bool update();

    /**
     * @brief Check if screen is currently touched
     * @return True if pressed
     */
    bool isPressed() const;

    /**
     * @brief Get current mapped X coordinate
     * @return X coordinate in display space
     */
    int16_t getX() const { return _lastX; }

    /**
     * @brief Get current mapped Y coordinate
     * @return Y coordinate in display space
     */
    int16_t getY() const { return _lastY; }

    /**
     * @brief Check if touch was just released (rising edge)
     * @return True if released this update
     */
    bool wasReleased() const;

    /**
     * @brief Raw touch reading - implement in subclass
     * @param x Output raw X
     * @param y Output raw Y
     * @param z Output pressure
     * @return True if touched
     */
    virtual bool readRaw(int16_t& x, int16_t& y, int16_t& z) = 0;

protected:
    /**
     * @brief Map raw touch coordinates to display coordinates
     * @param rawX Raw X
     * @param rawY Raw Y
     * @param outX Mapped X
     * @param outY Mapped Y
     */
    void mapCoordinates(int16_t rawX, int16_t rawY, int16_t& outX, int16_t& outY);
};

} // namespace NanoUI

#endif // TOUCH_HANDLER_H
