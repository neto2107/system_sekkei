

float v0 = 5.0; //ベース電圧
void setup(){
  Serial.begin(9600);

}

void loop(){
  // A0ピンのAD変換結果を取得する
  int sensorValue = analogRead(A0);
  // AD変換結果から、電圧を求める
  float v = changeVoltage(sensorValue);
  //電圧を温度に変換
  float temp = changeTemp(v);
  Serial.print(sensorValue);//arduinoからの読み取り値
  Serial.print(",");
  Serial.print(v); //電圧値
  Serial.print(",");
  Serial.print(temp); //温度を表示（セルシウス温度)
  Serial.println(); //改行
  
  delay(1);        //待機時間
}

//センサー値を電圧値に変換する
//v0:ベース電圧
float changeVoltage(int value){
  return v0 * (value/1024.0);
}

//電圧を温度に変換する
float changeTemp(float volt){
 return (volt * 1000.0 - 600.0)/10.0;//電圧→温度
}