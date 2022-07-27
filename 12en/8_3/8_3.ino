#define TRIG 7
#define ECHO 8
#define BZ 3 //ブザー
#define LED 9 //LEDを9ピンに定義

#define TEMP 26 //現在の温度

//距離計測に用いる変数
  unsigned long interval; //超音波が返ってくるまでの時間(ms)
  float interval_s;//超音波が帰ってくるまでの時間(μs)
  
  float c; //音速
  int d; //距離

void setup(){
  Serial.begin(9600);
  pinMode(TRIG,OUTPUT);
  pinMode(ECHO,INPUT);
  pinMode(BZ,OUTPUT);
  pinMode(LED,OUTPUT);
}

void loop(){
  //超音波センサーで距離を計測
  measure_dist();
  //ブザーを鳴らす
  tone(BZ,map(d,0,30,100,1000));

  //LEDを点灯させる
  if(d<=10){
    digitalWrite(LED,HIGH);
  }else{
    digitalWrite(LED,LOW);
  }

}
//超音波センサーを用いて距離を計測する
int measure_dist(){

    //10msのパルスをTringピンに出力
  digitalWrite(TRIG,HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG,LOW);

  //Echo信号がHIGHである時間（μs)をpalseIn関数で計測
  //23071μs以上経過したら、超音波が反射して帰ってこないとみなして0を返す。
  interval = pulseIn(ECHO, HIGH,23071);
  interval_s = (float)interval / 1000000.0;//Msをsに変換

  c = 0.61 * TEMP + 331.5; //音速を計算する
  d = (c * interval_s / 2.0) * 100.0; //距離を計算する(cm)
  
  Serial.println(d);//超音波の往復時間をシリアルモニタに表示
  delay(60);//次のTrig信号の出力まで60ms待機
  return d;
}
