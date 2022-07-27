//課題9.1.3
#include <ZumoMotors.h>          //モータライブラリの読み込み
#include <Pushbutton.h>


const int trig = 2;              //TrigピンをArduinoの2番ピンに
const int echo = 4;              //EchoピンをArduinoの4番ピンに

const int routateTime=2000; //1回転する時間


unsigned int timeNow,timePrev,timePrev2,timePrev3,timePrev4; //時間計測用
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
  timePrev2 = millis();
  timePrev3 = millis();
  timePrev4 = millis();

}


int mode = 0;
int timeDist = 0;//カップを見つけてから見失うまでの時間
int timeoutflag = 0;
void moveRobot(){
  Serial.println(mode);
  if(mode == 1){//未発見状態だったら探索行動を取る
    //探索行動 
    tansaku();
  }else if(mode == 2){ //発見したとき、回転を続け、センシング時間を算出
    motors.setLeftSpeed(-150);
    motors.setRightSpeed(150);
    timeDist = calSensor();
    if(timeDist != 0){//カップのセンシング時間を得たとき
      mode = 3;
    }
    if(timeoutflag ==0){ //1秒後にタイムアウトできるように,モード2に移行したときの時間を取得
      timePrev3 = timeNow;
      timeoutflag = 1;
    }else if(timeoutflag ==1&& timeNow-timePrev3 > 1000){ //1秒後にタイムアウトし、探索に戻る。
      mode =1;
      timeoutflag =0;
    }
    
  }
  if(mode ==3){ //カップのセンシング時間を取得したら、カップが中心に来るように調整する。
    Serial.println(timeDist);
    if(sensingRobot(timeDist)==1){ //カップが真正面に来るように調整したら、次のモードに進む
      mode = 4;
    }
  }else if(mode ==4){//探索が終了したとき前進させる
    motors.setLeftSpeed(100);
    motors.setRightSpeed(100);
    if(dist>=30 || dist ==0){ //カップを見失ったら、再び探索に戻る
      mode = 0; //リセット
    }
    if(dist<=4 && dist >0){ //カップが4cm以下に近づいたら、停止させる。
      mode = 5;
    }
  }
  if(mode == 5){//キャッチしたら停止させる。
    motors.setLeftSpeed(0);
    motors.setRightSpeed(0);
    if(dist>=30 || dist ==0){ //カップを見失ったら、再び探索に戻る。
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
  if((timeNow - timePrev2 <= ((timeDist/2)+50))&&mode3 ==1){ //センシング時間の半分の時間逆回転させる
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

void tansaku(){
  if(mode2 == 0){ //モードが0の時回転
    motors.setLeftSpeed(-150);
    motors.setRightSpeed(150);
    if(timeNow-timePrev3 >= routateTime){//1回転したら前進する
      mode2 = 1;
      timePrev3 = timeNow;
    }
  }if(mode2 == 1){//モードが1の時前進
    motors.setLeftSpeed(100);
    motors.setRightSpeed(100);
    if(timeNow-timePrev3 >=1000){//1秒前進したら回転に戻る
      mode2 = 0;
      timePrev3 = timeNow;
    }
  }
}

void loop() {
  timeNow = millis();
  
  // put your main code here, to run repeatedly:
  dist = distance();;//超音波センサーの値を格納しておく変数
  moveRobot();

}
  
