unsigned long timePrev = 0; //基準となる時間を格納

const int LED1_PIN =13;
void flash(){ //LED点滅処理
  void flash() {
  static boolean output = HIGH;
  
  digitalWrite(LED1_PIN, output);
  output = !output;
}

void setup()
{
  pinMode(LED1_PIN,OUTPUT);
}

void loop(){
  unsigned long timeNow = millis(); //millis関数を用いて現在の時間情報を取得
  if(timeNow - timePrev >=750){ //750ms経過
    flash();
    timePrev = timeNow; //時間情報の更新
  }
}