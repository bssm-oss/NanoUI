/**
 * @file basic_button.ino
 * @brief Basic button and label demo for NanoUI
 * 
 * Loads a simple UI from PROGMEM flash memory.
 * Demonstrates button press callbacks and runtime label updates.
 */

#include <NanoUI.h>

// Pin definitions for ILI9341
#define TFT_CS   10
#define TFT_DC   9
#define TFT_RST  8
#define TOUCH_CS 7

// UI definition stored in PROGMEM to save SRAM
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
          "id": "lbl_title",
          "x": 10, "y": 10,
          "text": "NanoUI Basic Demo",
          "style": { "color": "#FFFFFF", "fontSize": 2, "align": "left" }
        },
        {
          "type": "button",
          "id": "btn_press",
          "x": 90, "y": 100, "width": 140, "height": 50,
          "label": "PRESS ME",
          "style": { "bg": "#00B4D8", "text": "#FFFFFF", "radius": 8, "fontSize": 2 },
          "onPress": "buttonPressed"
        },
        {
          "type": "label",
          "id": "lbl_millis",
          "x": 10, "y": 200,
          "text": "Uptime: 0ms",
          "style": { "color": "#FFD166", "fontSize": 1, "align": "left" }
        }
      ]
    }
  ]
})";

NanoUI::NanoUI ui;

void onButtonPressed(NanoUI::UIEvent evt) {
    Serial.println("Button pressed!");
    ui.setText("lbl_title", "Button Clicked!");
}

void setup() {
    Serial.begin(115200);
    while (!Serial) { ; }
    Serial.println("NanoUI Basic Button Demo");

    ui.begin(TFT_CS, TFT_DC, TFT_RST, TOUCH_CS);

    if (!ui.loadFromFlash(UI_JSON)) {
        Serial.println("Failed to load UI!");
        return;
    }

    ui.on("buttonPressed", onButtonPressed);
    Serial.println("UI loaded. Touch the button!");
}

void loop() {
    ui.update();

    // Update uptime label every 500ms
    static unsigned long lastUpdate = 0;
    if (millis() - lastUpdate > 500) {
        lastUpdate = millis();
        char buf[32];
        snprintf(buf, sizeof(buf), "Uptime: %lums", millis());
        ui.setText("lbl_millis", buf);
    }
}
