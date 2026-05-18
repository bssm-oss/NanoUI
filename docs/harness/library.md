# 하네스: 라이브러리 구조 (Library Harness)

## 역할

NanoUI 라이브러리의 **각 모듈이 어떤 계약(contract)을 가지는지** 정의한다.
소스를 읽지 않아도 모듈 경계·책임·공개 인터페이스를 파악할 수 있다.

---

## 모듈 맵

```
NanoUI (공개 API)
  │
  ├── drivers/IDriver.h          — 디스플레이 드라이버 인터페이스
  │     └── ILI9341Driver        — ILI9341 구체 구현
  │
  ├── core/UIParser              — JSON → UIScreen[] 변환
  ├── core/UIRenderer            — UIScreen → 픽셀 렌더링
  ├── core/UIScreen              — 화면 단위 컴포넌트 컨테이너
  ├── core/TouchHandler          — 저항막 터치 읽기 + 좌표 정규화
  │
  └── components/
        ├── UIComponent          — 모든 컴포넌트 기반 클래스 (id, x, y, visible, enabled)
        ├── UIButton             — 클릭 이벤트 + navigate 액션
        ├── UILabel              — 텍스트 표시 (align, fontSize, color)
        ├── UIToggle             — ON/OFF 상태 + onChange 콜백
        ├── UISlider             — min/max 값 + onChange 콜백
        └── UIImage              — PROGMEM 이미지 심볼 렌더링
```

---

## 모듈별 계약

### `NanoUI` (NanoUI.h)

| 책임 | 상태를 바꾸는 함수 | 읽기 전용 |
|------|--------------------|-----------|
| 초기화 | `begin(cs, dc, rst, touch_cs)` | — |
| UI 로드 | `loadFromFlash()`, `loadFromSD()` | — |
| 화면 전환 | `show(id)`, `back()` | `getCurrentScreen()` |
| 런타임 | `update()` | — |
| 컴포넌트 조작 | `setText()`, `setValue()`, `setVisible()`, `setEnabled()` | `find(id)` |

`update()` 는 `loop()` 에서 매 프레임 호출. 터치 읽기 + 이벤트 발행 + 더티 컴포넌트 재렌더링을 한 번에 처리.

---

### `UIParser` (core/UIParser.h)

- **입력**: JSON 문자열(PROGMEM) 또는 SD 파일 경로
- **출력**: `UIScreen[]` 배열 채움 + 화면 수 반환
- **사이드이펙트**: `UIComponent*` 객체 동적 할당 (`UIScreen` 소멸자에서 해제)
- **메모리 제한**: `MAX_SCREENS`, `MAX_COMPONENTS_PER_SCREEN` 상수로 제어

---

### `IDriver` (drivers/IDriver.h)

디스플레이 드라이버가 구현해야 할 인터페이스:

```cpp
void init();
void fillScreen(uint16_t color);
void fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
void drawRoundRect(…);
void drawString(…);
void setRotation(uint8_t r);
int16_t width();
int16_t height();
```

ILI9341 외 다른 드라이버(ST7789 등)를 추가하려면 `IDriver`를 상속하고 `ui.setDriver()`로 교체.

---

### `TouchHandler` (core/TouchHandler.h)

- `readRaw(x, y, z)` → 원시 ADC 값
- `update()` → 눌림 상태 갱신, 좌표 정규화
- `isPressed()`, `getX()`, `getY()` → NanoUI.update()에서 폴링

기본 구현은 저항막 터치(4선). XPT2046 같은 전용 IC를 쓰려면 `TouchHandler` 상속 후 `ui.setTouchHandler()` 교체.

---

### `UIEvent` 구조체

```cpp
struct UIEvent {
    char id[MAX_ID_LEN];  // 컴포넌트 ID 또는 onPress 액션 이름
    int value;            // 슬라이더 값 (0~100 기본)
    bool toggled;         // 토글 현재 상태
};
```

콜백은 `ui.on("actionName", callback)` 으로 등록.

---

## 메모리 상수 (platformio.ini build_flags에서 재정의 가능)

| 상수 | 기본값 | 의미 |
|------|--------|------|
| `MAX_COMPONENTS_PER_SCREEN` | 20 | 화면당 최대 컴포넌트 수 |
| `MAX_SCREENS` | 5 | 최대 화면 수 |
| `MAX_HISTORY_DEPTH` | 5 | back() 히스토리 깊이 |
| `MAX_CALLBACKS` | 10 | 등록 가능한 콜백 수 |
| `MAX_IMAGE_SYMBOLS` | 10 | 등록 가능한 이미지 심볼 수 |
| `MAX_ID_LEN` | 16 | ID 문자열 최대 길이 |
