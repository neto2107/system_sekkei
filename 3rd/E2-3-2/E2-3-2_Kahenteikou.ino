//学籍番号 6120011749
//氏名　奥井翔麻


float v0 = 5.0; //ベース電圧
// the setup routine runs once when you press reset:
void setup() {
  
  //シリアル通信を9600bpsで初期化
  Serial.begin(9600);
}

// the loop routine runs over and over again forever:
void loop() {
  // A0ピンのAD変換結果を取得する
  int sensorValue = analogRead(A0);
  // AD変換結果から、電圧を求める
  float v = v0 * ((float)sensorValue/1024);
  // 求めた電圧をシリアル通信てPCに送信
  Serial.println(v);
  
  delay(1);        //待機時間
}