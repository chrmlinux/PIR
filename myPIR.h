//===============================================================> myPIR.h
//  HC-SR501
//  Author/date   : @chrmLinux 2021/09/29
//  Author/update : @chrmLinux 2021/09/29
//  https://datasheetspdf.com/pdf-file/775434/ETC/HC-SR501/1
//
//  +--------------------------+
//  |          . . .           |
//  |          G O V           |
//  |          N U C           |
//  |          D T C           |
//  || Repeat                  |
//  |. Single ==   ==          |
//  +--------------------------+
//    Sensitivity  TimeDelay
//             VR         VR
//    min <-> max  min <-> max
//
// 1)１分くらい待たないと安定しない
// 2)OFF直後に BLOCK TIME がある
//
//         +---+............
//         |   | BLOCK     :
//         |   |     TIME  :
// --------+   +----------------------------
// Status ON   OFF
//
//
// 3)ぢゃ、仮想的にONするって感じで
//
//         +---+............
//         |   | VirtualON :
//         |   |      TIME :
// --------+   +----------------------------
// Status ON               OFF
//
//

const uint32_t BLOCK_TM = (5 * 1000);
static uint16_t _pirPin = 0;
static uint32_t _blockTime = 0L;

//========================
// BLOCKING TIME秒数 待つ
//========================
bool waitBlockTime()
{
  static uint32_t tm = millis();
  bool rtn = false;
  if ((millis() - tm) > _blockTime) {
    tm = millis();
    rtn = true;
//    Serial.printf("[wait]\n");
  }
  return rtn;
}

//========================
// PIR初期化
//========================
void setupPIR(uint16_t pin, uint32_t blocktime)
{
  _pirPin = pin;
  _blockTime = (blocktime < BLOCK_TM) ? BLOCK_TM : blocktime;
  pinMode(_pirPin, INPUT_PULLUP);
}

//========================
// 設定したBlockTImeを得る
//========================
uint32_t getBlockTime(void)
{
  return _blockTime;
}

//========================
// PIRから実際に値を得る
//========================
int16_t getRealPIR(void)
{
  int16_t val = digitalRead(_pirPin);
  return val;
}

//========================
// PIRから仮想の値を得る
//========================
int16_t getPIR(void)
{
  static int16_t preVal = -1;

  if (preVal == HIGH) {
    if (!waitBlockTime()) {
      return HIGH;
    }
  }

  int16_t val = getRealPIR();
  if (preVal != val) {
//    Serial.printf("[getPIR] preVal:%d val:%d\n", preVal, val);
    preVal = val;
  }
  return val;
}

//========================
// センサ検知からn分間の傾向
//========================
uint32_t getAnyMinTrand(uint32_t any_sec_msec)
{
  static uint32_t tm = millis();
  static uint32_t cnt = 0L;
  static int16_t preVal = 0;
  int16_t val;

  if ((millis() - tm) > any_sec_msec) {
    tm = millis();
    cnt = 0L;
  }

  val = getPIR();
  if (preVal != val) {
    tm = millis();
    preVal = val;
    if (val == HIGH) {
      cnt++;
    }
  }

  return cnt;
}
//<=============================================================== myPIR.h
