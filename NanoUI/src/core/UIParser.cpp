#include "UIParser.h"
#include "UIScreen.h"
#include "../components/UIButton.h"
#include "../components/UILabel.h"
#include "../components/UIToggle.h"
#include "../components/UISlider.h"
#include "../components/UIImage.h"
#include <string.h>

namespace NanoUI {

UIParser::UIParser() {
    _lastError[0] = '\0';
}

uint16_t UIParser::parseColor(const char* hexStr) {
    if (!hexStr || hexStr[0] != '#') return 0x0000;
    
    long rgb = strtol(hexStr + 1, NULL, 16);
    uint8_t r = (rgb >> 16) & 0xFF;
    uint8_t g = (rgb >> 8) & 0xFF;
    uint8_t b = rgb & 0xFF;
    
    return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3);
}

int UIParser::parseFromFlash(const char* jsonStr, UIScreen* screens, int maxScreens) {
    if (!jsonStr || !screens || maxScreens <= 0) {
        strncpy(_lastError, "Invalid arguments", sizeof(_lastError));
        return 0;
    }

    JsonDocument doc;
    DeserializationError error = deserializeJson(doc, jsonStr);

    if (error) {
        strncpy(_lastError, error.c_str(), sizeof(_lastError));
        return 0;
    }

    JsonArray screensArr = doc["screens"];
    if (!screensArr) {
        strncpy(_lastError, "Missing screens array", sizeof(_lastError));
        return 0;
    }

    int count = 0;
    for (JsonObject screenObj : screensArr) {
        if (count >= maxScreens) break;
        if (parseScreen(screenObj, screens[count])) {
            count++;
        }
    }

    return count;
}

int UIParser::parseFromSD(const char* path, UIScreen* screens, int maxScreens) {
    // SD card support can be added here by opening the file and streaming to ArduinoJson
    // For now, return error to indicate not implemented in base
    strncpy(_lastError, "SD not implemented", sizeof(_lastError));
    (void)path;
    (void)screens;
    (void)maxScreens;
    return -1;
}

bool UIParser::parseScreen(JsonObject screenObj, UIScreen& screen) {
    const char* id = screenObj["id"];
    if (!id) {
        strncpy(_lastError, "Screen missing id", sizeof(_lastError));
        return false;
    }
    screen.setId(id);

    const char* bg = screenObj["background"];
    screen.backgroundColor = bg ? parseColor(bg) : 0x0000;

    JsonArray componentsArr = screenObj["components"];
    if (!componentsArr) return true;

    for (JsonObject compObj : componentsArr) {
        if (screen.componentCount >= MAX_COMPONENTS_PER_SCREEN) break;
        UIComponent* comp = parseComponent(compObj);
        if (comp) {
            screen.addComponent(comp);
        }
    }

    return true;
}

UIComponent* UIParser::parseComponent(JsonObject compObj) {
    const char* type = compObj["type"];
    if (!type) return nullptr;

    UIComponent* comp = nullptr;

    if (strcmp(type, "button") == 0) {
        comp = new UIButton();
    } else if (strcmp(type, "label") == 0) {
        comp = new UILabel();
    } else if (strcmp(type, "toggle") == 0) {
        comp = new UIToggle();
    } else if (strcmp(type, "slider") == 0) {
        comp = new UISlider();
    } else if (strcmp(type, "image") == 0) {
        comp = new UIImage();
    } else {
        return nullptr;
    }

    // Parse common properties
    const char* id = compObj["id"];
    if (id) strncpy(comp->id, id, MAX_ID_LEN - 1);
    comp->id[MAX_ID_LEN - 1] = '\0';

    comp->x = compObj["x"] | 0;
    comp->y = compObj["y"] | 0;
    comp->width = compObj["width"] | 0;
    comp->height = compObj["height"] | 0;

    // Parse type-specific properties
    if (strcmp(type, "button") == 0) {
        UIButton* btn = static_cast<UIButton*>(comp);
        const char* label = compObj["label"];
        if (label) btn->setText(label);
        const char* action = compObj["onPress"];
        if (action) btn->setAction(action);
        JsonObject onPressObj = compObj["onPress"];
        if (onPressObj) {
            const char* act = onPressObj["action"];
            if (act && strcmp(act, "navigate") == 0) {
                btn->actionType = UIButton::ActionType::Navigate;
                const char* target = onPressObj["target"];
                if (target) btn->setTarget(target);
            }
        }
    } else if (strcmp(type, "label") == 0) {
        UILabel* lbl = static_cast<UILabel*>(comp);
        const char* text = compObj["text"];
        if (text) lbl->setText(text);
    } else if (strcmp(type, "toggle") == 0) {
        UIToggle* tgl = static_cast<UIToggle*>(comp);
        tgl->toggled = compObj["default"] | false;
        const char* action = compObj["onChange"];
        if (action) tgl->setAction(action);
    } else if (strcmp(type, "slider") == 0) {
        UISlider* sld = static_cast<UISlider*>(comp);
        sld->minValue = compObj["min"] | 0;
        sld->maxValue = compObj["max"] | 100;
        sld->setValue(compObj["value"] | sld->minValue);
        const char* action = compObj["onChange"];
        if (action) sld->setAction(action);
    } else if (strcmp(type, "image") == 0) {
        UIImage* img = static_cast<UIImage*>(comp);
        const char* src = compObj["src"];
        if (src) img->setSource(src);
    }

    // Parse style
    parseStyle(compObj["style"], comp);

    return comp;
}

void UIParser::parseStyle(JsonObject styleObj, UIComponent* comp) {
    if (!styleObj || !comp) return;

    const char* type = comp->getType();

    if (strcmp(type, "button") == 0) {
        UIButton* btn = static_cast<UIButton*>(comp);
        const char* bg = styleObj["bg"];
        if (bg) btn->bgColor = parseColor(bg);
        const char* text = styleObj["text"];
        if (text) btn->textColor = parseColor(text);
        btn->radius = styleObj["radius"] | 4;
        btn->fontSize = styleObj["fontSize"] | 2;
    } else if (strcmp(type, "label") == 0) {
        UILabel* lbl = static_cast<UILabel*>(comp);
        const char* color = styleObj["color"];
        if (color) lbl->textColor = parseColor(color);
        lbl->fontSize = styleObj["fontSize"] | 2;
        const char* align = styleObj["align"];
        if (align) lbl->align = align[0];
    } else if (strcmp(type, "toggle") == 0) {
        UIToggle* tgl = static_cast<UIToggle*>(comp);
        const char* active = styleObj["activeColor"];
        if (active) tgl->activeColor = parseColor(active);
        const char* inactive = styleObj["inactiveColor"];
        if (inactive) tgl->inactiveColor = parseColor(inactive);
    } else if (strcmp(type, "slider") == 0) {
        UISlider* sld = static_cast<UISlider*>(comp);
        const char* track = styleObj["trackColor"];
        if (track) sld->trackColor = parseColor(track);
        const char* thumb = styleObj["thumbColor"];
        if (thumb) sld->thumbColor = parseColor(thumb);
    }
}

} // namespace NanoUI
