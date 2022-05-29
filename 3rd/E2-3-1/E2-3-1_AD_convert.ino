//学籍番号　6120011749
//氏名　奥井翔麻
// the setup routine runs once when you press reset:
void setup() {
  
  //シリアル通信を9600bpsで初期化
  Serial.begin(9600);
}

// the loop routine runs over and over again forever:
void loop() {
  // A0ピンのAD変換結果を取得する
  int sensorValue = analogRead(A0);
  // シリアル通信によりAD変換結果をPCへ送信
  Serial.println(sensorValue);
  delay(1);        //待機時間
}