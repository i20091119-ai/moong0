# UNO Q 버튼 2개 연습 앱 — 작업 지시서 (연수생용)

이 파일은 Claude Code가 읽는 작업 지시서. 사람이 읽어도 됨.
사람이 따라 하는 순서는 `GUIDE.md`, 진행 기록은 `STATUS.md`.

## 목표

- 버튼 2개(A, B) 누름 → UNO Q가 감지 → 노트북 브라우저 화면에 표시
- 앱 코드는 `button2/` 에 완성되어 있음. 보드에 올려 실행·확인하는 것이 목적

## 완료 기준

1. 버튼 A 누름 → 웹 화면 A 칸 깜빡임 + A 누른 횟수 +1
2. 버튼 B 동일
3. 길게 눌러도 1회만 셈(디바운스)
4. 웹 화면: 노트북 브라우저 `http://<보드이름>.local:7000` 또는 `http://<보드IP>:7000`
5. LED 매트릭스에 마지막 누른 버튼 글자(A/B) 표시

## 보드 정보 (연수생이 채움)

- 보드 이름: `<보드이름>` (예: `uno07`) — 같은 Wi-Fi의 다른 보드와 겹치면 안 됨
- 호스트: `<보드이름>.local`
- SSH 별칭: `uno` (`%USERPROFILE%\.ssh\config` 의 `HostName` 이 자기 보드를 가리켜야 함)
- 보드에서 실행 중인 다른 앱: `<없음 / 앱 이름>`

## 장비 연결

```
[윈도우 노트북] ── Wi-Fi ── [UNO Q]
                     │ USB-C
                  [허브] ── PD 충전기(40W)
```

- 노트북 ↔ UNO Q: Wi-Fi만. USB 직결 안 함(허브 경유 시 호스트끼리라 연결 불가)
- 허브 역할: 전원 공급만
- 같은 Wi-Fi 공유기에 연결. 기기 간 통신 차단(클라이언트 격리) 걸린 Wi-Fi는 `.local` 접속 실패

## 배선

| 버튼 | 한쪽 단자 | 다른 쪽 단자 |
|---|---|---|
| A | D2 | GND |
| B | D3 | GND |

- 핀 모드 `INPUT_PULLUP` — 안 누름 HIGH, 누름 LOW
- 외부 저항 불필요
- D0, D1 사용 금지(시리얼)
- 버튼 GND 2개 → 와고 → UNO Q GND 1가닥

## 보드 쪽 사실 (2026-10-19 강사 보드에서 확인)

- OS: Debian Linux (Qualcomm QRB2210), 사용자 `arduino`
- 앱 폴더: `~/ArduinoApps/<앱이름>/` → 앱 ID `user:<앱이름>`
- 앱 관리: `arduino-app-cli app list | start | stop | restart | logs`
- `start` / `stop` 인자는 `app_path` → `~/ArduinoApps/<앱이름>` 형식으로 동작 확인
- MCU(STM32U585) = 스케치(C++), MPU(Linux) = Python. 둘 사이 = Bridge
- 웹 화면 = WebUI 브릭, 포트 7000
- App Lab GUI에서도 실행 가능: 앱 카드 → Run, 아래 `Python` 탭에 로그

## 앱 폴더 구조

```
button2/
├── app.yaml            # 앱 이름, 사용 브릭 목록(web_ui)
├── sketch/
│   ├── sketch.ino      # 버튼 읽기 → Bridge로 Python에 알림, LED 매트릭스
│   └── sketch.yaml     # 보드·라이브러리 정보
├── python/
│   └── main.py         # Bridge 수신 → WebUI로 브라우저에 전달
└── assets/
    ├── index.html, app.js, style.css
    └── libs/           # arduino.js, socket.io.min.js (공식 예제 복사)
```

## 작업 순서 (Claude Code)

1. **접속·현황 확인**
   - `ssh uno "arduino-app-cli app list"` 로 실행 중인 앱 확인
   - 실행 중인 앱이 있으면 그 앱의 `sketch/sketch.ino` 에서 D2/D3 사용 여부 확인 → 사람에게 보고
2. **보드로 복사** — `scp -r button2 uno:~/ArduinoApps/`
3. **다른 앱 정지** — 사람에게 확인 받은 뒤 `ssh uno "arduino-app-cli app stop ~/ArduinoApps/<앱이름>"`
4. **실행** — `ssh uno "arduino-app-cli app start ~/ArduinoApps/button2"`. 첫 실행은 컴파일로 몇 분
5. **확인** — `ssh uno "arduino-app-cli app logs ~/ArduinoApps/button2"`. 사람에게 버튼 눌러 보라고 요청, 브라우저 결과 보고받기
6. **기록** — 결과를 `STATUS.md` 에 채움

## 금지·주의

- 보드 시스템 설정, 패키지 업데이트, 네트워크 설정 변경 금지
- `~/ArduinoApps/button2/` 밖의 파일 수정·삭제 금지
- 다른 앱 실행 중이면 멈추기 전에 사람에게 확인
- 부팅 시 자동 실행(DEFAULT) 앱 설정 변경 금지
- 다른 앱이 D2를 출력으로 쓰는 중에는 버튼 배선·누르기 금지 (핀 단락 위험)
- 접속 실패 시 직접 고치지 말고 사람에게 오류 메시지 보고
- 같은 오류로 3번 실패 시 중단하고 원인·시도 내용 보고
