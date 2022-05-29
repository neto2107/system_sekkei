#include <MsTimer2.h>
const int LED_PIN = 9; //LEDをD13に設定
volatile boolean st_switch = false; //ディジタル出力の値を格納する変数
unsigned int timer; //呼び出し回数を格納する変数(30*timer ms)

//初期設定
void setup(){
  pinMode(LED_PIN,OUTPUT); //LED_PINをデジタル出力設定

  //外部割り込みの設定
  attachInterrupt(0,blink,CHANGE);
  
  //タイマー割り込みの設定
  MsTimer2::set(30,flash);
  MsTimer2::start();
}

//メインループ
void loop(){
  //timerの値を0-255に変換してpwm出力
  analogWrite(LED_PIN,map(timer,0,100,0,255));
}

//割り込み時に呼ばれる関数
void blink(){
  st_switch = !st_switch; //st_switchを反転
}

//タイマー割り込み時に呼ばれる関数
void flash(){
  //スイッチの切り替えにより条件分岐
  if(st_switch ==false){ //st_switchが偽なら
    timer ++; //timerを増加させる
    if(timer >= 100){ //timerが100以上なら
      timer = 0;//タイマーを0にする
    }
  }else{ //st_switchが真なら
    timer --; //timerを減少させる
    if(timer <= 0) //timerが0以下なら
      timer = 100; //タイマーを100にする
  }
}