//課題13.1
#include <ZumoMotors.h>          //モータライブラリの読み込み
#include <Pushbutton.h>


const int trig = 2;              //TrigピンをArduinoの2番ピンに
const int echo = 4;              //EchoピンをArduinoの4番ピンに


unsigned int timeNow,timePrev,timePrev2; //時間計測用
int dist; //超音波センサーの計測値(cm)

ZumoMotors motors;               //ZumoMotorsクラスのインスタンス生成
Pushbutton button(ZUMO_BUTTON);  //Pushbuttonクラスのインスタンスを生成

//距離を計測
int distance() {
  unsigned long interval;        //Echoのパルス幅(μs)
  int dist;                      //距離(cm)
  
  digitalWrite(trig, HIGH);      //10μsのパルスを超音波センサのTrigピンに出力
  delayMicroseconds(10);
  digitalWrite(trig, LOW);

  interval = pulseIn(echo, HIGH, 23071);               //Echo信号がHIGHである時間(μs)を計測
  dist = (0.61 * 25 + 331.5) * interval / 10000 / 2;   //距離(cm)に変換

  delay(60);                     //trigがHIGHになる間隔を60ms以上空ける（超音波センサの仕様）

  return dist;
}

void setup() {
  // put your setup code here, to run once:
  button.waitForButton();        //ユーザボタンが押されるまで待機
  Serial.begin(9600);            //シリアル通信を9600bpsに初期化
  pinMode(trig, OUTPUT);         //trigを出力ポートに設定
  pinMode(echo, INPUT);          //echoを入力ポートに設定
  timePrev = millis();

}


int mode = 0;
int timeDist = 0;//カップを見つけてから見失うまでの時間
void moveRobot(){
  Serial.println(mode);
  if(mode == 1){//未発見状態だったら左回転
    //左回転 
    motors.setLeftSpeed(-150);
    motors.setRightSpeed(150);
  }else if(mode == 2){ //発見したとき、回転を続け、センシング時間を算出
    motors.setLeftSpeed(-150);
    motors.setRightSpeed(150);
    timeDist = calSensor(); //カップを発見してから見失うまでの時間時間を算出し、代入
    if(timeDist != 0){//カップのセンシング時間を得たとき
      mode = 3;
    }
  }
  if(mode ==3){//カップのセンシング時間が算出できたとき、
    Serial.println(timeDist);
    if(sensingRobot(timeDist)==1){ //センシング時間だけ逆回転させて、カップが真正面に来るようにする。
      mode = 4;
    }
  }else if(mode ==4){//探索が終了したとき、停止させる。
    motors.setLeftSpeed(0);
    motors.setRightSpeed(0);
    if(dist>=30 || dist ==0){ //カップを再び見失ったとき
      mode = 0; //リセット
    }
  }

  if((dist >=30 || dist == 0)&&mode == 0){
    mode = 1; //探索モード
    Serial.println("探索モード");
  }else if(dist <=30&&mode ==1){
    mode = 2; //調整モード
    Serial.println("位置の調整");
  }
}

int mode3 = 0;
//センサーがカップを検出したときの動作;
//戻り値　動作中:0 停止:1
int sensingRobot(int timeDist){
  //タイマーをリセット
  if(mode3 == 0){
    timePrev2 =timeNow;
    mode3 =1;
  }
  Serial.println(timeNow - timePrev2);
  Serial.println(((timeDist/2) +10000));
  if((timeNow - timePrev2 <= ((timeDist/2)+100))&&mode3 ==1){ //センシング時間の半分の時間逆回転させる
    motors.setLeftSpeed(150);
    motors.setRightSpeed(-150);
  }else if(mode3 == 1){
    mode3 = 2;
  }
  if(mode3 == 2){
    mode3 = 0; //モードをリセット
    return 1;
  }
  return 0;
}

//カップを検知してから外れるまでにどれだけの時間が立ったかを検知する関数
int mode2 = 0;
int calSensor(){
    // put your main code here, to run repeatedly:
  int dist = distance();;//超音波センサーの値を格納しておく変数
  int distTime = 0 ;//センサーが検知してから見失うまでの時間
  if(mode2 ==0){ //最初だけ
    mode2 = 1; //モードを変更
    timePrev = timeNow; //タイマーをリセット

  }
  if((dist >= 30||dist ==0) && mode2 == 1){
    mode2 = 0; //モードリセット
    distTime = timeNow -timePrev; //センシング時間を算出

  }

  return distTime; //センシング時間を返す
}

void loop() {
  timeNow = millis();
  
  // put your main code here, to run repeatedly:
  dist = distance();//超音波センサーの値を格納しておく変数
  moveRobot(); //　ロボットを動作させる。

}
  
