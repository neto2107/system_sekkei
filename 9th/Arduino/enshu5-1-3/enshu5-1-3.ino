// 演習5.1.3 4秒ごとにduty比を変更
#include <MsTimer2.h>

const int D6 = 6;                       // PWM出力ディジタルピンの定義
float dutyH=0.25, dutyL=0.2, duty=duty; // duty変数の定義. duty比変更
boolean isHigh=true;
int iteration=0;
int Ts=40;                              // サンプリング周期 [msec]

void control() {                        // タイマ割り込み(MsTimer2)のISR
  iteration++;
  if (iteration>=100) {                 // 4 sec 毎
    iteration=0;
    if (isHigh) {
      isHigh=false;
      duty = dutyL;
    } else {
      isHigh=true;
      duty = dutyH;
    }
  }
  analogWrite(D6,duty*255);
}

void setup() {
  analogWrite(D6,0);
  MsTimer2::set(Ts, control);             // 40 msec 毎
  MsTimer2::start();
}

void loop() {}
