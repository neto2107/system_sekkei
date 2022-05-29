#include <MsTimer2.h>

//LEDのポートを設定
const int RED_LED = 13;
const int YELLOW_LED = 9;
//変数の宣言
unsigned long time_pre =0; //基準となる時間を格納
unsigned long time_pre2 = 0;
boolean flag = false; //終了フラグ
boolean led_flag = LOW;//LEDの状態を表すフラグ

void setup()
{
  //ピンモードを設定
  pinMode(RED_LED, OUTPUT);
  pinMode(YELLOW_LED,OUTPUT);
  //シリアル通信を9600bpsで初期化
  Serial.begin(9600);

  //タイマー割り込みの設定
  MsTimer2::set(1000,flash); //1sごとに呼び出す
  MsTimer2::start();
}

void loop()
{
    unsigned long time_now = millis(); //開始からの時間をtime_nowに格納
    //1s経過ごとに実行
    if(time_now - time_pre >=1000 && flag == false){
      //照度センサーの値をシリアル通信
      Serial.println(analogRead(A0));
      //経過時間をリセット
      time_pre = time_now;
    }
    //500ms経過ごとに実行
    if(time_now - time_pre2 >=500 && flag ==false){
      digitalWrite(YELLOW_LED,led_flag);
      //led_flagを切り替え(LOW -> HIGH, HIGH -> LOW)
      led_flag = !led_flag;
      //time_pre2にtime_nowを代入(経過時間をリセット)
      time_pre2 = time_now;
    }

}

//タイマー割り込み時に呼び出される関数
unsigned int timer_cnt; //flashが何回呼ばれたかを格納する関数
void flash(){
  timer_cnt ++;
  if(timer_cnt ==10){ //１０回目に呼び出された時(10s経過)

    //RED_LEDを点灯させる
    digitalWrite(RED_LED,HIGH);
  }
  if(timer_cnt ==11) //11回目に呼び出された時(11s経過)
  {
    //終了フラグをtrueにする
    flag = true;
    //LEDを消灯させる。
    digitalWrite(RED_LED,LOW);
    //自己停止
    MsTimer2::stop();
  }


}