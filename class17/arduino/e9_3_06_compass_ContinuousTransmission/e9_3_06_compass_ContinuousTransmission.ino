#include <ZumoMotors.h>           // Zumo用モータ制御ライブラリの読み込み
#include <Pushbutton.h>           // Zumo用ユーザボタンライブラリの読み込み
#include <Wire.h>                 // I2C/TWI 通信デバイスの制御用ライブラリの読み込み
#include <LSM303.h>               // 加速度，地磁気センサ用ライブラリの読み込み

int cnt = 0;                      // 送信回数の確認用カウンタ

#define CRB_REG_M_2_5GAUSS 0x60   // CRB_REG_M の値 ： 地磁気センサーのスケールを +/-2.5 ガウスに設定
#define CRA_REG_M_220HZ    0x1C   // CRA_REG_M の値 ： 地磁気センサのアップデートレートを 220 Hz に設定

Pushbutton button(ZUMO_BUTTON);
LSM303 compass;

void setup()
{
  Serial.begin(9600);             // シリアル通信の初期化
  Wire.begin();                   // Wireライブラリの初期化と，I2Cバスとの接続
  
  compass.init();                 // LSM303 の初期化
  compass.enableDefault();        // 加速度センサ・地磁気センサ を利用可能にする
  compass.writeReg(LSM303::CRB_REG_M, CRB_REG_M_2_5GAUSS); // 地磁気センサーのスケールを +/-2.5 ガウスに設定
  compass.writeReg(LSM303::CRA_REG_M, CRA_REG_M_220HZ);    // 地磁気センサのアップデートレートを 220 Hz に設定

  button.waitForButton();         // ユーザボタンが押されるまで待つ
}

void loop()
{
  compass.read();                 // 地磁気センサの値を読む
  Serial.print(compass.m.x);      // 地磁気センサから得られた値(x)をシリアル送信
  Serial.print(",");              // 区切り文字（,）をシリアル送信
  Serial.print(compass.m.y);      // 地磁気センサから得られた値(y)をシリアル送信
  Serial.print(",");              // 区切り文字（,）をシリアル送信
  
  Serial.println(String(cnt));    // 送信回数カウンタをシリアル送信
  cnt += 1;                       // 送信回数カウンタをインクリメント
  delay(100);                     // 100ms 待つ
}
