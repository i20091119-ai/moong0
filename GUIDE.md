# UNO Q 버튼 2개 연습 — 연수생 따라하기

버튼 2개(A, B)를 누르면 UNO Q가 감지해 노트북 브라우저에 표시하는 연습.
코드는 `button2/` 폴더에 완성되어 있음. 받아서 보드에 올리고 실행하는 과정을 체험.

- 소요 시간: 30~40분 (보드 업데이트가 뜨면 +10분)
- 2026-10-19 강사 보드에서 같은 순서로 동작 확인 (기록: `instructor/board-log-2026-10-19.md`)
- 진행하면서 `STATUS.md` 에 기록

---

## 0. 준비물

| 항목 | 비고 |
|---|---|
| Arduino UNO Q | 보드 이름 확인 (예: `uno07`). 연수생마다 달라야 함 → 0-1 |
| 허브 + PD 충전기(40W) + USB-C 케이블 | 보드 전원용 |
| 아케이드 버튼 2개 | A, B |
| 점퍼선 5가닥 + 와고 1개 | 버튼 → 보드 |
| Windows 노트북 | Arduino App Lab, git 설치 |
| Wi-Fi | 보드와 노트북이 같은 공유기. 기기 간 통신 차단 없는 것 |

---

## 0-1. 보드 이름 — 연수생마다 다르게 (중요)

여러 대가 같은 Wi-Fi에 있으면 `보드이름.local` 로 서로 찾음. 이름이 겹치면 꼬임.

- 같은 이름 2대 → 한 대가 `math-2.local` 식으로 자동 변경되거나, 다른 사람 보드에 접속·업로드
- App Lab 보드 목록에 같은 이름이 여러 개 떠서 구분 불가
- `ssh uno` 별칭도 자기 보드 이름을 가리켜야 함

| 할 일 | 예 |
|---|---|
| 보드마다 이름 다르게 | `uno01`, `uno02`, … `uno20` |
| 보드에 이름 스티커 | 보드·허브에 `uno07` |
| 연수생 노트북 SSH 별칭은 자기 보드로 | `HostName uno07.local` |

- 이름 변경은 연수 전에 강사가 일괄로 하는 것을 권장
- 이름 규칙: 영문 소문자·숫자·하이픈(`-`)만. 한글·공백·밑줄 금지

### 보드 이름 바꾸는 법

**방법 1 — App Lab**

1. App Lab 왼쪽 위 보드 이름 옆 `…` 누름
2. 이름 변경(또는 보드 설정) 메뉴에서 새 이름 입력
3. 보드 재시작 후 App Lab이 새 이름으로 다시 연결

메뉴 이름은 App Lab 버전에 따라 다를 수 있음.

**방법 2 — 명령어** (`…` 메뉴에 없을 때)

```powershell
ssh arduino@현재이름.local "sudo hostnamectl set-hostname uno07"
ssh arduino@현재이름.local "sudo reboot"
```

- `sudo` 비밀번호 = 보드 비밀번호
- 1~2분 뒤 `ssh arduino@uno07.local` 로 접속 확인

**바꾼 뒤 할 일**

- `%USERPROFILE%\.ssh\config` 의 `HostName` 을 새 이름으로
- 접속 주소: `http://새이름.local:7000`
- 보드·허브에 이름 스티커
- 보드 소프트웨어가 오래된 경우 이름 변경 후 `.local` 접속이 안 될 수 있음 → App Lab에서 보드 업데이트 먼저
- IP 주소는 공유기가 보드마다 다르게 주므로 겹치지 않음. 단 바뀔 수 있음
- 웹 화면 포트 7000은 보드마다 따로라 같은 번호여도 문제없음

---

## 1. 장비 연결

```
[UNO Q] ─USB-C─ [허브] ─ PD 충전기(40W) ─ 콘센트

[노트북] ─ Wi-Fi ─ [UNO Q]   (같은 공유기)
```

- 허브는 전원만 담당
- 노트북과 보드는 케이블로 연결하지 않음. 허브를 거쳐 노트북에 꽂으면 서로 통신 불가
- 전원 연결 후 접속까지 1~2분 걸림

---

## 2. 배선

