//学籍番号 6120011749
//氏名　奥井翔麻

#define LED_PIN 3

float v0 = 5.0; //ベース電圧
int L_base=88.3; //閾値

//luxが閾値(th_val)以上の時,HIGH,閾値以下の時LOWを返す関数
int lux_threshold(float lux,int th_val){
  if(lux >=th_val){//luxが閾値以上の時
    return HIGH;
  }else{  //luxが閾値以下の時
    return LOW;
  }
}

// the setup routine runs once when you press reset:
void setup() {
  
  //シリアル通信を9600bpsで初期化
  Serial.begin(9600);
  //LED_PINを出力モードに設定
  pinMode(LED_PIN, OUTPUT);
}

// the loop routine runs over and over again forever:
void loop() {
  // A0ピンのAD変換結果を取得する
  int sensorValue = analogRead(A0);
  // AD変換結果から、電圧を求める
  float v = v0 * ((float)sensorValue/1024);
  //電圧から照度を求める(lx)
  float L = 222 * v;
  // 求めた電圧をシリアル通信てPCに送信
  Serial.println(L);
  //照度が閾値以上かどうかで、LEDを点灯
  digitalWrite(LED_PIN,(uint8_t)lux_threshold(L,L_base));
  
  delay(1);        //待機時間
}

