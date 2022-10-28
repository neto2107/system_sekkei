
#include <Wire.h>
#include <ZumoMotors.h>
#include <Pushbutton.h>
#include <LSM303.h>

ZumoMotors motors; 
Pushbutton button(ZUMO_BUTTON);
LSM303 compass;

unsigned long timeInit_G, timeNow_G, timePrev_G; //  スタート時間，経過時間, 1回前
int motorR_G, motorL_G;  // 左右のZumoのモータに与える回転力
int mode_G=0; 
float mx=0, my=0, mz=0; //地磁気センサーの状態を格納する変数
float ax=0, ay=0, az=0;//加速度センサーの状態を格納する変数

void setup()
{
  //初期化
  Serial.begin(9600);
  Wire.begin();
  setupCompass();
  //キャリブレーション用にリセット
  compass.m_min.x = 32767;  compass.m_min.y = 32767;  compass.m_min.z = 32767;
  compass.m_max.x = -32768;  compass.m_max.y = -32768;  compass.m_max.z = -32768;
// 演習7-2.5.2ではつぎの3行のコメントを外せばよい
 button.waitForButton();  //ボタンが押されるまで待機
  calibrationCompass(); //2次元のキャリブレーションを実行
  button.waitForButton(); //ボタンが押されるまで待機
  //変数の初期化
  mode_G = 0;
  timeInit_G = millis();
  timePrev_G=0;
}

void loop()
{
  timeNow_G = millis() - timeInit_G; // 経過時間
  //経過時間が100ms以下だったらそのまま修了(100ms間隔でセンシングを行う。)
  if (timeNow_G-timePrev_G<100) {
    return;
  }
  //コンパスと加速度センサーの値を取得
  compass.read();
  //取得した値と、max,minの値を比較してそれぞれの最小値、最大値をmin,maxに格納する。
  compass.m_min.x = min(compass.m.x,compass.m_min.x);  compass.m_max.x = max(compass.m.x,compass.m_max.x);
  compass.m_min.y = min(compass.m.y,compass.m_min.y);  compass.m_max.y = max(compass.m.y,compass.m_max.y);
  compass.m_min.z = min(compass.m.z,compass.m_min.z);  compass.m_max.z = max(compass.m.z,compass.m_max.z);
  //0-1の値に変換
  ax = compass.a.x/256; //map(compass.a.x,-32768,32767,-128,127);
  ay = compass.a.y/256; //map(compass.a.y,-32768,32767,-128,127);
  az = compass.a.z/256; //map(compass.a.z,-32768,32767,-128,127);
  //-128-127の値に変換
  mx = map(compass.m.x,compass.m_min.x,compass.m_max.x,-128,127);
  my = map(compass.m.y,compass.m_min.y,compass.m_max.y,-128,127);
  mz = map(compass.m.z,compass.m_min.z,compass.m_max.z,-128,127); 
  sendData(); // データ送信
  //タイマーリセット
  timePrev_G=timeNow_G;
}

//1バイト分のintがたのデータ送信
void write1byteint(int x) {
  Serial.write(x+128); //少数は整数に直す。
}
//2バイト分のint型のデータ送信
void write2byte(int x) {
  Serial.write(x>>8); //下位8ビットをシフトし、上位８ビットのみ送信する
  Serial.write(x&255);//下位8ビットのみ送信する(0000000011111111でAND演算)
}
//　送信処理
void sendData()
{
    Serial.write('H');
    write1byteint((int)ax);    write1byteint((int)ay);    write1byteint((int)az);
    write1byteint((int)mx);    write1byteint((int)my);    write1byteint((int)mz);
    Serial.write(mode_G);
}
