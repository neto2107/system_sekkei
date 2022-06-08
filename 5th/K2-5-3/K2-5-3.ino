const int index_max = 5;    //記憶しておく値の個数
const int lux_max = 222 * 5; //照度の最大値5v*222
float array_lux[index_max]; //照度を記録しておく配列
float sum_lux;              //照度の平均値
unsigned long preTime;      //計測間隔(100ms)
unsigned long preTime2;     //送信間隔(500ms)
unsigned long preTime3;     //計測時間(15s)
void setup()
{
  //シリアル通信を開始
  Serial.begin(9600);
  //ピンモードの設定
  pinMode(13, OUTPUT);
  pinMode(9, OUTPUT);

  preTime = millis();
  preTime2 = millis();
  preTime3 = millis();
}

void loop()
{
  //開始からの時刻を取得
  unsigned long nowTime = millis();
  if (nowTime - preTime3 <= 15000)
  {
    digitalWrite(13, HIGH);

    if (nowTime - preTime >= 100)
    { // 100ms経過後データを取得する
      //配列を１つ前にシフトする
      shift_array(array_lux, index_max);
      //配列の最後に計測値を追加
      array_lux[index_max - 1] = getData();
      //時刻を初期化
      preTime = nowTime;
    }
    if (nowTime - preTime2 >= 500)
    {
      //照度の区間平均を研鑽
      sum_lux = calAverage(array_lux);
      sendData(sum_lux);
      //9ピンにPWM出力、map関数を使って照度を0-255の値に変換
      analogWrite(9,map(sum_lux,0,lux_max,0,255));
      preTime2 = nowTime;
    }
  }
  else//15s経過したら計測を終了する
  {
    digitalWrite(13, LOW);
    digitalWrite(9,LOW);
  }
}
//配列の要素を１つ前にシフトする
void shift_array(float array[], int index_max)
{
  for (int i = 0; i < index_max - 1; i++)
  {
    array[i] = array[i + 1]; //１つ後ろから前方にシフト
  }
}
//配列の要素について平均を計算する
float calAverage(float array[])
{
  float sum = 0.0;
  for (int i = 0; i < index_max; i++)
  {
    sum += array[i];
  }
  return sum / (float)index_max;
}

//データを取得し、照度に変換する
float getData()
{
  int val = analogRead(A0);
  float vo = changeVoltage(val);
  float lx = changeLx(vo);
  return lx;
}
//データをPCに送信する
float sendData(float lx)
{
  Serial.println(lx);
}

//センサー値を電圧値に変換する
// v0:ベース電圧
float v0 = 5.0;
float changeVoltage(int value)
{
  return v0 * ((float)value / 1024.0);
}
//電圧から照度を求める
float changeLx(float voltage)
{
  return 222.0 * voltage;
}