#include <MsTimer2.h>

// Switch on LED on pin 13 each second
//使用するピンの宣言
const int LED1_PIN = 13;
const int LED2_PIN = 9;
int timer=0; //割り込みの回数の値(max 125(2.5s))
void flash() {
  timer ++; //timer を1増やす
  //250 - timer の値を0-255に変換してpwm出力
  analogWrite(LED2_PIN,map(250 - timer,0,250,0,255));
  if(timer >= 250){//timerが250以上になったら、(5s経過したら)
    timer = 0; //timerを初期化
  }
  Serial.println(timer);//値確認用のシリアル出力
}

void setup() {
  //ピンモードの設定
  pinMode(LED1_PIN, OUTPUT);
  pinMode( LED2_PIN, OUTPUT);

  //シリアル通信を9600bpsで初期化
  Serial.begin(9600);

  MsTimer2::set(20, flash); // タイマーを20ミリ秒でセット
  MsTimer2::start();//タイマースタート
}

void loop() {
  digitalWrite(LED1_PIN,HIGH); //LED1のデジタル出力をHIGHに
  delay(500); //500ms待機
  digitalWrite(LED1_PIN,LOW);///LED1のデジタル出力をLOWに
  delay(500); //500ms待機
}
