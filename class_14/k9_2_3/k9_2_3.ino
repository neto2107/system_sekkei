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
#define STOP    13 //停止
#define BACK    14 //後退
#define FORWARD2 15 //カップを検知しているときの前進処理
#define KEISOKU 16 //カップが移動しているかどうかを読み取る

#define DIST_LINE 15 //カップを検知する距離

//モータのglobal変数
ZumoMotors motors;
int motorL, motorR;

const int buzzerPin = 3; //ブザーは3ピン

//カラーセンサのglobal変数
Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_2_4MS, TCS34725_GAIN_60X);
float r, b, g;
const unsigned int r_min = 38, g_min = 51, b_min = 55;
const unsigned int r_max = 252, g_max = 255, b_max = 255;
int color; //色

//超音波センサのglobal変数
const int trig = 2; //Tringピン
const int echo = 4; //Echoピン
int dist;           //距離(cm)
int pre_dist;       //カップが動いているかどうかを見るときに使用する変数

//状態遷移のglobal変数
int mode;                 //ロボットの状態（初期状態／直進／回転／停止）
int keisoku_flag = 0;
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

int distance() {
  unsigned long interval; //Echoのパルス幅(μs)
  int dst;                //距離(cm)
  
  //10μsのパルスを超音波センサのTrigピンに出力
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);

  //5767μs(100cm)、超音波が反射してこなければタイムアウトしてエラー値0を返す
  //Echo信号がHIGHである時間(μs)を計測
  interval = pulseIn(echo, HIGH, 5767);
  
  dst = (0.61 * 25 + 331.5) * interval / 10000 / 2; //距離(cm)に変換
  if (dst == 0) {dst = 100;} //エラー値0（超音波が反射してこない）は100cmを検出したことにする
  delay(60); //trigがHIGHになる間隔を60ms以上空ける（超音波センサの仕様）

  return dst;
}

void task() {
  switch (mode) {
    case INIT: //初期状態
      mode = FORWARD;
      start_time = millis();
      break;
    case FORWARD: //直進
      motorL = motorR = 150;
      //黒を踏んだら後退→回転
      if (color == BLACK) {
        mode = BACK;
        start_time = millis();
      }
      if(millis() -start_time >1000){//1秒間直進したら回転する
        mode=ROTATE;
        start_time = millis();
      }
      break;
    case FORWARD2: 
      if(dist < DIST_LINE + 10&&color == BLACK){ //カップを検出し、ライン上にいるなら停止
        mode = STOP;
        buzzer(2,550); //ブザーを2飼いならす
      }else if(dist < DIST_LINE + 10){//カップを認識しているときの直進
        motorL = motorR = 150;
      }else{//カップを見失ったら探索に戻る
        mode=ROTATE;
        start_time = millis();
      }
      break;
    case ROTATE: //回転
      if (millis() - start_time < 1000) { //1秒回転
        motorL = -150; motorR = 150;
        if(dist < DIST_LINE){//カップを見つけたらKEISOKUに移行
          mode = KEISOKU;
          start_time =millis();
          buzzer(1 ,440); //ブザーを1回鳴らす
        }
      } else {
        mode = FORWARD;//1秒間回転したら直進する
        start_time = millis();//タイマーリセット
      }
      
      break;
    case BACK:
      if(millis()-start_time < 300){ //0.3s後退
        motorL = -150; motorR = -150;
      }else{//回転に移動
        mode = ROTATE;
        start_time = millis();//タイマーリセット
      }
      break;
    case KEISOKU:
      motorL = motorR = 0;
      if(millis()-start_time < 1000){ //1秒間計測
        if(!(dist > pre_dist - 3&&dist < pre_dist +3)){ //distが+-0の範囲に収まっていないなら
          keisoku_flag = 1; //フラグを1にする
          buzzer(3,440);
        }
        Serial.println(keisoku_flag);
      }else{ //1経過あと
        if(keisoku_flag ==0){//カップが静止しているなら
          mode = FORWARD2; //カップを外に出す
        }else{  //カップが動いているなら、
          mode = KEISOKU;
          pre_dist = dist;
          start_time = millis();//再び計測を行う。
        }
        if(dist > 30){
          mode = ROTATE;
          start_time = millis();//見失っているとき、探索に戻る
        }
        keisoku_flag = 0; //計測フラグをリセット
      }
      
      break;
    case STOP: //停止
      motorL = motorR = 0;
      break;
  }
}

void setup() {
  Serial.begin(9600);
  tcs.begin();
  pinMode(trig, OUTPUT); //trigを出力ポートに設定
  pinMode(echo, INPUT);  //echoを入力ポートに設定
  mode = INIT;
  motorL = motorR = 0;
}

int unsigned long buzzer_time;
int cnt = 0;
int b_mode =0;
//ブザーを鳴らすための変数
//常時引数0で呼び出し、音を鳴らしたいところで、鳴らしたい回数を引数にして呼び出す
void buzzer(int n,int hz){
  if(n>0){//nが0以上のときカウンターをセット
    cnt = n;
  }

  switch(b_mode){
    case 0:
      if(cnt>0){
        b_mode = 1;
      }
      break;
    case 1:
      tone(buzzerPin, hz);             //ブザーを440Hzで鳴らす
      if(millis() - buzzer_time>100){ //100ミリ秒経過したら音を消す
        b_mode = 2;
        buzzer_time = millis();
      }
      break;
    case 2://ブザーの音を消す
      noTone(buzzerPin);
      if(millis() - buzzer_time>100){//音が消えてから100ms経過したら、初期状態に戻る。
        b_mode = 0;
        cnt --;
      }
      break;
  }
  
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
  //Serial.println(color); //task()をコメントアウトし、ロボットを色の上に乗せて正しく識別できているか確認できる

  //距離を計測
  dist = distance();
  //Serial.println(dist);

  //ブザーを呼び出し
  buzzer(0,440);

  task(); //演習9.2.2の主な処理をする関数
}
