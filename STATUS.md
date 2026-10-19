# 진행 상황

## 결과 (2026-10-19) — 완료

완료 기준 1~5 모두 보드에서 확인.

- [x] 버튼 A → A 칸 깜빡임 + 횟수 +1
- [x] 버튼 B 동일
- [x] 길게 눌러도 1회만 셈
- [x] 웹 화면 접속 (`http://172.16.249.155:7000`)
- [x] LED 매트릭스에 마지막 버튼 글자 표시

코드 수정 없이 첫 실행에서 동작.

## 보드에서 확인한 사실

- `arduino-app-cli` 앱 ID 형식: `user:<폴더이름>` (예: `user:morse-booth`, `user:button2`)
- `app start` / `app stop` 인자: `app_path` → `~/ArduinoApps/<폴더이름>` 사용
- App Lab GUI에서도 실행 가능: 앱 카드 열기 → Run. 아래 `Python` 탭에 로그
- 작업 중 App Lab이 보드 소프트웨어 업데이트함 (`arduino-app-cli` 0.13.0, `arduino:zephyr` 0.90.0 등). 업데이트 후에도 앱 정상 동작
- 보드 전원 연결 직후 접속까지 1~2분 걸림

### 웹 화면 주소

- `math.local:7000` → `ERR_CONNECTION_REFUSED` (앱 시작 전에 연 탭이었을 수 있음, 재확인 안 함)
- IP 주소 `172.16.249.155:7000` → 정상. IP는 공유기에 따라 바뀔 수 있음

### Morse Booth Bridge와 핀 충돌

| 핀 | Morse Booth Bridge | button2 |
|---|---|---|
| D2 | 부저 (OUTPUT) | 버튼 A |
| D3 | 아케이드 버튼 (INPUT_PULLUP) | 버튼 B |
| D4 | 버튼 LED (OUTPUT) | 안 씀 |

- 두 앱 동시 실행 불가. button2 실행 전 Morse 정지
- Morse 실행 중 D2를 GND에 연결하면 출력 핀 단락 위험
- DEFAULT는 Morse 그대로 → 재부팅하면 Morse가 실행됨. 재부팅 전 Morse 부품 원래대로 연결

### 아케이드 버튼 배선

- 스위치 단자 2개: 한쪽 D2(또는 D3), 다른 쪽 와고 → GND. 극성 없음
- 버튼 LED 다리는 연결 안 함

## 구조

```
버튼(D2/D3) → sketch.ino  Bridge.notify("button_pressed", "A")
            → main.py     Bridge.provide("button_pressed", ...) 횟수 +1
            → WebUI       ui.send_message("pressed", {counts, last})
            → app.js      칸 0.3초 노란색 + 숫자 갱신
초기화 버튼 → app.js ui.send_message("reset") → main.py on_reset → "counts" 전송
```

- 앱 전체가 보드에서 실행. 노트북은 화면 보기만 함 (노트북 꺼도 계속 셈)
- 횟수는 메모리 보관 → 앱 재시작·재부팅 시 0

## 참고한 공식 예제 (github.com/arduino/app-bricks-examples)

- `core-and-foundational/03-bridge-basics/02-send-data-to-python` — `Bridge.notify` / `Bridge.provide`
- `core-and-foundational/08-web-ui-basics/02-data-transmission` — `ui.send_message`, `assets/libs/` 구성
- `core-and-foundational/08-web-ui-basics/03-button-action` — 브라우저 → Python (`ui.on_message`)
- `core-and-foundational/02-led-matrix/01-led-matrix-frame-mcu` — `matrix.draw(frame)` 8x13
- `inspirational/common/weather-forecast` — 스케치에서 `String` 인자 Bridge 전달

## 연습 끝낸 뒤 원상 복구

1. App Lab에서 Button2 Stop
2. Morse 부품 원래대로 연결 (D2 부저, D3 아케이드 버튼, D4 버튼 LED)
3. 보드 재부팅 또는 `ssh uno "arduino-app-cli app start ~/ArduinoApps/morse-booth"`
