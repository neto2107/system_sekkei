
const int LED_PIN = 13; //LED_PINを13とする
volatile int output = LOW; //ディジタル出力の値を格納する変数
volatile boolean lock_flag = false; //チャタリング防止用のフラグ
boolean lock_flag2 =false; //タイマーのセットを管理するフラグ
unsigned long pre_time; //前回時点の時刻を格納
unsigned long now_time; //現在時刻を格納

//初期設定
void setup(){
  //ピンモードのセットアップ

  pinMode(LED_PIN,OUTPUT);
  Serial.begin(19200);
  /*
  割り込みに使用するデジタルポートをD2に設定
  割り込みサービスルーチンblinkを実行
  割り込み要因としてスイッチの状態がLOWの時に実行
  */
  attachInterrupt(0, blink, LOW);
}

void loop(){
  now_time = millis(); //起動からの時刻を代入
  //LEDの出力状況を更新
  digitalWrite(LED_PIN, output);
  //割り込み処理が発生したら
  if(lock_flag == true&&lock_flag2 == false){
    pre_time = now_time;
    lock_flag2 =true; //lock_flag2を真にする
  }else if(lock_flag == true && now_time - pre_time >= 50){ //50msたったら、
    //フラグをリセット
    lock_flag = false;
    lock_flag2 = false;
  }
}

void blink(){
  //最初の反応以降は無視するようにする（チャタリング防止)
  if(lock_flag ==false){
  output = !output; //ディジタル出力を反転
  lock_flag = true;
  }
}