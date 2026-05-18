# 하네스: 빌드 검증 (Build Harness)

## 역할

소스 변경 후 **펌웨어가 컴파일 가능한 상태인지** 확인한다.
보드 없이도 실행 가능하며, 변경의 안전성을 가장 빠르게 검증하는 첫 번째 게이트다.

---

## 입력 (트리거 조건)

- `src/` 하위 파일 변경
- `examples/` 하위 파일 변경
- `platformio.ini` 또는 `library.json` 변경

## 출력 (성공 기준)

```
[SUCCESS] Took XX.XX seconds
```

모든 `.cpp` / `.ino` 파일이 에러 없이 컴파일 완료.

## 실패 기준

```
[FAILED] ...
```

컴파일 에러 또는 링커 에러 발생.

---

## 명령어

```bash
PIO=~/.platformio/penv/bin/pio

# 기본 예제(basic_button) 빌드
$PIO run

# 특정 예제 클린 빌드
$PIO run -t clean && $PIO run

# 예제별 독립 빌드 (보드 없이 전체 확인)
cd examples/basic_button    && $PIO run && cd ../..
cd examples/multi_screen    && $PIO run && cd ../..
cd examples/sensor_dashboard && $PIO run && cd ../..
```

---

## 라이브러리 의존성 구조

```
NanoUI (src/)
  ├── Adafruit ILI9341  ^1.6.0  — 디스플레이 드라이버 HAL
  ├── Adafruit GFX      ^1.11.0 — 2D 그래픽 프리미티브
  ├── ArduinoJson       ^7.0.0  — JSON 파서 (PROGMEM 지원)
  └── Adafruit BusIO    ^1.14.0 — SPI/I2C 추상화 (GFX 의존성)
```

의존성은 `lib_extra_dirs = ${PROJECT_DIR}/..` 로 NanoUI 자체 소스를 자동 탐색.

---

## 에이전트 행동 규칙

1. 소스를 수정했으면 **반드시 빌드를 실행**하고 SUCCESS를 확인한다.
2. FAILED 가 나오면 수정 없이 다음 단계(Upload)로 진행하지 않는다.
3. 에러 메시지를 그대로 다음 에이전트에 전달할 때는 stderr 전체를 포함한다.
4. 이 하네스 단계에서는 **보드 연결 여부와 무관**하다.
