//演習9.1.5
#include <ZumoMotors.h>          //モータライブラリの読み込み
#include <Pushbutton.h>          //ボタンライブラリの読み込み

ZumoMotors motors;               //ZumoMotorsクラスのインスタンス生成
Pushbutton button(ZUMO_BUTTON);  //Pushbuttonクラスのインスタンスを生成

void setup() {
  button.waitForButton();        //ユーザボタンが押されるまで待機
  Serial.begin(9600);
}
void loop() {
  //1秒間，時計回りに回転
  Serial.write(0);               //0を送信
  motors.setLeftSpeed(150);
  motors.setRightSpeed(-150);
  delay(3000);
  //1秒間，反時計回りに回転
  Serial.write(1);               //1を送信
  motors.setLeftSpeed(-150); 
  motors.setRightSpeed(150); 
  delay(3000); 
}
