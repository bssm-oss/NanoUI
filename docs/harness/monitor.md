# 하네스: 시리얼 모니터 (Monitor Harness)

## 역할

업로드된 펌웨어의 **런타임 동작을 시리얼 출력으로 검증**한다.
UI 렌더링, 터치 이벤트, 콜백 실행 여부를 사람 또는 에이전트가 확인한다.

---

## 사전 조건

- [업로드 하네스](upload.md) 완료
- 보드가 USB로 연결됨 (동일 세션 또는 재연결)

## 출력 (정상 동작 기준)

보드 부팅 후 시리얼에 다음 패턴이 나타나야 한다:

```
NanoUI Basic Button Demo        ← setup() 진입 확인
UI loaded. Touch the button!    ← JSON 파싱·렌더링 성공
```

버튼을 터치했을 때:
```
Button pressed!                 ← 콜백 실행 확인
```

## 실패 기준

- 아무 출력 없음 → `Serial.begin()` 미실행 또는 리셋 루프
- `Failed to load UI!` → JSON 파싱 실패 (메모리 부족 또는 JSON 오류)
- `Guru Meditation Error` / `abort()` → 런타임 크래시 (스택 오버플로 등)

---

## 명령어

```bash
PIO=~/.platformio/penv/bin/pio

# 시리얼 모니터 실행 (Ctrl+C 로 종료)
$PIO device monitor

# 포트 직접 지정
$PIO device monitor --port /dev/ttyUSB0 --baud 115200

# 업로드 + 즉시 모니터
$PIO run -t upload && $PIO device monitor
```

---

## 예상 출력 패턴 (예제별)

### basic_button
```
NanoUI Basic Button Demo
UI loaded. Touch the button!
```

### multi_screen
```
NanoUI Multi-Screen Demo
UI loaded. Navigate between screens!
```

### sensor_dashboard
```
NanoUI Sensor Dashboard
Dashboard ready!
```

---

## 에이전트 행동 규칙

1. 시리얼 출력을 최소 **3초** 기다린다 (ESP32 부팅 시간).
2. `Failed to load UI!` 출력 시 → JSON 또는 메모리 문제로 판단, 빌드 하네스로 돌아간다.
3. 크래시 발생 시 → `Guru Meditation Error` 이후 주소를 `esp32_exception_decoder` 결과와 함께 리포트.
4. 이 하네스는 **물리 보드 필요** — 출력 없으면 보드 상태를 먼저 확인한다.
