//演習9.1.2
#include <ZumoMotors.h>             //モータライブラリの読み込み
#include <Pushbutton.h>             //ボタンライブラリの読み込み

const int buzzerPin = 3;            //ブザーは3番ピン

ZumoMotors motors;                  //ZumoMotorsクラスのインスタンス生成
Pushbutton button(ZUMO_BUTTON);     //Pushbuttonクラスのインスタンスを生成

void setup() {
  button.waitForButton();           //ユーザボタンが押されるまで待機
  tone(buzzerPin, 440);             //ブザーを440Hzで鳴らす 
  delay(500);                       //500ms鳴らす 
  noTone(buzzerPin);                //ブザーを止める
}

void loop() {
  if (button.isPressed() == true) { //ユーザボタンが押されている間は後転
    motors.setLeftSpeed(-100);
    motors.setRightSpeed(-100);
  } else {                          //ユーザボタンが押されていない間は前転
    motors.setLeftSpeed(100);
    motors.setRightSpeed(100);
  } 
}
