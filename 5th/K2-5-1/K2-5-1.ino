int output = LOW; //RED_LEDへの出力用
int output2 = LOW; //YELLOW_LEDへの出力用

//LEDピンを定数指定
const int RED_LED = 13;
const int YELLOW_LED =9;
//文字"0"のASCIIコード
const int zero = 48;
void setup(){
  pinMode(RED_LED,OUTPUT);
  pinMode(YELLOW_LED,OUTPUT);
  Serial.begin(9600);//シリアル通信開始と転送レート指定
}

void loop(){
  if(Serial.available()>0){
    int recv = Serial.read();//受信データの読み込み
    //"0"のASCIIコード48に一致するときに実行
    if(recv == zero){
      output = !output;
    //"1"のASCIIコード49に一致するときに実行
    }else if(recv == zero + 1){
      output2 = !output2;
    }else{
      output = !output;
      output2 = !output2;
    }
    //受信内容をPCへ送信
    Serial.print("I received!!"); //パソコンへ応答を返す
    Serial.print(",");
    Serial.print(recv);
    Serial.println();
  }
  //LEDの点灯
  digitalWrite(RED_LED,output);
  digitalWrite(YELLOW_LED,output2);
}