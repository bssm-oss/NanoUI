#include "ILI9341ParallelDriver.h"

#if defined(ESP32)
#include "soc/gpio_reg.h"   // GPIO_OUT_W1TS_REG, GPIO_OUT_W1TC_REG
#endif

namespace NanoUI {

// ── _ParGFX ──────────────────────────────────────────────────────────────────

_ParGFX::_ParGFX(ILI9341ParallelDriver* drv, int16_t w, int16_t h)
    : Adafruit_GFX(w, h), _drv(drv) {}

void _ParGFX::drawPixel(int16_t x, int16_t y, uint16_t color) {
    _drv->writePixelRaw(x, y, color);
}

// ── ILI9341ParallelDriver ─────────────────────────────────────────────────────

ILI9341ParallelDriver::ILI9341ParallelDriver(
    int8_t cs, int8_t dc, int8_t wr, int8_t rst,
    int8_t d0, int8_t d1, int8_t d2, int8_t d3,
    int8_t d4, int8_t d5, int8_t d6, int8_t d7)
    : _cs(cs), _dc(dc), _wr(wr), _rst(rst),
      _w(320), _h(240), _rotation(1),
      _curX(0), _curY(0), _textColor(0xFFFF), _textBgColor(0xFFFF), _textSize(1),
      _gfx(nullptr)
{
    _db[0] = d0; _db[1] = d1; _db[2] = d2; _db[3] = d3;
    _db[4] = d4; _db[5] = d5; _db[6] = d6; _db[7] = d7;
}

ILI9341ParallelDriver::~ILI9341ParallelDriver() {
    if (_gfx) delete _gfx;
}

// ── GPIO helpers ──────────────────────────────────────────────────────────────

void ILI9341ParallelDriver::buildLUT() {
    for (int i = 0; i < 8; i++) {
        _dbMask[i] = (1UL << _db[i]);
    }
    _csMask = (1UL << _cs);
    _dcMask = (1UL << _dc);
    _wrMask = (1UL << _wr);

    for (int v = 0; v < 256; v++) {
        uint32_t s = 0, c = 0;
        for (int bit = 0; bit < 8; bit++) {
            if (v & (1 << bit)) s |= _dbMask[bit];
            else                 c |= _dbMask[bit];
        }
        _lut[v].set = s;
        _lut[v].clr = c;
    }
}

inline void ILI9341ParallelDriver::write8(uint8_t v) {
#if defined(ESP32)
    // REG_WRITE works across all IDF versions; GPIO.out_w1ts is a union in IDF5
    REG_WRITE(GPIO_OUT_W1TS_REG, _lut[v].set);
    REG_WRITE(GPIO_OUT_W1TC_REG, _lut[v].clr);
    REG_WRITE(GPIO_OUT_W1TC_REG, _wrMask); // WR LOW
    REG_WRITE(GPIO_OUT_W1TS_REG, _wrMask); // WR HIGH (latch on rising edge)
#else
    for (int i = 0; i < 8; i++) {
        digitalWrite(_db[i], (v >> i) & 1);
    }
    digitalWrite(_wr, LOW);
    digitalWrite(_wr, HIGH);
#endif
}

void ILI9341ParallelDriver::writeCmd(uint8_t cmd) {
    digitalWrite(_dc, LOW);
    write8(cmd);
}

void ILI9341ParallelDriver::writeData(uint8_t data) {
    digitalWrite(_dc, HIGH);
    write8(data);
}

void ILI9341ParallelDriver::writeBurst16(uint16_t color, int32_t count) {
    uint8_t hi = color >> 8;
    uint8_t lo = color & 0xFF;

#if defined(ESP32)
    uint32_t setHi = _lut[hi].set, clrHi = _lut[hi].clr;
    uint32_t setLo = _lut[lo].set, clrLo = _lut[lo].clr;
    uint32_t wr = _wrMask;

    REG_WRITE(GPIO_OUT_W1TS_REG, _dcMask); // DC HIGH = data

    while (count--) {
        REG_WRITE(GPIO_OUT_W1TS_REG, setHi);
        REG_WRITE(GPIO_OUT_W1TC_REG, clrHi);
        REG_WRITE(GPIO_OUT_W1TC_REG, wr);
        REG_WRITE(GPIO_OUT_W1TS_REG, wr);

        REG_WRITE(GPIO_OUT_W1TS_REG, setLo);
        REG_WRITE(GPIO_OUT_W1TC_REG, clrLo);
        REG_WRITE(GPIO_OUT_W1TC_REG, wr);
        REG_WRITE(GPIO_OUT_W1TS_REG, wr);
    }
#else
    digitalWrite(_dc, HIGH);
    while (count--) {
        write8(hi);
        write8(lo);
    }
#endif
}

// ── Address window ────────────────────────────────────────────────────────────

void ILI9341ParallelDriver::setAddrWindow(int16_t x0, int16_t y0, int16_t x1, int16_t y1) {
    writeCmd(0x2A);
    writeData(x0 >> 8); writeData(x0 & 0xFF);
    writeData(x1 >> 8); writeData(x1 & 0xFF);
    writeCmd(0x2B);
    writeData(y0 >> 8); writeData(y0 & 0xFF);
    writeData(y1 >> 8); writeData(y1 & 0xFF);
    writeCmd(0x2C);
}

// ── Init sequence ─────────────────────────────────────────────────────────────

void ILI9341ParallelDriver::initSequence() {
    writeCmd(0x01); delay(150); // Software reset

    writeCmd(0xEF); writeData(0x03); writeData(0x80); writeData(0x02);
    writeCmd(0xCF); writeData(0x00); writeData(0xC1); writeData(0x30);
    writeCmd(0xED); writeData(0x64); writeData(0x03); writeData(0x12); writeData(0x81);
    writeCmd(0xE8); writeData(0x85); writeData(0x00); writeData(0x78);
    writeCmd(0xCB); writeData(0x39); writeData(0x2C); writeData(0x00); writeData(0x34); writeData(0x02);
    writeCmd(0xF7); writeData(0x20);
    writeCmd(0xEA); writeData(0x00); writeData(0x00);

    writeCmd(0xC0); writeData(0x23);
    writeCmd(0xC1); writeData(0x10);
    writeCmd(0xC5); writeData(0x3E); writeData(0x28);
    writeCmd(0xC7); writeData(0x86);

    writeCmd(0x36); writeData(0x20); // MADCTL: landscape, RGB
    writeCmd(0x3A); writeData(0x55); // 16bpp

    writeCmd(0xB1); writeData(0x00); writeData(0x18);
    writeCmd(0xB6); writeData(0x08); writeData(0x82); writeData(0x27);
    writeCmd(0xF2); writeData(0x00);
    writeCmd(0x26); writeData(0x01);

    writeCmd(0xE0);
    writeData(0x0F); writeData(0x31); writeData(0x2B); writeData(0x0C);
    writeData(0x0E); writeData(0x08); writeData(0x4E); writeData(0xF1);
    writeData(0x37); writeData(0x07); writeData(0x10); writeData(0x03);
    writeData(0x0E); writeData(0x09); writeData(0x00);
    writeCmd(0xE1);
    writeData(0x00); writeData(0x0E); writeData(0x14); writeData(0x03);
    writeData(0x11); writeData(0x07); writeData(0x31); writeData(0xC1);
    writeData(0x48); writeData(0x08); writeData(0x0F); writeData(0x0C);
    writeData(0x31); writeData(0x36); writeData(0x0F);

    writeCmd(0x11); delay(120); // Sleep Out
    writeCmd(0x29);             // Display On
}

// ── IDriver interface ─────────────────────────────────────────────────────────

void ILI9341ParallelDriver::init() {
    Serial.println("[ParDrv] init start");
    Serial.printf("[ParDrv] CS=%d DC=%d WR=%d\n", _cs, _dc, _wr);
    Serial.printf("[ParDrv] DB: %d %d %d %d %d %d %d %d\n",
        _db[0],_db[1],_db[2],_db[3],_db[4],_db[5],_db[6],_db[7]);

    if (_rst >= 0) {
        pinMode(_rst, OUTPUT);
        digitalWrite(_rst, HIGH); delay(10);
        digitalWrite(_rst, LOW);  delay(20);
        digitalWrite(_rst, HIGH); delay(150);
    }

    pinMode(_cs, OUTPUT); digitalWrite(_cs, HIGH);
    pinMode(_dc, OUTPUT); digitalWrite(_dc, HIGH);
    pinMode(_wr, OUTPUT); digitalWrite(_wr, HIGH);
    for (int i = 0; i < 8; i++) {
        pinMode(_db[i], OUTPUT);
        digitalWrite(_db[i], LOW);
    }

    buildLUT();
    _gfx = new _ParGFX(this, _w, _h);

    digitalWrite(_cs, LOW); // Assert CS; stay low for all transactions
    initSequence();

    Serial.println("[ParDrv] init done");
}

void ILI9341ParallelDriver::fillScreen(uint16_t color) {
    Serial.printf("[ParDrv] fillScreen 0x%04X\n", color);
    setAddrWindow(0, 0, _w - 1, _h - 1);
    writeBurst16(color, (int32_t)_w * _h);
}

void ILI9341ParallelDriver::fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) {
    if (x < 0 || y < 0 || w <= 0 || h <= 0) return;
    if (x + w > _w) w = _w - x;
    if (y + h > _h) h = _h - y;
    setAddrWindow(x, y, x + w - 1, y + h - 1);
    writeBurst16(color, (int32_t)w * h);
}

