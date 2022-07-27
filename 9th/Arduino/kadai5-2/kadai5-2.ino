// list 5.1 ロータリエンコーダによる回転速度の計測
#include <MsTimer2.h>

const int D2 = 2;
const int D6 = 6;    // PWM出力ディジタルピンの定義
float dutyH = 0.25;    // duty変数の定義. duty比変更
float dutyL = 0.2;
float duty = dutyL;
boolean isHigh=false;
int Ts = 40;  // [msec]
int iteration=0;
unsigned long timep=0;
volatile float motrpm = 0;
volatile float motrpmf4 = 0; //a=4
volatile float motrpmf8 = 0; //a=8
volatile float motrpmf2 = 0; //a=2
volatile boolean isD2High=false;

void countUp() {
  if ((isD2High)&&(digitalRead(D2)==LOW)) {
    unsigned long timen = millis();
    motrpm = 1000.0*60.0/(float)(timen-timep);
    motrpmf2 = (1.0*motrpmf2+motrpm)/2.0;
    motrpmf4 = (3.0*motrpmf4+motrpm)/4.0;
    motrpmf8 = (7.0*motrpmf8+motrpm)/8.0;
    timep=timen;
    isD2High=false;
  } else if ((!isD2High)&&(digitalRead(D2)==HIGH)) {
    isD2High=true;
  }
}

void control() {
  iteration++;
  if (iteration>=100) {
    iteration=0;
    if (isHigh) {
      isHigh=false;
      duty = dutyL;
    } else {
      isHigh=true;
      duty =  dutyH;
    }
  }
  analogWrite(D6,duty*255);
  //motrpmを送信のためunsigned intにキャスト
  unsigned int x = (unsigned int)motrpm;
  unsigned int y = (unsigned int)motrpmf2;
  unsigned int z = (unsigned int)motrpmf4;
  unsigned int l = (unsigned int)motrpmf8;
  //デューティ比を100倍してunsigned intにキャスト
  //データの先頭を示す識別子を送信
  Serial.write('H');
  //motrpmの値を送信
  Serial.write(x>>8);
  Serial.write(x&255);
  //motrpm2の値を送信
  Serial.write(y>>8);
  Serial.write(y&255);
    //motrpmf4の値を送信
  Serial.write(z>>8);
  Serial.write(z&255);
  //motrpmf8の値を送信
  Serial.write(l>>8);
  Serial.write(l&255);
  
}

void setup() {
  analogWrite(D6,dutyH*255);
  attachInterrupt(0,countUp,CHANGE);
  MsTimer2::set(Ts, control); // 40ms period
  MsTimer2::start();  
  Serial.begin(115200);
}

void loop() {}
