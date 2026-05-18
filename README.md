# NanoUI

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)

JSON 기반 터치 디스플레이 UI 라이브러리 for ESP32
사용자가 `.ui.json` 파일 하나로 UI를 정의하면, 아두이노가 이를 파싱하여 ILI9341 3.2인치 터치 디스플레이에 렌더링합니다.

---

## 특징

- **JSON 기반 UI 선언**: C++ 코드 없이 UI 레이아웃을 JSON으로 설계
- **저메모리 최적화**: Arduino Uno/Nano (2KB SRAM) 기준으로 설계
- **컴포넌트 지원**: Button, Label, Toggle, Slider, Image
- **화면 전환**: 히스토리 스택 기반 `show()` / `back()` 네비게이션
- **드라이버 추상화**: ILI9341 외 ST7789 등 추가 용이
- **이벤트 콜백**: 터치 이벤트를 이름 기반 콜백으로 처리

---

## 설치 방법

### PlatformIO

`platformio.ini`에 추가:

```ini
lib_deps =
    https://github.com/bssm-oss/NanoUI.git
    adafruit/Adafruit ILI9341 @ ^1.6.0
    adafruit/Adafruit GFX Library @ ^1.11.0
    bblanchon/ArduinoJson @ ^7.0.0
```

## 빠른 시작

```cpp
#include <NanoUI.h>

const char UI_JSON[] PROGMEM = R"({
  "version": "1.0",
  "display": { "width": 320, "height": 240, "rotation": 1 },
  "screens": [
    {
      "id": "main",
      "background": "#1A1A2E",
      "components": [
        {
          "type": "button",
          "id": "btn_start",
          "x": 90, "y": 100, "width": 140, "height": 50,
          "label": "START",
          "style": { "bg": "#00B4D8", "text": "#FFFFFF", "radius": 8 },
          "onPress": "startMotor"
        }
      ]
    }
  ]
})";

NanoUI::NanoUI ui;

void onStartMotor(NanoUI::UIEvent evt) {
    Serial.println("Motor started!");
}

void setup() {
    Serial.begin(115200);
    ui.begin(10, 9, 8, 7);  // CS, DC, RST, TOUCH_CS
    ui.loadFromFlash(UI_JSON);
    ui.on("startMotor", onStartMotor);
}

void loop() {
    ui.update();
}
```

---

## `.ui.json` 전체 스펙

### 루트 객체

| 필드 | 타입 | 설명 |
|------|------|------|
| `version` | string | UI 정의 버전 (예: `"1.0"`) |
| `display` | object | 디스플레이 설정 |
| `screens` | array | 화면 정의 배열 |

### display 객체

| 필드 | 타입 | 기본값 | 설명 |
|------|------|--------|------|
| `width` | number | 320 | 화면 폭 (픽셀) |
| `height` | number | 240 | 화면 높이 (픽셀) |
| `rotation` | number | 1 | 회전 각도 (0~3) |

### screen 객체

| 필드 | 타입 | 설명 |
|------|------|------|
| `id` | string | 화면 고유 식별자 |
| `background` | string | 배경색 `"#RRGGBB"` |
| `components` | array | 컴포넌트 배열 |

### 컴포넌트 공통 속성

| 필드 | 타입 | 필수 | 설명 |
|------|------|------|------|
| `type` | string | O | `"button"`, `"label"`, `"toggle"`, `"slider"`, `"image"` |
| `id` | string | O | 컴포넌트 고유 ID |
| `x` | number | O | X 좌표 |
| `y` | number | O | Y 좌표 |
| `width` | number | △ | 너비 (일부 컴포넌트 필수) |
| `height` | number | △ | 높이 (일부 컴포넌트 필수) |

### button

| 필드 | 타입 | 설명 |
|------|------|------|
| `label` | string | 버튼 텍스트 |
| `style.bg` | string | 배경색 `"#RRGGBB"` |
| `style.text` | string | 글자색 `"#RRGGBB"` |
| `style.radius` | number | 모서리 반경 (기본: 4) |
| `style.fontSize` | number | 글자 크기 배율 (기본: 2) |
| `onPress` | string / object | 콜백 이름 또는 `{ "action": "navigate", "target": "screenId" }` |

### label

| 필드 | 타입 | 설명 |
|------|------|------|
| `text` | string | 표시할 텍스트 |
| `style.color` | string | 글자색 `"#RRGGBB"` |
| `style.fontSize` | number | 글자 크기 배율 (기본: 2) |
| `style.align` | string | 정렬 `"left"`, `"center"`, `"right"` |

### toggle

| 필드 | 타입 | 설명 |
|------|------|------|
| `default` | boolean | 초기 상태 (기본: false) |
| `style.activeColor` | string | ON 상태 색상 |
| `style.inactiveColor` | string | OFF 상태 색상 |
| `onChange` | string | 상태 변경 시 콜백 이름 |

### slider

| 필드 | 타입 | 설명 |
|------|------|------|
| `min` | number | 최소값 (기본: 0) |
| `max` | number | 최대값 (기본: 100) |
| `value` | number | 초기값 |
| `style.trackColor` | string | 트랙 색상 |
| `style.thumbColor` | string | thumb 색상 |
| `onChange` | string | 값 변경 시 콜백 이름 |

### image

| 필드 | 타입 | 설명 |
|------|------|------|
| `src` | string | 등록된 심볼 이름 |

이미지는 `UIImage::registerSymbol()`로 사전 등록 필요:

```cpp
const uint16_t wifi_icon[] PROGMEM = { ... }; // RGB565 data
NanoUI::UIImage::registerSymbol("wifi_icon", wifi_icon, 50, 50);
```

---

## 공개 API

### 초기화

```cpp
void begin(int tft_cs, int tft_dc, int tft_rst, int touch_cs);
```

### JSON 로드

```cpp
bool loadFromSD(const char* path);
bool loadFromFlash(const char* jsonStr);
```

### 화면 전환

```cpp
void show(const char* screenId);
void back();
```

### 이벤트 콜백

```cpp
void on(const char* eventName, void(*callback)(UIEvent));
```

`UIEvent` 구조체:

```cpp
struct UIEvent {
    const char* id;
    int value;
    bool toggled;
};
```

### 런타임 업데이트

```cpp
void update();  // loop()에서 호출
```

### 컴포넌트 제어

```cpp
void setText(const char* id, const char* text);
void setValue(const char* id, int value);
void setVisible(const char* id, bool visible);
void setEnabled(const char* id, bool enabled);
UIComponent* find(const char* id);
```

---

## 메모리 설정

`#define`으로 제한 조정 가능 (include 전에 정의):

```cpp
#define MAX_COMPONENTS_PER_SCREEN 20
#define MAX_SCREENS 5
#define MAX_HISTORY_DEPTH 5
#define MAX_CALLBACKS 10
#define MAX_IMAGE_SYMBOLS 10
#include <NanoUI.h>
```

---

## 예제

- `basic_button` - 버튼 콜백 + millis() 라벨 업데이트
- `multi_screen` - 화면 전환 + back() 네비게이션
- `sensor_dashboard` - 슬라이더 밝기 조절 + 토글 릴레이 + 센서 값 표시

---

## 라이선스

MIT License - 자세한 내용은 [LICENSE](LICENSE) 파일을 참조하세요.

---

## 기여

Issue와 Pull Request를 환영합니다!
