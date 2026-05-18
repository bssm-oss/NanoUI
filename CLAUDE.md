# NanoUI – Claude 하네스

JSON 기반 터치 UI 라이브러리 (LOLIN D32 + ILI9341 3.2인치).

## 하네스 루프

소스 수정 → 빌드 → 업로드 → 모니터 순서로 반드시 진행.

```
[수정] → [빌드 하네스] → [업로드 하네스] → [모니터 하네스]
              ↑ FAILED 시 되돌아옴
```

각 단계 상세: [docs/harness/](docs/harness/)

---

## 핵심 명령어

```bash
PIO=~/.platformio/penv/bin/pio

$PIO run                      # 빌드 (basic_button, 보드 불필요)
$PIO run -t upload            # 업로드 (보드 필요)
$PIO device monitor           # 시리얼 모니터 (Ctrl+C 종료)
$PIO run -t upload && $PIO device monitor   # 업로드 후 즉시 모니터
```

예제 전환: [platformio.ini](platformio.ini) 의 `src_dir` 값 변경.

---

## 라이브러리 구조 요약

자세한 모듈 계약: [docs/harness/library.md](docs/harness/library.md)

```
src/
  NanoUI.h/.cpp          공개 API (begin / loadFromFlash / show / update / on)
  components/            UIButton · UILabel · UIToggle · UISlider · UIImage
  core/                  UIParser · UIRenderer · UIScreen · TouchHandler
  drivers/               IDriver (인터페이스) · ILI9341Driver (구현)
examples/
  basic_button/          버튼 콜백 + millis 라벨
  multi_screen/          화면 전환 네비게이션
  sensor_dashboard/      슬라이더 + 토글 + 센서 값
```

---

## 보드 / 핀

| 신호 | GPIO | 비고 |
|------|------|------|
| TFT_CS | 10 | SPI CS |
| TFT_DC | 9 | D/C |
| TFT_RST | 8 | Reset |
| TOUCH_CS | 7 | 터치 CS |
| 터치 기본 핀 | 32-35 | `DefaultTouchHandler` — `setTouchHandler()`로 재정의 가능 |

---

## 수정 시 주의

- `UIEvent` 시그니처 변경 → 모든 예제 콜백 동시 수정
- 메모리 상수 조정 → `platformio.ini` `build_flags` 에서만
- `IDriver` 인터페이스 변경 → `ILI9341Driver` 구현체도 동기화
