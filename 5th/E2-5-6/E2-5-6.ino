

float v0 = 5.0; //ベース電圧
unsigned long preTime;//初期時間
const int RED_LED = 13;
const int YELLOW_LED = 9;

void setup(){
  Serial.begin(9600);
  preTime = millis();
  pinMode(RED_LED,OUTPUT);
  pinMode(YELLOW_LED,OUTPUT);

}

void loop(){
  unsigned long now_time = millis();
  // A0ピンのAD変換結果を取得する
  int sensorValue = analogRead(A0);
  // AD変換結果から、電圧を求める
  float v = changeVoltage(sensorValue);
  //電圧を温度に変換
  float temp = changeTemp(v);
  //Serial.print(sensorValue);//arduinoからの読み取り値
  //Serial.print(",");
  //Serial.print(v); //電圧値
  //Serial.print(",");
  //経過時間が10秒以内の時
  if(now_time - preTime <= 10000){
    Serial.print(temp); //温度を表示（セルシウス温度)
    Serial.println(); //改行
    digitalWrite(RED_LED,HIGH);
  }else{
    digitalWrite(RED_LED,LOW);
  }
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