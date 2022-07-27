#define TRIG 7
#define ECHO 8
#define BZ 3 //ブザー

#define TEMP 26 //現在の温度

//距離計測に用いる変数
  unsigned long interval; //超音波が返ってくるまでの時間(ms)
  float interval_s;//超音波が帰ってくるまでの時間(μs)
  
  float c; //音速
  int d; //距離
  int d_avg;//距離の移動平均をとった値

void setup(){
  Serial.begin(9600);
  pinMode(TRIG,OUTPUT);
  pinMode(ECHO,INPUT);
  pinMode(BZ,OUTPUT);

}

void loop(){
  measure_dist();
  count_human();
}

//超音波センサを用いて距離を計測する。
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
  d_avg = moving_average(d);//距離の移動平均を計算する。
  
  //Serial.println(d);//超音波の往復時間をシリアルモニタに表示
  delay(60);//次のTrig信号の出力まで60ms待機
  return d_avg;
}

int count = 0;
int sum = 0;
int data[] = {0,0,0,0,0};
//移動平均を求めるプログラム
int moving_average(int x){
  if(count == 5){
    count = 0;
  }
  sum -= data[count]; //古いデータを削除
  data[count] = x; 
  sum += data[count]; //新しいデータを追加
  count ++;
  return sum/5;//過去5回の移動平均を返す
}


boolean flag = false;
int cnt_human = 0;//人間のカウント値
//横切った人間をカウントするメソッド
int count_human(){
  if(flag==false && d_avg < 20&&d>0){
    cnt_human ++;
    flag = true;
    Serial.println("入室を検知");
    Serial.print(cnt_human);
    Serial.println("人目");
  }
  if(flag == true && d_avg > 50){
    flag = false;
  }
  return cnt_human;
}
