//LEDのポートを設定
const int RED_LED = 13;
const int YELLOW_LED = 9;
//変数の宣言
unsigned long time_pre =0; //基準となる時間を格納
unsigned long time_pre2 = 0;
unsigned long time_now = 0; //現在時刻を格納
unsigned int cnt; //カウンター
boolean flag = false; //終了フラグ
boolean led_flag = LOW;//LEDの状態を表すフラグ

void setup()
{
  //ピンモードを設定
  pinMode(RED_LED, OUTPUT);
  pinMode(YELLOW_LED,OUTPUT);
  //シリアル通信を9600bpsで初期化
  Serial.begin(9600);
}

void loop()
{
    time_now = millis(); //開始からの時間をtime_nowに格納

    if(cnt < 10){ //カウンターが回以下なら
      //1s経過ごとに実行
      if(time_now - time_pre >=1000){
        //照度センサーの値をシリアル通信
        Serial.println(analogRead(A0));
        //経過時間をリセット
        time_pre = time_now;
        cnt ++; //カウンターを増加させる
      }
      //500ms経過ごとに実行
      if(time_now - time_pre2 >=500){
        digitalWrite(YELLOW_LED,led_flag);
        //led_flagを切り替え(LOW -> HIGH, HIGH -> LOW)
        led_flag = !led_flag;
        //time_pre2にtime_nowを代入(経過時間をリセット)
        time_pre2 = time_now;
      }

    }else{ //10回計測し終わったら
      if(flag == false){ //最初
        digitalWrite(RED_LED, HIGH);
        time_pre = time_now;
        flag = true;
      }else if(flag == true && time_now -time_pre == 500){//終了から500ms経過
        digitalWrite(YELLOW_LED, LOW);
      
      //終了から1ms経過
      }else if(flag == true && time_now -time_pre == 1000){
        digitalWrite(RED_LED, LOW);
      }
    }
}