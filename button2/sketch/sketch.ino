// 버튼 2개(A=D2, B=D3, 반대쪽 GND) 눌림 순간만 Python에 "A"/"B" 전송
// INPUT_PULLUP: 안 누름 HIGH, 누름 LOW

#include <Arduino_RouterBridge.h>
#include <Arduino_LED_Matrix.h>

Arduino_LED_Matrix matrix;

const uint8_t PIN_A = 2;
const uint8_t PIN_B = 3;
const unsigned long DEBOUNCE_MS = 30;

struct Button {
  uint8_t pin;
  const char *name;
  int stable;            // 디바운스 끝난 상태
  int lastRead;          // 직전 읽은 값
  unsigned long changedAt;
};

Button buttons[] = {
  {PIN_A, "A", HIGH, HIGH, 0},
  {PIN_B, "B", HIGH, HIGH, 0},
};

// 8x13 매트릭스, 밝기 0~7
uint8_t frameA[104] = {
  0,0,0,0,0,7,7,7,0,0,0,0,0,
  0,0,0,0,7,7,0,7,7,0,0,0,0,
  0,0,0,7,7,0,0,0,7,7,0,0,0,
  0,0,0,7,7,0,0,0,7,7,0,0,0,
  0,0,0,7,7,7,7,7,7,7,0,0,0,
  0,0,0,7,7,0,0,0,7,7,0,0,0,
  0,0,0,7,7,0,0,0,7,7,0,0,0,
  0,0,0,7,7,0,0,0,7,7,0,0,0,
};
uint8_t frameB[104] = {
  0,0,0,7,7,7,7,7,7,0,0,0,0,
  0,0,0,7,7,0,0,0,7,7,0,0,0,
  0,0,0,7,7,0,0,0,7,7,0,0,0,
  0,0,0,7,7,7,7,7,7,0,0,0,0,
  0,0,0,7,7,0,0,0,7,7,0,0,0,
  0,0,0,7,7,0,0,0,7,7,0,0,0,
  0,0,0,7,7,0,0,0,7,7,0,0,0,
  0,0,0,7,7,7,7,7,7,0,0,0,0,
};

void setup() {
  for (Button &b : buttons) {
    pinMode(b.pin, INPUT_PULLUP);
  }
  matrix.begin();
  matrix.setGrayscaleBits(3);
  matrix.clear();
  Bridge.begin();
}

void loop() {
  unsigned long now = millis();
  for (Button &b : buttons) {
    int r = digitalRead(b.pin);
    if (r != b.lastRead) {
      b.lastRead = r;
      b.changedAt = now;
    }
    if (now - b.changedAt >= DEBOUNCE_MS && r != b.stable) {
      b.stable = r;
      if (b.stable == LOW) {  // HIGH -> LOW = 눌림 순간
        matrix.draw(b.pin == PIN_A ? frameA : frameB);
        Bridge.notify("button_pressed", String(b.name));
      }
    }
  }
}
