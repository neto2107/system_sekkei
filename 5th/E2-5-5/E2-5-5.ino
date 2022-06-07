int output = LOW; //LEDへの出力用
void setup(){
  pinMode(13,OUTPUT);
  Serial.begin(9600);//シリアル通信開始と転送レート指定
}

void loop(){
  if(Serial.available()>0){
    int recv = Serial.read();//受信データの読み込み
    
    output = !output;
    Serial.println("I received!!"); //パソコンへ応答を返す
  }
  digitalWrite(13,output);
}