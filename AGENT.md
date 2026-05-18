# NanoUI – Agent 하네스

이 파일은 Claude 외 모든 AI 에이전트(Codex, Gemini, GPT 등)가
NanoUI 프로젝트에서 작업할 때 참조하는 공통 하네스 문서다.

---

## 프로젝트 한 줄 요약

**NanoUI**: ESP32(LOLIN D32) + ILI9341 터치 디스플레이용 JSON 기반 UI 라이브러리.
사용자는 `.ui.json` 파일 하나로 화면을 선언하고, 라이브러리가 렌더링 + 터치 처리를 담당한다.

---

## 역할별 하네스 (분리된 책임)

| 역할 | 문서 | 보드 필요 |
|------|------|-----------|
| 빌드 검증 | [docs/harness/build.md](docs/harness/build.md) | ❌ |
| 보드 업로드 | [docs/harness/upload.md](docs/harness/upload.md) | ✅ |
| 런타임 모니터링 | [docs/harness/monitor.md](docs/harness/monitor.md) | ✅ |
| 라이브러리 구조 이해 | [docs/harness/library.md](docs/harness/library.md) | ❌ |

에이전트는 **맡은 역할의 하네스 문서만** 읽으면 된다.
소스를 전부 읽지 않아도 각 하네스가 입력·출력·실패 기준을 명시한다.

---

## 하네스 파이프라인

```
[코드 에이전트]  →  [빌드 하네스]  →  [업로드 하네스]  →  [모니터 에이전트]
  소스 수정           pio run            pio run -t upload    pio device monitor
  (소스 불필요)       SUCCESS?           보드 연결 확인        출력 패턴 확인
```

실패 시 해당 단계에서 멈추고 이전 에이전트에게 에러를 전달한다.

---

## 도구 경로

```bash
PIO=~/.platformio/penv/bin/pio
```

모든 PlatformIO 명령은 이 경로의 바이너리를 사용한다 (`pio` 가 PATH에 없을 수 있음).

---

## 예제 전환 방법

1. [platformio.ini](platformio.ini) 의 `src_dir` 줄 수정:
   ```ini
   src_dir = examples/multi_screen   ; basic_button / multi_screen / sensor_dashboard
   ```
2. `pio run` 재실행

또는 각 예제 디렉터리에서 독립적으로 빌드:
```bash
cd examples/multi_screen && ~/.platformio/penv/bin/pio run -t upload
```

---

## 핵심 제약

- `MAX_SCREENS`, `MAX_COMPONENTS_PER_SCREEN` 등 메모리 상수는 소스가 아닌 `platformio.ini` `build_flags`에서 조정
- 터치 핀은 하드코딩되어 있으나 `ui.setTouchHandler()`로 재정의 가능
- `UIEvent.id` 는 `MAX_ID_LEN=16` 바이트 제한 (널 종단 포함)
- ESP32에서 `A0-A3` 미정의 → 기본 터치 핀은 GPIO 32~35 사용
