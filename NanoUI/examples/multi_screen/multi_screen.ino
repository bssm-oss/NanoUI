/**
 * @file multi_screen.ino
 * @brief Multi-screen navigation demo for NanoUI
 * 
 * Demonstrates screen switching with navigate actions
 * and the back() history stack functionality.
 */

#include <NanoUI.h>

#define TFT_CS   10
#define TFT_DC   9
#define TFT_RST  8
#define TOUCH_CS 7

const char UI_JSON[] PROGMEM = R"({
  "version": "1.0",
  "display": { "width": 320, "height": 240, "rotation": 1 },
  "screens": [
    {
      "id": "main",
      "background": "#1A1A2E",
      "components": [
        {
          "type": "label",
          "id": "lbl_main",
          "x": 60, "y": 20,
          "text": "Main Screen",
          "style": { "color": "#FFFFFF", "fontSize": 3, "align": "left" }
        },
        {
          "type": "button",
          "id": "btn_settings",
          "x": 90, "y": 100, "width": 140, "height": 50,
          "label": "SETTINGS",
          "style": { "bg": "#00B4D8", "text": "#FFFFFF", "radius": 8, "fontSize": 2 },
          "onPress": { "action": "navigate", "target": "settings" }
        }
      ]
    },
    {
      "id": "settings",
      "background": "#0D0D0D",
      "components": [
        {
          "type": "label",
          "id": "lbl_settings",
          "x": 50, "y": 20,
          "text": "Settings",
          "style": { "color": "#FFFFFF", "fontSize": 3, "align": "left" }
        },
        {
          "type": "button",
          "id": "btn_back",
          "x": 10, "y": 10, "width": 80, "height": 40,
          "label": "BACK",
          "style": { "bg": "#555555", "text": "#FFFFFF", "radius": 4, "fontSize": 2 },
          "onPress": { "action": "navigate", "target": "main" }
        },
        {
          "type": "toggle",
          "id": "tog_dark",
          "x": 120, "y": 100, "width": 80, "height": 36,
          "default": true,
          "style": { "activeColor": "#06D6A0", "inactiveColor": "#555555" },
          "onChange": "darkModeChanged"
        }
      ]
    }
  ]
})";

NanoUI::NanoUI ui;

void onDarkModeChanged(NanoUI::UIEvent evt) {
    Serial.print("Dark mode: ");
    Serial.println(evt.toggled ? "ON" : "OFF");
}

void setup() {
    Serial.begin(115200);
    while (!Serial) { ; }
    Serial.println("NanoUI Multi-Screen Demo");

    ui.begin(TFT_CS, TFT_DC, TFT_RST, TOUCH_CS);

    if (!ui.loadFromFlash(UI_JSON)) {
        Serial.println("Failed to load UI!");
        return;
    }

    ui.on("darkModeChanged", onDarkModeChanged);
    Serial.println("Tap SETTINGS to navigate. Use BACK to return.");
}

void loop() {
    ui.update();
}
