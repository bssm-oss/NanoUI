# 하네스: 업로드 (Upload Harness)

## 역할

빌드된 펌웨어를 **LOLIN D32 보드에 플래싱**한다.
빌드 하네스가 SUCCESS를 반환한 이후에만 실행되어야 한다.

---

## 사전 조건

- [빌드 하네스](build.md) SUCCESS 상태
- LOLIN D32가 USB로 연결됨
- `/dev/ttyUSB*` 또는 `/dev/ttyACM*` 장치 노드 존재

## 출력 (성공 기준)

```
Writing at 0x... (100 %)
Hash of data verified.
```

esptool.py 가 `Hash of data verified` 출력 후 리셋.

## 실패 기준

- `Serial port ... not found` → 보드 미연결 또는 드라이버 문제
- `A fatal error occurred` → 플래시 쓰기 실패

---

## 명령어

```bash
PIO=~/.platformio/penv/bin/pio

# 업로드 (포트 자동 감지)
$PIO run -t upload

# 포트 직접 지정
$PIO run -t upload --upload-port /dev/ttyUSB0

# 업로드 후 즉시 모니터 (Ctrl+C 로 종료)
$PIO run -t upload && $PIO device monitor
```

---

## 핀 배치 (LOLIN D32 ↔ ILI9341)

| 신호 | LOLIN D32 GPIO | 비고 |
|------|---------------|------|
| TFT_CS | 10 | SPI CS |
| TFT_DC | 9 | Data/Command |
| TFT_RST | 8 | Reset |
| TOUCH_CS | 7 | 저항막 터치 |
| 터치 XP | 34 | ADC1_CH6 (기본값) |
| 터치 YP | 33 | ADC1_CH5 (기본값) |
| 터치 XM | 32 | ADC1_CH4 (기본값) |
| 터치 YM | 35 | ADC1_CH7 (기본값) |

기본값은 코드 내 `DefaultTouchHandler`에서 설정됨.
실제 배선이 다를 경우 `ui.setTouchHandler()`로 교체.

---

## 에이전트 행동 규칙

1. 업로드 전 **빌드 하네스 결과가 SUCCESS** 인지 확인한다.
2. 포트 인식 실패 시 `ls /dev/tty*` 로 장치 목록 확인 후 `--upload-port` 지정.
3. 이 하네스는 **물리 보드 필요** — 보드 없으면 건너뛰고 빌드 하네스만 돌린다.
4. 업로드 성공 여부를 모니터 하네스에 전달한다.
