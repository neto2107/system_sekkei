#define TRIG 7
#define ECHO 8
#define BZ 3 //ブザー

#define TEMP 26 //現在の温度

//距離計測に用いる変数
  unsigned long interval; //超音波が返ってくるまでの時間(ms)
  float interval_s;//超音波が帰ってくるまでの時間(μs)
  
  float c; //音速
  int d; //距離

//Processingとの通信に用いる変数
unsigned int timeNow,timePrev;
byte inByte; //Processingからの送信要求を受け取る変数

void setup(){
  Serial.begin(9600);
  pinMode(TRIG,OUTPUT);
  pinMode(ECHO,INPUT);
  pinMode(BZ,OUTPUT);

  //timePrevを初期化する
  timePrev = millis();//時間を初期化する
}

void loop(){
  measure_dist();

  //圧電ブザーの音を鳴らす
  
  //tone(BZ,map(d,0,30,100,1000));

  trans_processing();
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
  
  //Serial.println(d);//超音波の往復時間をシリアルモニタに表示
  delay(60);//次のTrig信号の出力まで60ms待機
  return d;
}

void trans_processing(){
  timeNow = millis();
  if (timeNow - timePrev >= 50)
  {                  // 50msごとに実行(光センサーの計測値を送信)
    if(Serial.available()>0){//送信要求を受け取ったか確認
      inByte = Serial.read();//受信済みの信号を読み込む
      if(inByte ==0xff){//受け取った値が0xffなら（正規の送信要求なら）
        Serial.write('H');  //文字0x20をバイナリデータとして送信
        sendInt(d);
        timePrev = timeNow; // 1ステップ前の経過時間を更新
      }
    }
  }
  if(timeNow - timePrev >=1000){ //１秒以上送信要求がないなら、データを送信
        Serial.write('H');  //文字'h'をバイナリデータとして送信
        sendInt(d);
  }
}

//Int型の数値をsereal.writeで送信する
void sendInt(int n)
{
  int high = n >> 8; //上位8ビットをhighに代入
  int low = n & 255; //下位8ビットをlowに代入(下位8ビットがすべて1の値とAnd演算することで、上位8ビットがすべて0になる)
  Serial.write(high);
  Serial.write(low);
}