void ILI9341ParallelDriver::drawRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) {
    _gfx->drawRect(x, y, w, h, color);
}

void ILI9341ParallelDriver::fillCircle(int16_t x, int16_t y, int16_t r, uint16_t color) {
    _gfx->fillCircle(x, y, r, color);
}

void ILI9341ParallelDriver::drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color) {
    _gfx->drawLine(x0, y0, x1, y1, color);
}

void ILI9341ParallelDriver::drawPixel(int16_t x, int16_t y, uint16_t color) {
    writePixelRaw(x, y, color);
}

void ILI9341ParallelDriver::writePixelRaw(int16_t x, int16_t y, uint16_t color) {
    if (x < 0 || y < 0 || x >= _w || y >= _h) return;
    setAddrWindow(x, y, x, y);
    digitalWrite(_dc, HIGH);
    write8(color >> 8);
    write8(color & 0xFF);
}

void ILI9341ParallelDriver::drawRGBBitmap(int16_t x, int16_t y, int16_t w, int16_t h, const uint16_t* data) {
    setAddrWindow(x, y, x + w - 1, y + h - 1);
    digitalWrite(_dc, HIGH);
    int32_t total = (int32_t)w * h;
    for (int32_t i = 0; i < total; i++) {
        write8(data[i] >> 8);
        write8(data[i] & 0xFF);
    }
}

