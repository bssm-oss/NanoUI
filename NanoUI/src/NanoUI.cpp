#include "NanoUI.h"
#include <string.h>

namespace NanoUI {

// Simple built-in touch handler using Adafruit TouchScreen
class DefaultTouchHandler : public TouchHandler {
private:
    int _xp, _yp, _xm, _ym;
public:
    DefaultTouchHandler(int xp, int yp, int xm, int ym) : _xp(xp), _yp(yp), _xm(xm), _ym(ym) {}
    bool readRaw(int16_t& x, int16_t& y, int16_t& z) override {
        // Simple resistive touch reading without external library
        pinMode(_yp, INPUT);
        pinMode(_ym, INPUT);
        pinMode(_xp, OUTPUT);
        pinMode(_xm, OUTPUT);
        digitalWrite(_xp, HIGH);
        digitalWrite(_xm, LOW);
        delayMicroseconds(20);
        x = analogRead(_yp);

        pinMode(_xp, INPUT);
        pinMode(_xm, INPUT);
        pinMode(_yp, OUTPUT);
        pinMode(_ym, OUTPUT);
        digitalWrite(_yp, HIGH);
        digitalWrite(_ym, LOW);
        delayMicroseconds(20);
        y = analogRead(_xp);

        pinMode(_xp, OUTPUT);
        digitalWrite(_xp, LOW);
        pinMode(_ym, OUTPUT);
        digitalWrite(_ym, HIGH);
        pinMode(_yp, INPUT);
        pinMode(_xm, INPUT);
        delayMicroseconds(20);
        z = analogRead(_xm);

        if (z > 10) {
            return true;
        }
        return false;
    }
};

NanoUI::NanoUI() 
    : _driver(nullptr), _iliDriver(nullptr), _renderer(nullptr),
      _parser(nullptr), _touchHandler(nullptr), _ownsTouchHandler(false),
      _screenCount(0), _currentScreen(nullptr), _historyIndex(-1),
      _callbackCount(0), _touchWasPressed(false), _lastTouchX(0), _lastTouchY(0) {
    for (int i = 0; i < MAX_HISTORY_DEPTH; i++) {
        _history[i] = nullptr;
    }
}

NanoUI::~NanoUI() {
    if (_renderer) delete _renderer;
    if (_parser) delete _parser;
    if (_iliDriver) delete _iliDriver;
    if (_ownsTouchHandler && _touchHandler) delete _touchHandler;
}

void NanoUI::begin(int tft_cs, int tft_dc, int tft_rst, int touch_cs) {
    (void)touch_cs; // May be used for XPT2046 in future

    _iliDriver = new ILI9341Driver((int8_t)tft_cs, (int8_t)tft_dc, (int8_t)tft_rst);
    _driver = _iliDriver;
    _driver->init();
    _driver->fillScreen(0x0000);

    _renderer = new UIRenderer(_driver);
    _parser = new UIParser();

    // Default touch handler using common pin mapping
    // A2=YP, A3=XM, A0=YP? Actually common: XP=A2, YM=A3, XM=A0, YP=A1
    // Using a generic mapping; users should override with setTouchHandler()
    _touchHandler = new DefaultTouchHandler(A2, A1, A0, A3);
    _ownsTouchHandler = true;
    _touchHandler->setDisplaySize(_driver->width(), _driver->height());
}

void NanoUI::setDriver(IDriver* driver) {
    if (_iliDriver) {
        delete _iliDriver;
        _iliDriver = nullptr;
    }
    _driver = driver;
    if (_renderer) {
        _renderer->setDriver(driver);
    }
}

bool NanoUI::loadFromSD(const char* path) {
    if (!_parser) return false;
    _screenCount = _parser->parseFromSD(path, _screens, MAX_SCREENS);
    return _screenCount > 0;
}

bool NanoUI::loadFromFlash(const char* jsonStr) {
    if (!_parser) return false;
    _screenCount = _parser->parseFromFlash(jsonStr, _screens, MAX_SCREENS);
    if (_screenCount > 0) {
        // Auto-show first screen
        show(_screens[0].id);
    }
    return _screenCount > 0;
}

void NanoUI::show(const char* screenId) {
    UIScreen* target = findScreen(screenId);
    if (!target || target == _currentScreen) return;

    if (_currentScreen) {
        pushHistory(_currentScreen);
    }

    _currentScreen = target;
    renderCurrentScreen();
}

void NanoUI::back() {
    UIScreen* prev = popHistory();
    if (prev) {
        _currentScreen = prev;
        renderCurrentScreen();
    }
}

void NanoUI::on(const char* eventName, void(*callback)(UIEvent)) {
    if (_callbackCount >= MAX_CALLBACKS || !eventName || !callback) return;
    strncpy(_callbacks[_callbackCount].eventName, eventName, MAX_ID_LEN - 1);
    _callbacks[_callbackCount].eventName[MAX_ID_LEN - 1] = '\0';
    _callbacks[_callbackCount].callback = callback;
    _callbackCount++;
}

void NanoUI::update() {
    if (!_touchHandler || !_currentScreen) return;

    bool changed = _touchHandler->update();
    bool pressed = _touchHandler->isPressed();
    int16_t tx = _touchHandler->getX();
    int16_t ty = _touchHandler->getY();

    // Handle touch release events
    if (_touchWasPressed && !pressed) {
        for (uint8_t i = 0; i < _currentScreen->componentCount; i++) {
            UIComponent* comp = _currentScreen->components[i];
            if (comp && comp->enabled && comp->visible) {
                if (comp->contains(_lastTouchX, _lastTouchY)) {
                    comp->handleTouch(_lastTouchX, _lastTouchY, false);
                    fireEvent(comp);
                } else {
                    comp->handleTouch(_lastTouchX, _lastTouchY, false);
                }
            }
        }
    }

    // Handle touch press/move
    if (pressed) {
        for (uint8_t i = 0; i < _currentScreen->componentCount; i++) {
            UIComponent* comp = _currentScreen->components[i];
            if (comp && comp->enabled && comp->visible) {
                if (comp->contains(tx, ty)) {
                    comp->handleTouch(tx, ty, true);
                }
            }
        }
    }

    _touchWasPressed = pressed;
    _lastTouchX = tx;
    _lastTouchY = ty;

    // Redraw components that need it
    for (uint8_t i = 0; i < _currentScreen->componentCount; i++) {
        UIComponent* comp = _currentScreen->components[i];
        if (comp && comp->needsRedraw && comp->visible) {
            comp->draw(_driver);
            comp->needsRedraw = false;
        }
    }
}

void NanoUI::setText(const char* id, const char* text) {
    UIComponent* comp = find(id);
    if (comp) comp->setText(text);
}

void NanoUI::setValue(const char* id, int value) {
    UIComponent* comp = find(id);
    if (comp) comp->setValue(value);
}

void NanoUI::setVisible(const char* id, bool visible) {
    UIComponent* comp = find(id);
    if (comp) {
        comp->visible = visible;
        if (_renderer && _currentScreen) {
            _renderer->renderScreen(_currentScreen);
        }
    }
}

void NanoUI::setEnabled(const char* id, bool enabled) {
    UIComponent* comp = find(id);
    if (comp) comp->enabled = enabled;
}

UIComponent* NanoUI::find(const char* id) {
    if (!_currentScreen) return nullptr;
    return _currentScreen->findComponent(id);
}

void NanoUI::pushHistory(UIScreen* screen) {
    if (_historyIndex < MAX_HISTORY_DEPTH - 1) {
        _history[++_historyIndex] = screen;
    } else {
        // Shift history down
        for (int i = 0; i < MAX_HISTORY_DEPTH - 1; i++) {
            _history[i] = _history[i + 1];
        }
        _history[MAX_HISTORY_DEPTH - 1] = screen;
    }
}

UIScreen* NanoUI::popHistory() {
    if (_historyIndex >= 0) {
        return _history[_historyIndex--];
    }
    return nullptr;
}

UIScreen* NanoUI::findScreen(const char* id) {
    for (int i = 0; i < _screenCount; i++) {
        if (strcmp(_screens[i].id, id) == 0) {
            return &_screens[i];
        }
    }
    return nullptr;
}

void NanoUI::fireEvent(const char* id, int value, bool toggled) {
    UIEvent evt;
    strncpy(evt.id, id, MAX_ID_LEN - 1);
    evt.id[MAX_ID_LEN - 1] = '\0';
    evt.value = value;
    evt.toggled = toggled;

    for (int i = 0; i < _callbackCount; i++) {
        // Match by component id or action name depending on component type
        if (strcmp(_callbacks[i].eventName, id) == 0) {
            _callbacks[i].callback(evt);
        }
    }
}

void NanoUI::fireEvent(UIComponent* comp) {
    if (!comp) return;

    const char* action = nullptr;
    int value = 0;
    bool toggled = false;

    if (strcmp(comp->getType(), "button") == 0) {
        UIButton* btn = static_cast<UIButton*>(comp);
        if (btn->actionType == UIButton::ActionType::Navigate) {
            show(btn->navigateTarget);
            return;
        }
        action = btn->actionName;
    } else if (strcmp(comp->getType(), "toggle") == 0) {
        UIToggle* tgl = static_cast<UIToggle*>(comp);
        action = tgl->actionName;
        value = tgl->getValue();
        toggled = tgl->toggled;
    } else if (strcmp(comp->getType(), "slider") == 0) {
        UISlider* sld = static_cast<UISlider*>(comp);
        action = sld->actionName;
        value = sld->getValue();
    }

    if (action && action[0] != '\0') {
        fireEvent(action, value, toggled);
    }
}

void NanoUI::renderCurrentScreen() {
    if (_renderer && _currentScreen) {
        _renderer->renderScreen(_currentScreen);
    }
}

} // namespace NanoUI
