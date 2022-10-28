#include <ZumoMotors.h>
#include <ZumoBuzzer.h>               // ブザーライブラリの読み込み

const int buzzerPin = 3;              // ブザーは 3 番ピン


byte inByte; //Processingからの送信要求を受け取る変数
unsigned long int timePrev, timeNow,timePrev2,timeNow2;
int move_flag = 0;
ZumoBuzzer buzzer;                    // ZumoBuzzer クラスのインスタンスを生成

ZumoMotors motors;
void setup(){
  Serial.begin(9600); //シリアル通信を9600bpsで初期化
  timePrev = millis();//時間を初期化する
    //buzzer.play("L16 cdegreg4");        // ブザーにて音楽を鳴らす
    //Processingに準備ができたことを送信する。
      Serial.write("H");
      Serial.write("S");
}

void loop(){
  //現在時刻を格納する
  timeNow = millis();
  if(timeNow -timePrev >= 50){ //前回の受信から50ms経過していたら、受信しているかを確認する
    inByte = Serial.read();//受信済みの信号を読み込む
    if(inByte == 0x32 && move_flag ==0){ //受け取った値が0xffなら(正規の要求なら)かつ、状態が初期状態なら
      timePrev2 = millis(); //モーターを動かすタイマーをセットする
      move_flag = 1; //状態を1に買える（動作状態）
      buzzer.play(">>a32");             // 音を鳴らす
    }
  }
  //1秒間受信しなかったら、タイマーをリセット
  if(timeNow - timePrev >= 1000){
  timePrev = millis();
  }
  // move_flagの状態が1の時のみ動かす
  if(move_flag == 1){
    move();
  }
}

//Arduinoを動かす処理(2秒間回転する
void move(){
      
  if(timeNow - timePrev2 <= 2000){
    motors.setSpeeds(150,-150);
  }else{//2秒経過したら
    //修了メッセージを送る。
    Serial.write("H");
    Serial.write("E");
    //速度を0にする
    motors.setSpeeds(0,0);
    //動作状態を変更する（修了）
    move_flag = 2;
  }
  
}