void ILI9341ParallelDriver::setCursor(int16_t x, int16_t y) {
    _curX = x; _curY = y;
    _gfx->setCursor(x, y);
}

void ILI9341ParallelDriver::setTextColor(uint16_t color) {
    _textColor = color;
    _textBgColor = color; // reset to transparent (fg==bg means no bg fill in Adafruit_GFX)
    _gfx->setTextColor(color);
}

void ILI9341ParallelDriver::setTextBgColor(uint16_t bg) {
    _textBgColor = bg;
    _gfx->setTextColor(_textColor, _textBgColor);
}

void ILI9341ParallelDriver::setTextSize(uint8_t size) {
    _textSize = size;
    _gfx->setTextSize(size);
}

void ILI9341ParallelDriver::print(const char* str) {
    _gfx->print(str);
}

void ILI9341ParallelDriver::print(int n) {
    _gfx->print(n);
}

int16_t ILI9341ParallelDriver::width()  const { return _w; }
int16_t ILI9341ParallelDriver::height() const { return _h; }

void ILI9341ParallelDriver::setRotation(uint8_t r) {
    _rotation = r & 3;
    writeCmd(0x36);
    switch (_rotation) {
        case 0: writeData(0x40);             _w = 240; _h = 320; break;
        case 1: writeData(0x20);             _w = 320; _h = 240; break;
        case 2: writeData(0x80);             _w = 240; _h = 320; break;
        case 3: writeData(0x40|0x80|0x20);   _w = 320; _h = 240; break;
    }
    // Do NOT call _gfx->setRotation() — hardware MADCTL handles rotation.
    // Keeping _gfx at rotation-0 preserves correct _width/_height (320x240 landscape).
}

} // namespace NanoUI
