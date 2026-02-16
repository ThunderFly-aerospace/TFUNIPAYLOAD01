/*
  TFUNIPAYLOAD01B
  ATmega1284P @ 8 MHz (external oscillator)

                     Mighty 1284p
                      +---\/---+
           (D 0) PB0 1|        |40 PA0 (AI 0 / D24)
           (D 1) PB1 2|        |39 PA1 (AI 1 / D25)
      INT2 (D 2) PB2 3|        |38 PA2 (AI 2 / D26)
       PWM (D 3) PB3 4|        |37 PA3 (AI 3 / D27)
    PWM/SS (D 4) PB4 5|        |36 PA4 (AI 4 / D28)
      MOSI (D 5) PB5 6|        |35 PA5 (AI 5 / D29)
  PWM/MISO (D 6) PB6 7|        |34 PA6 (AI 6 / D30)
   PWM/SCK (D 7) PB7 8|        |33 PA7 (AI 7 / D31)
                 RST 9|        |32 AREF
                VCC 10|        |31 GND
                GND 11|        |30 AVCC
              XTAL2 12|        |29 PC7 (D 23)
              XTAL1 13|        |28 PC6 (D 22)
      RX0 (D 8) PD0 14|        |27 PC5 (D 21) TDI
      TX0 (D 9) PD1 15|        |26 PC4 (D 20) TDO
RX1/INT0 (D 10) PD2 16|        |25 PC3 (D 19) TMS
TX1/INT1 (D 11) PD3 17|        |24 PC2 (D 18) TCK
     PWM (D 12) PD4 18|        |23 PC1 (D 17) SDA
     PWM (D 13) PD5 19|        |22 PC0 (D 16) SCL
     PWM (D 14) PD6 20|        |21 PD7 (D 15) PWM
                      +--------+
*/

#include <Arduino.h>

#if !defined(PIN_LED_RED) || !defined(PIN_LED_BLUE) || !defined(PIN_LED_GREEN) || !defined(PIN_BTN_USER_A) || !defined(PIN_BTN_USER_B)
#error "Board macros PIN_LED_* and PIN_BTN_USER_* must be provided by board configuration"
#endif

static bool phase = false;
static uint32_t counter = 0;

void setup()
{
  Serial.begin(115200);

  pinMode(PIN_LED_RED, OUTPUT);
  pinMode(PIN_LED_BLUE, OUTPUT);
  pinMode(PIN_LED_GREEN, OUTPUT);

  // Buttons have external pull-up resistors and are active in LOW state.
  pinMode(PIN_BTN_USER_A, INPUT);
  pinMode(PIN_BTN_USER_B, INPUT);

  Serial.println("TFUNIPAYLOAD01 hello-world");
}

void loop()
{
  const bool btnAIsPressed = digitalRead(PIN_BTN_USER_A) == LOW;
  const bool btnBIsPressed = digitalRead(PIN_BTN_USER_B) == LOW;

  uint16_t blinkDelayMs = 500;
  if (btnAIsPressed && btnBIsPressed) {
    blinkDelayMs = 80;
  } else if (btnAIsPressed) {
    blinkDelayMs = 150;
  } else if (btnBIsPressed) {
    blinkDelayMs = 1000;
  }

  digitalWrite(PIN_LED_GREEN, phase ? HIGH : LOW);
  digitalWrite(PIN_LED_BLUE, phase ? LOW : HIGH);
  digitalWrite(PIN_LED_RED, (btnAIsPressed || btnBIsPressed) ? HIGH : LOW);

  counter++;
  Serial.print("line=");
  Serial.print(counter);
  Serial.print(" uptime=");
  Serial.print(millis() / 1000UL);
  Serial.print("s btnA=");
  Serial.print(btnAIsPressed ? "pressed" : "released");
  Serial.print(" btnB=");
  Serial.print(btnBIsPressed ? "pressed" : "released");
  Serial.print(" blink=");
  Serial.print(blinkDelayMs);
  Serial.println("ms");

  phase = !phase;
  delay(blinkDelayMs);
}
