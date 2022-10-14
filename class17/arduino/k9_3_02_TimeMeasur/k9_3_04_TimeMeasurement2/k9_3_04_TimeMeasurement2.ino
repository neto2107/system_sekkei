#include <ZumoMotors.h>           // Zumo用モータ制御ライブラリの読み込み
#include <Pushbutton.h>           // Zumo用ユーザボタンライブラリの読み込み
#include <Wire.h>                 // I2C/TWI 通信デバイスの制御用ライブラリの読み込み

unsigned long int timeStart = 0;  // 時間計測用変数
unsigned long int timeEnd = 0;    // 時間計測用変数
unsigned long int timeStart2 = 0;
unsigned long int timeEnd2 = 0;

int cnt = 0;                      // 送信回数の確認用カウンタ


Pushbutton button(ZUMO_BUTTON);

void setup()
{
  Serial.begin(9600);             // シリアル通信の初期化
  Wire.begin();                   // Wireライブラリの初期化と，I2Cバスとの接続

  button.waitForButton();         // ユーザボタンが押されるまで待つ
  Serial.println("START");        // 文字列をシリアル送信
}

void loop()
{
  button.waitForButton();         // ユーザボタンが押されるまで待つ

  timeStart = micros(); //現在時刻取得
  Serial.println(0); //printlnで送信
  timeEnd = micros(); //現在時刻取得
  timeStart2 = micros();
  Serial.write(0);
  timeEnd2 = micros();
  
  delay(50);                      // 150ms待つ
  Serial.print("println:");    // 経過時間をシリアル送信
  Serial.print(timeEnd-timeStart);
  Serial.print("write:");
  Serial.println(timeEnd2-timeStart2);

}
