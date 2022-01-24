#include "myPIR.h"
const uint16_t LEDPIN = (10);
const uint16_t PIRPIN = (26);
const uint32_t ONE_SEC_MSEC = (1 * 1000);
const uint32_t FIVE_SEC_MSEC = (5 * 1000);
const uint32_t ONE_MIN_MSEC = (60 * ONE_SEC_MSEC);

//========================
// 初期化
//========================
void setup(void)
{
  Serial.begin( 115200 );
  pinMode(LEDPIN, OUTPUT);
  digitalWrite(LEDPIN, !LOW);
  setupPIR(PIRPIN, FIVE_SEC_MSEC);
}

//========================
// LOOP
//========================
void loop(void)
{
  int16_t val, cnt;
  val = getPIR();
  cnt = getAnyMinTrand(ONE_MIN_MSEC);
  digitalWrite(LEDPIN, !val);
  Serial.printf("val=%d cnt=%d\n", val, cnt);
  delay(1);
}
