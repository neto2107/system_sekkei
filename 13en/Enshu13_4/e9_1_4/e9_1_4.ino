//演習9.1.4
#include <ZumoMotors.h>          //モータライブラリの読み込み
#include <Pushbutton.h>          //ボタンライブラリの読み込み

const int trig = 2;              //TrigピンをArduinoの2番ピンに
const int echo = 4;              //EchoピンをArduinoの4番ピンに
unsigned long start_time;        //時間計測の開始時間
int mode;                        //0は前進，1は後進

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
  mode = 0;                      //前進から始める
  start_time = millis();         //プログラムがスタートしてからの経過時間を保存
}

void loop() {
  int dist;

  dist = distance();                //距離を計測
  Serial.println(dist);             //距離をシリアルモニタに出力
  
  //3秒間前進
  if (mode == 0) {
    if (millis() - start_time < 3000) { //時間計測開始から3秒以内なら
      motors.setLeftSpeed(100);         //1ms前進
      motors.setRightSpeed(100);
      delay(1);
    } else {
      mode = 1;                         //3秒以上なら後進
      start_time = millis();            //計測開始時間を現在時間に
    }
  }

  //3秒間後進
  if (mode == 1) {
    if (millis() - start_time < 3000) { //時間計測開始から3秒以内なら
      motors.setLeftSpeed(-100);        //1ms後進
      motors.setRightSpeed(-100);
      delay(1);
    } else {
      mode = 0;                         //3秒以上なら前進
      start_time = millis();            //計測開始時間を現在時間に
    }
  }
}