| 선 | 한쪽 | 다른 쪽 |
|---|---|---|
| 1 | 버튼 A 스위치 단자 ① | **D2** |
| 2 | 버튼 A 스위치 단자 ② | 와고 |
| 3 | 버튼 B 스위치 단자 ① | **D3** |
| 4 | 버튼 B 스위치 단자 ② | 와고 |
| 5 | 와고 | UNO Q **GND** |

```
 D2 ──── [버튼 A] ──┐
                    ├── 와고 ──── GND
 D3 ──── [버튼 B] ──┘
```

- 저항 불필요 (보드 내부 풀업 사용)
- 아케이드 버튼: 가운데 검은 스위치의 은색 단자 2개 사용. 극성 없음
  - 단자가 3개면 `COM`, `NO` 사용, `NC` 비움
- 버튼 LED 다리(금색)는 연결 안 함
- D0, D1 사용 금지 (시리얼)
- GND는 D13 옆 또는 전원 핀 줄 아무 GND

---

## 3. 노트북 준비

### 3-1. git 확인

PowerShell:

```powershell
git --version
```

오류가 나면 https://git-scm.com 에서 설치 후 PowerShell 다시 열기.

### 3-2. 보드 SSH 접속 설정

`보드이름`은 자기 보드 이름으로 바꿈. 비밀번호는 보드 처음 설정할 때 정한 것.

```powershell
ssh arduino@보드이름.local
```

접속되면 `exit`로 나옴.

매번 길게 치지 않으려면 `%USERPROFILE%\.ssh\config` 파일에 별칭 추가:

```
Host uno
    HostName 보드이름.local
    User arduino
```

이후 `ssh uno` 로 접속. 아래 명령은 모두 `uno` 별칭 기준.

---

## 4. 코드 받기

**인터넷 되는 Wi-Fi**에서:

```powershell
cd $HOME\Documents
git clone <강사가 준 레포 주소>
cd <레포 이름>
dir button2
```

- 예: `git clone https://github.com/계정/uno-button-07.git` → `cd uno-button-07`
- 비공개 레포면 GitHub 로그인 창 → 로그인

- `app.yaml`, `assets`, `python`, `sketch` 4개 보이면 성공
- 레포 접근이 안 되면 강사에게 ZIP 받아서 `문서`에 압축 해제

---

## 5. 보드로 복사

보드와 같은 Wi-Fi로 돌아와서, 받은 레포 폴더에서:

```powershell
scp -r button2 uno:~/ArduinoApps/
```

```powershell
ssh uno "ls ~/ArduinoApps/button2"
```

- 4개 항목 보이면 성공
- App Lab 앱 목록에 **Button2** 카드가 생김

---

## 6. 실행 중인 다른 앱 정지

App Lab 화면 오른쪽 아래에 실행 중인 앱 이름 + 빨간 **Stop** 버튼이 있으면 누름.

- 스케치는 한 번에 하나만 실행됨. 다른 앱이 D2/D3를 쓰고 있으면 충돌
- 다른 앱이 D2를 출력으로 쓰는 중에 버튼을 누르면 핀 단락 위험 → **정지 후 배선**
- 부팅 시 자동 실행(DEFAULT) 설정은 바꾸지 않음

---

## 7. 실행

1. App Lab에서 **Button2** 카드 열기
2. 오른쪽 위 **Run** 누르기
3. 첫 실행은 스케치 컴파일로 몇 분 걸림
4. `Button2 is now running` 뜨면 완료

명령어로 하려면:

```powershell
ssh uno "arduino-app-cli app start ~/ArduinoApps/button2"
```

---

## 8. 확인

### 8-1. 브라우저 접속

```
http://보드이름.local:7000
```

안 열리면 IP로 접속. IP 확인:

```powershell
ssh uno "hostname -I"
```

첫 번째 숫자 사용 → `http://172.16.x.x:7000`

### 8-2. 체크리스트

- [ ] 화면에 "연결됨" (초록색)
- [ ] 버튼 A → A 칸 0.3초 노란색 + 숫자 +1
- [ ] 버튼 B → B 칸 동일
- [ ] 2~3초 길게 눌러도 숫자 1만 증가
- [ ] 보드 LED 매트릭스에 마지막 누른 글자(A/B)
- [ ] **초기화** → 숫자 0

### 8-3. 더 해볼 것

- 노트북 꺼도 보드는 계속 셈 → 다시 켜고 접속하면 그동안 누른 숫자 보임
- 같은 Wi-Fi 휴대폰으로 같은 주소 접속
- App Lab 아래 **Python** 탭에서 `button A -> 1` 로그 확인

