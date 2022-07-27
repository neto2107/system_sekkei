//演習9.1.1
#include <ZumoMotors.h> //モータライブラリの読み込み

ZumoMotors motors;      //ZumoMotors クラスのインスタンス生成

void setup() {}

void loop() {
  //前進
  motors.setLeftSpeed(100);   //左モータの速度 100(-400〜400 に設定可。正なら前転)
  motors.setRightSpeed(100);  //右モータの速度 100(-400〜400 に設定可。正なら前転)
  delay(1000);                //1 秒間前進
  //停止
  motors.setLeftSpeed(0);     //左モータの速度 0(0 なら回転しない)
  motors.setRightSpeed(0);    //右モータの速度 0(0 なら回転しない)
  delay(1000);                //1秒間停止
  //後進
  motors.setLeftSpeed(-100);  //左モータの速度-100(負なら後転)
  motors.setRightSpeed(-100); //右モータの速度-100(負なら後転)
  delay(1000);                //1秒間後進
  //停止
  motors.setLeftSpeed(0);     //左モータの速度 0
  motors.setRightSpeed(0);    //右モータの速度 0
  delay(1000);                //1秒間停止
}
