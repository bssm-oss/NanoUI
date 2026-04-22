#include "ILI9341Driver.h"

namespace NanoUI {

ILI9341Driver::ILI9341Driver(int8_t cs, int8_t dc, int8_t rst) : _ownsTft(true) {
    _tft = new Adafruit_ILI9341(cs, dc, rst);
}

ILI9341Driver::ILI9341Driver(Adafruit_ILI9341* tft) : _tft(tft), _ownsTft(false) {}

ILI9341Driver::~ILI9341Driver() {
    if (_ownsTft && _tft) {
        delete _tft;
    }
}

void ILI9341Driver::init() {
    _tft->begin();
}

void ILI9341Driver::fillScreen(uint16_t color) {
    _tft->fillScreen(color);
}

void ILI9341Driver::fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) {
    _tft->fillRect(x, y, w, h, color);
}

void ILI9341Driver::drawRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) {
    _tft->drawRect(x, y, w, h, color);
}

void ILI9341Driver::fillCircle(int16_t x, int16_t y, int16_t r, uint16_t color) {
    _tft->fillCircle(x, y, r, color);
}

void ILI9341Driver::drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color) {
    _tft->drawLine(x0, y0, x1, y1, color);
}

void ILI9341Driver::drawPixel(int16_t x, int16_t y, uint16_t color) {
    _tft->drawPixel(x, y, color);
}

void ILI9341Driver::drawRGBBitmap(int16_t x, int16_t y, int16_t w, int16_t h, const uint16_t* data) {
    _tft->drawRGBBitmap(x, y, data, w, h);
}

void ILI9341Driver::setCursor(int16_t x, int16_t y) {
    _tft->setCursor(x, y);
}

void ILI9341Driver::setTextColor(uint16_t color) {
    _tft->setTextColor(color);
}

void ILI9341Driver::setTextSize(uint8_t size) {
    _tft->setTextSize(size);
}

void ILI9341Driver::print(const char* str) {
    _tft->print(str);
}

void ILI9341Driver::print(int n) {
    _tft->print(n);
}

int16_t ILI9341Driver::width() const {
    return _tft->width();
}

int16_t ILI9341Driver::height() const {
    return _tft->height();
}

void ILI9341Driver::setRotation(uint8_t rotation) {
    _tft->setRotation(rotation);
}

} // namespace NanoUI
