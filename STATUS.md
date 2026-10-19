# 진행 상황 (2026-10-19, 클라우드 세션)

## 한 일

- `button2/` 앱 파일 작성 완료 (보드 미검증)
- 참고한 공식 예제 (github.com/arduino/app-bricks-examples)
  - `core-and-foundational/03-bridge-basics/02-send-data-to-python` — `Bridge.notify` / `Bridge.provide`
  - `core-and-foundational/08-web-ui-basics/02-data-transmission` — `ui.send_message`, `assets/libs/` 구성
  - `core-and-foundational/08-web-ui-basics/03-button-action` — 브라우저 → Python (`ui.on_message`)
  - `core-and-foundational/02-led-matrix/01-led-matrix-frame-mcu` — `matrix.draw(frame)` 8x13
  - `inspirational/common/weather-forecast` — 스케치에서 `String` 인자 Bridge 전달
- `assets/libs/arduino.js`, `socket.io.min.js` 예제에서 그대로 복사 (외부 CDN 없음)

## 못 한 일 (이 세션은 클라우드라 `ssh uno` 불가)

- `arduino-app-cli --help` 확인
- Morse Booth Bridge 앱 파일 읽기 / 비교
- 보드 복사·실행·로그 확인

## 노트북에서 이어서 할 일

1. `ssh uno "arduino-app-cli app --help"` 로 명령 형식 확인
2. Morse Booth Bridge 앱의 `app.yaml`, `sketch.yaml` 과 비교. 다르면 그쪽 형식 따름
3. Morse Booth Bridge가 D2/D3 쓰는지 확인 → 정지 여부 사람에게 확인 (DEFAULT 설정은 그대로)
4. `scp -r button2 uno:~/ArduinoApps/`
5. `app start` (첫 실행 컴파일 시간 걸림) → 로그 확인
6. 브라우저 `http://math.local:7000` → 버튼 A/B 눌러 확인

## 동작 구조

```
버튼(D2/D3) → sketch.ino  Bridge.notify("button_pressed", "A")
            → main.py     Bridge.provide("button_pressed", ...) 횟수 +1
            → WebUI       ui.send_message("pressed", {counts, last})
            → app.js      칸 0.3초 노란색 + 숫자 갱신
초기화 버튼 → app.js ui.send_message("reset") → main.py on_reset → "counts" 전송
```

## 확인 포인트 (오류 시 먼저 볼 곳)

- 스케치 컴파일 오류 `String` 인자 → `Bridge.notify("button_pressed", b.name)` 또는 정수(0/1)로 변경
- 매트릭스 관련 컴파일 오류 → `matrix` 관련 줄 삭제 (선택 기능)
- 브라우저 연결 안 됨 → Wi-Fi 클라이언트 격리, 포트 확인
