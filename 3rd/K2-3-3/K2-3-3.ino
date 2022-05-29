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
  Serial.begin(9600);
}

// the loop routine runs over and over again forever:
void loop() {
  //RED_LEDのディジタルPWM出力
  for(int i = 0; i<=4;i++){
    //analogWriteでPWM制御、デューティー比は0.25にiをかけることで算出
    analogWrite(RED_LED_PIN,changeval(0.25 * i));
    //１秒間待つ
    delay(1000);
    //シリアル通信でvalの値をpcに送信
    Serial.println(changeval(i*0.25));
  }
}

//デューティ比を0-255の値に変換する
int changeval(float raito){
  return (int)((float)raito * 255);
}