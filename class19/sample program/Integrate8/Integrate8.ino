#include <Wire.h>
#include <ZumoMotors.h>
#include <Pushbutton.h>

ZumoMotors motors;
Pushbutton button(ZUMO_BUTTON);
//p制御によるライントレース
float red_G, green_G, blue_G; // カラーセンサで読み取ったRGB値（0-255）
int mode_G; // タスクのモードを表す状態変数
unsigned long timeInit_G, timeNow_G; //  スタート時間，経過時間
int motorR_G, motorL_G; // 左右のZumoのモータに与える回転力
int motor_mode_G;//進行方向を表す状態変数 0-左回り、1-右回り

void setup()
{
  Serial.begin(9600);
  Wire.begin();

  button.waitForButton(); // Zumo buttonが押されるまで待機
  CalibrationColorSensor(); // カラーセンサーのキャリブレーション

  button.waitForButton(); // Zumo buttonが押されるまで待機
  timeInit_G = millis();
  mode_G = 0;
  motor_mode_G = 0;
  motorR_G = 0;
  motorL_G = 0;
}

void loop()
{
  getRGB(red_G, green_G, blue_G); // カラーセンサでRGB値を取得(0-255)
  timeNow_G = millis() - timeInit_G; // 経過時間
  motors.setSpeeds(motorL_G, motorR_G); // 左右モーターへの回転力入力
  sendData(); // データ送信

  //linetrace_bang_bang(); // ライントレース（bang-bang制御）
 //linetrace_P(); // ライントレース（P制御）
   task_A(); // タスクA
  //何かあった時の復帰用
  if(button.isPressed()){
    mode_G = 0;
    motors.setSpeeds(0,0);
    delay(200);
    button.waitForButton();
  }
}

// 通信方式2
void sendData()
{
  static unsigned long timePrev = 0;
  static boolean flag_start = true; // 最初の通信かどうか
  int inByte; 

  // if文の条件： 最初の通信である || 最後のデータ送信から500ms経過 || (データ送信要求が来ている && 最後のデータ送信から50ms経過)
  if ( flag_start == true || timeNow_G - timePrev > 500 || 
     (Serial.available() > 0 && timeNow_G - timePrev > 50) ) { 
    flag_start = false;
    while(Serial.available() > 0){ // 送信要求が複数来ていた場合は全て読み込む
      inByte = Serial.read();
    }
    
    Serial.write('H');
    Serial.write(mode_G);
    Serial.write((int)red_G);
    Serial.write((int)green_G );
    Serial.write((int)blue_G);

    timePrev = timeNow_G;
  }
}
