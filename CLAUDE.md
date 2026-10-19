# UNO Q 버튼 2개 연습 앱 — 인수인계

이 파일은 Claude Code가 읽는 작업 지시서. 사람이 읽어도 됨.
진행 상황은 `STATUS.md` 참고.

## 목표

- 버튼 2개(A, B) 누름 → UNO Q가 감지 → 노트북 브라우저 화면에 표시
- 연수 2일차(설치형 교구) 진행 전 연습. 구조 확인이 목적, 기능 최소

## 완료 기준

1. 버튼 A 누름 → 웹 화면 A 칸 깜빡임 + A 누른 횟수 +1
2. 버튼 B 동일
3. 길게 눌러도 1회만 셈(디바운스)
4. 웹 화면: 노트북 브라우저 `http://math.local:7000` (포트는 예제·브릭 설정 따라 다를 수 있음)
5. LED 매트릭스에 마지막 누른 버튼 글자(A/B) 표시 — 선택, 시간 남으면

## 장비 연결

```
[윈도우 노트북] ── Wi-Fi ── [UNO Q]
                     │ USB-C
                  [핸시 허브] ── PD 충전기(40W)
```

- 노트북 ↔ UNO Q: Wi-Fi만. USB 직결 안 함(허브 경유 시 호스트끼리라 연결 불가)
- 허브 역할: 전원 공급만. 프린터는 이번 연습에서 안 씀
- 같은 Wi-Fi 공유기에 연결. 기기 간 통신 차단(클라이언트 격리) 걸린 Wi-Fi는 `.local` 접속 실패

## 배선

| 버튼 | 한쪽 단자 | 다른 쪽 단자 |
|---|---|---|
| A | D2 | GND |
| B | D3 | GND |

- 핀 모드 `INPUT_PULLUP` — 안 누름 HIGH, 누름 LOW
- 외부 저항 불필요
- D0, D1 사용 금지(시리얼)
- 버튼 GND 2개 → 와고 → UNO Q GND 1가닥 (연수 세트와 같은 방식)

## 현재 상태 (2026-10-19 확인)

- 보드 이름: `math` (Arduino UNO Q), 호스트 `math.local`
- App Lab 0.10.0 (Windows 노트북에 설치)
- `ssh uno` 비밀번호 없이 접속 확인 완료
- 보드에 기존 앱 3개: **Morse Booth Bridge** (DEFAULT, 실행 중)
  - 내용: 아케이드 버튼·부저·LED 모스 브리지, Python MPU ↔ STM32 MCU, 웹앱 localhost:8080 연동
  - 버튼 입력 → 스케치 → Python 구조가 이번 앱과 같음 → **1순위 참고 예제**
  - 버튼 핀을 점유 중일 수 있음 → 새 앱 실행 전 정지 필요. 정지 전 사람에게 확인
  - DEFAULT(부팅 시 자동 실행) 설정은 변경 금지

## 보드 접속

- 노트북: Windows, PowerShell 내장 OpenSSH 사용
- 별칭 설정 파일: `%USERPROFILE%\.ssh\config`
- 이후 `ssh uno "명령"` 형태로 실행
- 접속 실패 시 직접 고치지 말고 사람에게 오류 메시지 보고

## 보드 쪽 사실

- OS: Debian Linux (Qualcomm QRB2210), 사용자 `arduino`
- 앱 폴더: `~/ArduinoApps/<앱이름>/`
- 앱 관리: `arduino-app-cli` (`app list`, `app start`, `app stop`, 로그 확인)
- MCU(STM32U585) = 스케치(C++), MPU(Linux) = Python. 둘 사이 = Bridge
- 웹 화면 = WebUI 브릭, 포트 7000

## 앱 폴더 구조

```
~/ArduinoApps/button2/
├── app.yaml            # 앱 이름, 사용 브릭 목록(web_ui)
├── sketch/
│   ├── sketch.ino      # 버튼 읽기 → Bridge로 Python에 알림
│   └── sketch.yaml     # 보드·라이브러리 정보
├── python/
│   └── main.py         # Bridge 수신 → WebUI로 브라우저에 전달
└── assets/
    ├── index.html      # 브라우저 화면
    ├── app.js, style.css
    └── libs/           # arduino.js, socket.io.min.js (공식 예제 복사)
```

## 작업 순서 (Claude Code)

1. **예제부터 확인 — 추측 금지**
   - `ssh uno "arduino-app-cli --help"`, `ssh uno "arduino-app-cli app --help"` 로 실제 명령·인자 형식 확인
   - `ssh uno "arduino-app-cli app list"` 로 기존 앱·예제 확인
   - Morse Booth Bridge 앱 폴더 위치 확인 후 전체 파일 읽기 (구조·Bridge 호출·웹 연동 방식 파악)
   - 웹 화면 방식이 WebUI 브릭이 아니면, Bridge + WebUI 쓰는 공식 예제 1개도 골라 전체 읽기
2. **로컬에서 작성** — `button2/` (작성 완료, 보드 미검증)
3. **보드로 복사** — `scp -r button2 uno:~/ArduinoApps/`
4. **실행** — 1단계에서 확인한 형식으로 `app start`. 첫 실행은 스케치 컴파일 때문에 시간 걸림
5. **확인** — 로그 확인 → 오류 있으면 수정 → 3~4 반복. 사람에게 버튼 눌러 보라고 요청, 브라우저 결과 보고받기

## 스케치 요구사항

- 버튼 2개 `INPUT_PULLUP`
- 눌림 순간(HIGH→LOW)만 1회 전송
- 디바운스 30ms 내외, `delay()` 대신 `millis()` 사용
- 전송 값: `"A"` 또는 `"B"`

## 웹 화면 요구사항

- A, B 큰 칸 2개, 누른 횟수 숫자
- 누르면 0.3초 색 변화
- 초기화 버튼 1개 (횟수 0)
- 외부 CDN 의존 최소화 — 예제가 쓰는 방식(socket 연결 등) 그대로

## 금지·주의

- 보드 시스템 설정, 패키지 업데이트, 네트워크 설정 변경 금지
- `~/ArduinoApps/button2/` 밖의 파일 수정·삭제 금지
- 다른 앱 실행 중이면 멈추기 전에 사람에게 확인
- 같은 오류로 3번 실패 시 중단하고 원인·시도 내용 보고

## 다음 단계 (이번 범위 아님)

- 버튼 4개 + 조이스틱(D2~D9) 확장 → 오픈프레임 세트와 동일 배선
- 감열 프린터(세우 SLK-TS100, 허브 USB) 출력 — 한글은 이미지로 변환 후 ESC/POS 래스터 출력
- 연수용 CLAUDE.md 템플릿으로 정리