---

## 9. 원상 복구

1. App Lab에서 Button2 **Stop**
2. 원래 앱 부품이 있었다면 원래 핀에 다시 연결
3. 보드 재부팅 → DEFAULT 앱 자동 실행

---

## 10. 문제 해결

실제 진행 중 겪은 것.

| 증상 | 원인 | 해결 |
|---|---|---|
| `Could not resolve host: github.com` | 노트북 인터넷 안 됨 (보드용 Wi-Fi에 인터넷 없음) | 인터넷 되는 Wi-Fi·핫스팟으로 잠깐 바꿔서 clone 후 복귀. `ping github.com`으로 확인 |
| `cd : 인수를 허용하는 위치 매개 변수를 찾을 수 없습니다` | 명령 2개를 한 줄에 붙여 입력 | 한 줄에 하나씩 |
| App Lab에 `Updates Available` → 설치 중 | 보드 소프트웨어 업데이트 | 끝날 때까지 전원·창 유지. 업데이트 후에도 앱 정상 동작 |
| `ssh uno` 연결 안 됨 | 보드 부팅 중 | 1~2분 기다린 후 재시도 |
| `보드이름.local:7000` → `ERR_CONNECTION_REFUSED` | 앱 시작 전에 연 탭 또는 `.local` 이름 해석 문제 | 새로고침. 안 되면 IP 주소로 접속 |
| 버튼 눌러도 반응 없음 | 배선 | 스위치 단자 2개가 D2(D3)와 GND에 각각 연결됐는지. Python 탭에 로그 찍히는지 확인 |
| 안 눌렀는데 계속 눌린 상태 | 같은 쪽 단자 2개 사용 (4다리 스위치) | 대각선 단자로 변경 |

---

## 11. 동작 구조

```
[버튼 D2/D3]
    │  INPUT_PULLUP: 안 누름 HIGH, 누름 LOW
    ▼
sketch/sketch.ino  (MCU, C++)
    │  눌림 순간(HIGH→LOW)만, 30ms 디바운스
    │  Bridge.notify("button_pressed", "A")
    │  LED 매트릭스에 A/B 표시
    ▼
python/main.py  (Linux, Python)
    │  Bridge.provide("button_pressed", ...) → 횟수 +1
    │  ui.send_message("pressed", {counts, last})
    ▼
assets/app.js  (브라우저)
       칸 0.3초 노란색 + 숫자 갱신
       초기화 버튼 → ui.send_message("reset") → Python이 0으로
```

| 파일 | 역할 |
|---|---|
| `app.yaml` | 앱 이름, 사용 브릭(`arduino:web_ui`) |
| `sketch/sketch.ino` | 버튼 읽기, 디바운스, Bridge 전송, LED 매트릭스 |
| `sketch/sketch.yaml` | 보드 플랫폼(`arduino:zephyr`) |
| `python/main.py` | 횟수 세기, 웹 화면으로 전달 |
| `assets/index.html`, `app.js`, `style.css` | 웹 화면 |
| `assets/libs/` | `socket.io.min.js`, `arduino.js` (공식 예제 복사, 인터넷 없이 동작) |

- 앱 전체가 보드 안에서 실행. 노트북은 화면 보기만
- 횟수는 메모리 보관 → 앱 재시작·재부팅 시 0
- 웹 화면 포트 7000 = WebUI 브릭 기본값

---

## 12. Claude Code로 진행하려면

노트북에서 받은 레포 폴더를 Claude Code로 열면 `CLAUDE.md`(작업 지시서)를 읽고 같은 순서로 진행.

- 클라우드(웹) Claude Code는 보드에 접속 못 함 → 노트북에서 실행되는 Claude Code 사용
- 예: "button2 앱 보드에서 실행해줘. 실행 중인 다른 앱은 멈춰도 됨."

---

## 참고

- 공식 예제: https://github.com/arduino/app-bricks-examples
  - `core-and-foundational/03-bridge-basics/02-send-data-to-python`
  - `core-and-foundational/08-web-ui-basics/02-data-transmission`
  - `core-and-foundational/08-web-ui-basics/03-button-action`
  - `core-and-foundational/02-led-matrix/01-led-matrix-frame-mcu`
- 강사 보드 기록: `instructor/board-log-2026-10-19.md`
