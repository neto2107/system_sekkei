//演習9.1.5
#include <ZumoMotors.h>          //モータライブラリの読み込み
#include <Pushbutton.h>


const int trig = 2;              //TrigピンをArduinoの2番ピンに
const int echo = 4;              //EchoピンをArduinoの4番ピンに
const int routateTime=4900; //1回転する時間
int mode = 0;//0:未検出 1:検出中 2:キャッチ
int mode2 = 0;//探索時のモード

unsigned long timeNow;
unsigned long timePrev;

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

void changeStatus(){
   int dist = distance();;//超音波センサーの値を格納しておく変数
   Serial.println(dist);             //距離をシリアルモニタに出力
   if(dist>=30 || dist == 0){ //カップを発見できていないなら
      mode = 0;
   }else if(dist < 30 && dist > 4){//カップを発見したら
      mode = 1; 
   }else if(dist <=4){//カップをキャッチしたら
      mode = 2;
   }else{//そのほかの時、探索モード
      mode = 0;
   }
}

void tansaku(){
  if(mode2 == 0){ //モードが0の時回転
    motors.setLeftSpeed(-150);
    motors.setRightSpeed(150);
    if(timeNow-timePrev >= routateTime){//1回転したら前進する
      mode2 = 1;
      timePrev = timeNow;
    }
  }if(mode2 == 1){//モードが1の時前進
    motors.setLeftSpeed(100);
    motors.setRightSpeed(100);
    if(timeNow-timePrev >=1000){//1秒前進したら回転に戻る
      mode2 = 0;
      timePrev = timeNow;
    }
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  timeNow = millis();
  changeStatus();
  switch(mode){
    case 0://未探索の時旋回
        tansaku();
        break;
    case 1://発見したとき全身
      motors.setLeftSpeed(100);
      motors.setRightSpeed(100);
      break;

    case 2://キャッチしたとき停止
      motors.setLeftSpeed(0);
      motors.setRightSpeed(0);
  }

}
  
