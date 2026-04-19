#include <SPI.h>

#define T_CLK  13
#define T_DIN  15
#define T_DO   27
#define T_CS   14

#define CMD_X  0x90
#define CMD_Y  0xD0

void setup() {
  Serial.begin(115200);
  delay(100);
  Serial.println("=== XPT2046 触控测试 ===");

  pinMode(T_CS, OUTPUT);
  pinMode(T_CLK, OUTPUT);
  pinMode(T_DIN, OUTPUT);
  pinMode(T_DO, INPUT);
  digitalWrite(T_CS, HIGH);
}

uint16_t readTouch(uint8_t cmd) {
  digitalWrite(T_CS, LOW);
  uint16_t data = 0;

  for (int i = 0; i < 8; i++) {
    digitalWrite(T_CLK, LOW);
    digitalWrite(T_DIN, (cmd & 0x80) ? HIGH : LOW);
    cmd <<= 1;
    digitalWrite(T_CLK, HIGH);
  }

  for (int i = 0; i < 12; i++) {
    digitalWrite(T_CLK, LOW);
    data <<= 1;
    digitalWrite(T_CLK, HIGH);
    if (digitalRead(T_DO)) data |= 1;
  }

  digitalWrite(T_CS, HIGH);
  return data >> 4;
}

void loop() {
  uint16_t x = readTouch(CMD_X);
  uint16_t y = readTouch(CMD_Y);

  if (x > 0 && y > 0 && x < 4095 && y < 4095) {
    Serial.print("X: "); Serial.print(x);
    Serial.print(" | Y: "); Serial.println(y);
  }

  delay(50);
}
