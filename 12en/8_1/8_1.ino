#define TRIG 7 //7ピンをTRIGで定義
#define ECHO 8 //8ピンをECHOで定義
//超音波センサから読み取った往復時間を格納する
unsigned long interval;

void setup() {
  Serial.begin(9600);
  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);
}

void loop() {
  //10msのパルスをTringピンに出力
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);

  //Echo信号がHIGHである時間（μs)をpalseIn関数で計測
  //23071μs以上経過したら、超音波が反射して帰ってこないとみなして0を返す。
  interval = pulseIn(ECHO, HIGH, 23071);

  Serial.println(interval);//超音波の往復時間をシリアルモニタに表示
  delay(60);//次のTrig信号の出力まで60ms待機
}
