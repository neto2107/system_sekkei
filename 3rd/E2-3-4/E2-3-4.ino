//学籍番号 6120011749
//氏名　奥井翔麻

#define RED_LED_PIN 9
#define YELLO_LED_PIN 11

//float v0 = 5.0; //ベース電圧

// the setup routine runs once when you press reset:
void setup() {
  //ピンモードを設定
  pinMode(RED_LED_PIN,OUTPUT);
  pinMode(YELLO_LED_PIN,OUTPUT);

  //シリアル通信を9600bpsで初期化
  //Serial.begin(9600);
}

// the loop routine runs over and over again forever:
void loop() {
  //RED_LEDのディジタルPWM出力
  analogWrite(RED_LED_PIN,changeval(0.25));
  analogWrite(YELLO_LED_PIN,changeval(0.75));
}

//デューティ比を0-255の値に変換する
int changeval(float raito){
  return (int)((float)raito * 255);
}