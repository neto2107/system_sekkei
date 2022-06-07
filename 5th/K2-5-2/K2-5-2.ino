
float v0 = 5.0; //ベース電圧
unsigned long preTime;//初期時間
const int RED_LED = 13;
const int YELLOW_LED = 9;
float tempSum = 0;
int count = 0;

void setup(){
  Serial.begin(9600);
  Serial.println("start");
  preTime = millis();
  pinMode(RED_LED,OUTPUT);
  pinMode(YELLOW_LED,OUTPUT);
  //温度の初期値を格納
  tempSum = calData();

}

void loop(){
  if(count < 100){
    unsigned long timeNow = millis();
    //温度を計測して平均をとる
    tempSum = (tempSum + calData())/2.0;
    if(timeNow - preTime >=100){
      SendData();
      preTime = timeNow;
    }
  }


}

float calData(){
  int sensorValue = analogRead(A0);
  float vo = changeVoltage(sensorValue);
  float Temp = changeTemp(vo);
  return Temp;
}
void SendData(){
  Serial.println(tempSum);
  count ++;
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