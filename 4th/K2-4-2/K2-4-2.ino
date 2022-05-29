unsigned long timePrev = 0; //基準となる時間を格納

const int LED1_PIN =13;
void flash(){ //LED点滅処理
  static boolean output = HIGH;//outputを宣言(初期値：HIGH)
  //デジタル出力設定
  digitalWrite(LED1_PIN, output);
  output = !output; //outputの値を反転させる。
}

void setup()
{
  //LED1_PINをアウトプットに設定
  pinMode(LED1_PIN,OUTPUT);
  //シリアル通信を9600bpsで初期化
  Serial.begin(9600);
  
}

void loop(){
  unsigned long timeNow = millis(); //millis関数を用いて現在の時間情報を取得
  if(timeNow - timePrev >=500){ //500ms経過

    Serial.println(analogRead(A0)); //A0ピンの状態を読み取ってシリアル送信
    timePrev = timeNow; //時間情報の更新
  }
}