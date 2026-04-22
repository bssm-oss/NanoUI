/**
 * @file sensor_dashboard.ino
 * @brief Sensor dashboard demo for NanoUI
 * 
 * Demonstrates slider for LED brightness, toggle for relay control,
 * and real-time analog sensor value updates on a label.
 */

#include <NanoUI.h>

#define TFT_CS   10
#define TFT_DC   9
#define TFT_RST  8
#define TOUCH_CS 7

#define LED_PIN     5
#define RELAY_PIN   6
#define SENSOR_PIN  A0

const char UI_JSON[] PROGMEM = R"({
  "version": "1.0",
  "display": { "width": 320, "height": 240, "rotation": 1 },
  "screens": [
    {
      "id": "dashboard",
      "background": "#1A1A2E",
      "components": [
        {
          "type": "label",
          "id": "lbl_title",
          "x": 10, "y": 10,
          "text": "Sensor Dashboard",
          "style": { "color": "#FFFFFF", "fontSize": 2, "align": "left" }
        },
        {
          "type": "label",
          "id": "lbl_sensor",
          "x": 10, "y": 50,
          "text": "Sensor: ----",
          "style": { "color": "#FFD166", "fontSize": 2, "align": "left" }
        },
        {
          "type": "toggle",
          "id": "tog_relay",
          "x": 200, "y": 50, "width": 80, "height": 36,
          "default": false,
          "style": { "activeColor": "#06D6A0", "inactiveColor": "#555555" },
          "onChange": "setRelay"
        },
        {
          "type": "label",
          "id": "lbl_relay",
          "x": 200, "y": 90,
          "text": "Relay",
          "style": { "color": "#FFFFFF", "fontSize": 1, "align": "center" }
        },
        {
          "type": "slider",
          "id": "sld_brightness",
          "x": 10, "y": 140, "width": 200, "height": 30,
          "min": 0, "max": 255, "value": 128,
          "style": { "trackColor": "#444444", "thumbColor": "#FFD166" },
          "onChange": "setBrightness"
        },
        {
          "type": "label",
          "id": "lbl_brightness",
          "x": 220, "y": 145,
          "text": "128",
          "style": { "color": "#FFFFFF", "fontSize": 2, "align": "left" }
        },
        {
          "type": "label",
          "id": "lbl_status",
          "x": 10, "y": 200,
          "text": "System OK",
          "style": { "color": "#00B4D8", "fontSize": 1, "align": "left" }
        }
      ]
    }
  ]
})";

NanoUI::NanoUI ui;

void onSetBrightness(NanoUI::UIEvent evt) {
    int brightness = evt.value;
    analogWrite(LED_PIN, brightness);

    char buf[8];
    snprintf(buf, sizeof(buf), "%d", brightness);
    ui.setText("lbl_brightness", buf);

    Serial.print("Brightness set to: ");
    Serial.println(brightness);
}

void onSetRelay(NanoUI::UIEvent evt) {
    bool on = evt.toggled;
    digitalWrite(RELAY_PIN, on ? HIGH : LOW);
    ui.setText("lbl_status", on ? "Relay ON" : "Relay OFF");
    Serial.print("Relay: ");
    Serial.println(on ? "ON" : "OFF");
}

void setup() {
    Serial.begin(115200);
    while (!Serial) { ; }
    Serial.println("NanoUI Sensor Dashboard Demo");

    pinMode(LED_PIN, OUTPUT);
    pinMode(RELAY_PIN, OUTPUT);
    analogWrite(LED_PIN, 128);

    ui.begin(TFT_CS, TFT_DC, TFT_RST, TOUCH_CS);

    if (!ui.loadFromFlash(UI_JSON)) {
        Serial.println("Failed to load UI!");
        return;
    }

    ui.on("setBrightness", onSetBrightness);
    ui.on("setRelay", onSetRelay);
    Serial.println("Dashboard loaded.");
}

void loop() {
    ui.update();

    // Update sensor reading every 500ms
    static unsigned long lastUpdate = 0;
    if (millis() - lastUpdate > 500) {
        lastUpdate = millis();
        int sensorValue = analogRead(SENSOR_PIN);
        char buf[24];
        snprintf(buf, sizeof(buf), "Sensor: %d", sensorValue);
        ui.setText("lbl_sensor", buf);
    }
}
