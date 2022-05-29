const int LED_PIN = 13; //LEDをD13に設定
volatile int output = LOW; //ディジタル出力の値を格納する変数

//初期設定
void setup(){
  pinMode(LED_PIN,OUTPUT); //LED_PINをデジタル出力設定

  //外部割り込みの設定
  attachInterrupt(0,blink,CHANGE);
}

void loop(){
  digitalWrite(LED_PIN,output);
}

void blink(){
  output = !output; //ディジタル出力を反転
}