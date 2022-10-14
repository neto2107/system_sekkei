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
      Serial.write("H");
      Serial.write("S");
}

void loop(){
  timeNow = millis();
  if(timeNow -timePrev >= 50){
    inByte = Serial.read();//受信済みの信号を読み込む
    if(inByte == 0x32 && move_flag ==0){ //受け取った値が0xffなら(正規の要求なら)
      timePrev2 = millis();
      move_flag = 1;
      buzzer.play(">>a32");             // 音を鳴らす
    }
  }
  if(timeNow - timePrev >= 1000){
  timePrev = millis();
  }

  if(move_flag == 1){
    move();
  }
}

void move(){
      
  if(timeNow - timePrev2 <= 2000){
    motors.setSpeeds(150,-150);
  }else{
    Serial.write("H");
    Serial.write("E");
    motors.setSpeeds(0,0);
    move_flag = 2;
  }
  
}