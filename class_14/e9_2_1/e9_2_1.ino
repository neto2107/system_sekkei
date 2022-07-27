#include <ZumoMotors.h>        //モータのライブラリ
#include <Adafruit_TCS34725.h> //カラーセンサのライブラリ

#define WHITE    0 //白
#define BLACK    1 //黒
#define RED      2 //赤
#define BLUE     3 //青
#define OTHER    4 //その他
#define INIT    10 //初期状態
#define FORWARD 11 //直進
#define ROTATE  12 //回転

//モータのglobal関数
ZumoMotors motors;
int motorL, motorR; //モータ速度

//カラーセンサのglobal関数
Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_2_4MS, TCS34725_GAIN_60X);
float r, b, g;

const unsigned int r_min = 38, g_min = 51, b_min = 55;
const unsigned int r_max = 252, g_max = 255, b_max = 255;
int color; //色

//状態遷移のglobal関数
int mode;                 //ロボットの状態
unsigned long start_time; //時間計測の開始時間

int identify_color(int r, int g, int b) {
  if (200 < r && 200 < g && 200 < b) {
    return WHITE; //白
  } else if (r < 50 && g < 50 && b < 50) {
    return BLACK; //黒
  } else if (100 < r && g < 50 && b < 50) {
    return RED; //赤
  } else if (r < 50 && g < 50 && 70 < b) {
    return BLUE; //青
  } else {
    return OTHER; //その他
  }
}

void task() {
  switch (mode) {
    case INIT: //初期状態
      mode = FORWARD;
      break;
    case FORWARD: //直進
      motorL = motorR = 150;
      if (color == BLACK) { //黒を踏んだら回転
        mode = ROTATE;
        start_time = millis();
      }
      break;
    case ROTATE: //回転
      if (millis() - start_time < 300) { //0.3秒後退
        motorL = motorR = -150;
      } else if (millis() - start_time < 1300) { //1秒回転
        motorL = -150; motorR = 150;
      } else {
        mode = FORWARD;
      }
      break;
  }
}

void setup() {
  Serial.begin(9600);
  tcs.begin();
  mode = INIT;
  motorL = motorR = 0;
}

void loop() {
  //モータを駆動
  motors.setSpeeds(motorL, motorR);
  
  //色を計測
  tcs.getRGB(&r, &g, &b);
  r = map(r, r_min, r_max, 0, 255);
  g = map(g, g_min, g_max, 0, 255);
  b = map(b, b_min, b_max, 0, 255);

  color = identify_color(r, g, b);
  Serial.println(color); //task()をコメントアウトし、ロボットを色の上に乗せて正しく識別できているか確認できる

  //task(); //演習9.2.1の主な処理をする関数
}
