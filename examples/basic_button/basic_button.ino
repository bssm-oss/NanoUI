/**
 * @file basic_button.ino
 * @brief NanoUI basic button & label demo (8-bit parallel ILI9341)
 *
 * Wiring (LOLIN D32):
 *   CS=27  DC/RS=2  WR=4  RST=33  RD→3.3V
 *   DB0=12  DB1=13  DB2=14  DB3=15
 *   DB4=21  DB5=22  DB6=25  DB7=26
 *   Touch: T_CS=5  SCK=18  MISO=19  MOSI=23
 */

#include <SPI.h>
#include <NanoUI.h>

// ── 디스플레이 핀 ───────────────────────────────────────────────────────────
#define TFT_CS   27
#define TFT_DC    2
#define TFT_WR    4
#define TFT_RST  33

#define DB0  12
#define DB1  13
#define DB2  14
#define DB3  15
#define DB4  21
#define DB5  22
#define DB6  25
#define DB7  26

// ── 터치 SPI 핀 ─────────────────────────────────────────────────────────────
#define TOUCH_CS  5
#define SPI_SCK  18
#define SPI_MISO 19
#define SPI_MOSI 23

// ── UI JSON ─────────────────────────────────────────────────────────────────
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
          "text": "NanoUI Demo",
          "style": { "color": "#FFFFFF", "fontSize": 2, "align": "left" }
        },
        {
          "type": "button",
          "id": "btn_press",
          "x": 90, "y": 90, "width": 140, "height": 50,
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
    delay(3000); // wait for serial monitor to connect before printing startup diagnostics
    Serial.println("NanoUI Basic Button Demo");

    SPI.begin(SPI_SCK, SPI_MISO, SPI_MOSI);

    ui.beginParallel(TFT_CS, TFT_DC, TFT_WR, TFT_RST,
                     DB0, DB1, DB2, DB3,
                     DB4, DB5, DB6, DB7,
                     TOUCH_CS);

    if (!ui.loadFromFlash(UI_JSON)) {
        Serial.println("Failed to load UI!");
        return;
    }

    ui.on("buttonPressed", onButtonPressed);
    Serial.println("UI loaded!");
}

void loop() {
    ui.update();

    static unsigned long lastUpdate = 0;
    if (millis() - lastUpdate > 500) {
        lastUpdate = millis();
        char buf[32];
        snprintf(buf, sizeof(buf), "Uptime: %lums", millis());
        ui.setText("lbl_millis", buf);
    }
}
