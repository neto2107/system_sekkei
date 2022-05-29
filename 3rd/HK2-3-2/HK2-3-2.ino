//学籍番号 6120011749
//氏名　奥井翔麻

//BUSOR_PINを9とすることを定義
#define BUSOR_PIN 9
#define LUX_MIN 3 //光度の最低値
#define LUX_MAX 80//光度の最高値
float v0 = 5.0; //ベース電圧
float V_hold = 2.5; //閾値電圧
// the setup routine runs once when you press reset:
void setup() {
  
  //シリアル通信を9600bpsで初期化
  Serial.begin(9600);
  //BUSORPINのピン設定を出力にする。
  pinMode(BUSOR_PIN, OUTPUT);

}

// the loop routine runs over and over again forever:
void loop() {
  // A0ピンのAD変換結果を取得する
  int sensorValue = analogRead(A0);
  int busorValue = convert(sensorValue,LUX_MIN,LUX_MAX); //ブザーに接続されている回路のデューティー比

  // 求めた電圧をシリアル通信てPCに送信
  Serial.println(busorValue);
  //ブザー回路のPWM出力を設定
  analogWrite(BUSOR_PIN,busorValue);

}

//光度の最大値、最小値、入力からPWM制御の値を計算する(0-255)
int convert(int lux , int lux_min,int lux_max){
  //mapメソッドを使って最小値0,最大値255の値になるように変換
  return (int)map((long)lux,(long)lux_min,(long)lux_max,0,255);
}