//演習9.1.3
#include <ZumoMotors.h>          //モータライブラリの読み込み
#include <Pushbutton.h>          //ボタンライブラリの読み込み

const int trig = 2;              //TrigピンをArduinoの2番ピンに
const int echo = 4;              //EchoピンをArduinoの4番ピンに

ZumoMotors motors;               //ZumoMotorsクラスのインスタンス生成
Pushbutton button(ZUMO_BUTTON);  //Pushbuttonクラスのインスタンスを生成

//距離を計測
int distance() {
  unsigned long interval;        //Echoのパルス幅(μs)
  int dist;                      //距離(cm)
  
  digitalWrite(trig, HIGH);      //10μsのパルスを超音波センサのTrigピンに出力
  delayMicroseconds(10);
  digitalWrite(trig, LOW);

  interval = pulseIn(echo, HIGH, 23071);               //Echo信号がHIGHである時間(μs)を計測
  dist = (0.61 * 25 + 331.5) * interval / 10000 / 2;   //距離(cm)に変換

  delay(60);                     //trigがHIGHになる間隔を60ms以上空ける（超音波センサの仕様）

  return dist;
}

void setup() {
  button.waitForButton();        //ユーザボタンが押されるまで待機
  Serial.begin(9600);            //シリアル通信を9600bpsに初期化
  pinMode(trig, OUTPUT);         //trigを出力ポートに設定
  pinMode(echo, INPUT);          //echoを入力ポートに設定
}

void loop() {
  int dist;

  dist = distance();             //距離を計測
  Serial.println(dist);          //距離をシリアルモニタに出力
  
  if (0 < dist && dist < 10) {               //距離が10cm未満なら停止
    motors.setLeftSpeed(0);
    motors.setRightSpeed(0);
  } else {                       //距離が10cm以上なら前進
    motors.setLeftSpeed(100);
    motors.setRightSpeed(100);
  }
}
