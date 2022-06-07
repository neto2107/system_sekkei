
float v0 = 5.0; //ベース電圧
unsigned long preTime;//初期時間
const int RED_LED = 13;
const int YELLOW_LED = 9;

int count = 0;

void setup(){
  Serial.begin(9600);
  Serial.println("start");
  preTime = millis();
  pinMode(RED_LED,OUTPUT);
  pinMode(YELLOW_LED,OUTPUT);

}

void loop(){
  if(count < 100){
    unsigned long timeNow = millis();
    if(timeNow - preTime >=100){
      SendData();
      preTime = timeNow;
    }
  }
}

void SendData(){
  int sensorValue = analogRead(A0);
  float vo = changeVoltage(sensorValue);
  float Temp = changeTemp(vo);
  Serial.println(Temp);
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