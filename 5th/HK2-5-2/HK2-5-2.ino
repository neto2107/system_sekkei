int flag = 0;            //現在の進行状態のフラグ
const int index_max = 5; //配列の要素数
float array_lux[index_max];
float array_temp[index_max];
unsigned long pre_time;  //タイマー
unsigned long pre_time2; //計測間隔(40ms)
unsigned long pre_time3; //送信間隔(200ms)
unsigned long now_time;  //開始からの時刻を格納

float lux_max = 0;     //照度センサーの最大値
float lux_min = 1000;  //照度センサー最小値
float temp_max = 0;    //温度センサーの最大値
float temp_min = 1000; //温度センサーの最小値

#define RED_LED 13
#define YELLOW_LED 9
enum stts
{
  start = 0,
  calibLux = 1, //照度センサーのキャリブレーション
  calibLux2 = 2,
  calibTem = 3, //温度センサーのキャリブレーション
  calibTem2 = 4,
  measure_lux = 5, //照度センサーの計測
  measure_lux2 = 6,
  measure_temp = 7, //温度センサーの計測
  measure_temp2 = 8,
  end = 9
};

void setup()
{
  //シリアル通信を開始
  Serial.begin(9600);
  //ピンモードの設定
  pinMode(RED_LED, OUTPUT);
  pinMode(YELLOW_LED, OUTPUT);
  // pre_timeをセット
  pre_time = millis();

}

void loop()
{

  //現在時刻を格納
  now_time = millis();

  //初期状態
  if (flag == start)
  {
    Serial.println("キャリブレーションを開始します。");
    flag = calibLux;
  }
  //光センサーのキャリブレーション
  if (flag == calibLux)
  {
    Serial.println("照度センサーのキャリブレーションを開始します。");
    Serial.println("キャリブレーション中は黄色のLEDが点灯します。");
    digitalWrite(YELLOW_LED, HIGH);
    flag = calibLux2; //次の状態にセット
    pre_time = now_time;
  }
  if (flag == calibLux2)
  {
    //キャリブレーションを行う関数を呼び出す
    calibrationLux();
  }
  //温度センサーのキャリブレーション
  if (flag == calibTem)
  {
    Serial.println("温度センサーのキャリブレーションを開始します。");
    Serial.println("キャリブレーション中は黄色のLEDが点灯します。");
    digitalWrite(YELLOW_LED, HIGH);
    flag = calibTem2;
    pre_time = now_time;
  }
  if (flag == calibTem2)
  {
    //キャリブレーションを行う関数を呼び出す
    calibrationTemp();
  }
  if (flag == measure_lux)
  {
    Serial.println("照度センサーの計測を開始します。");
    Serial.println("センサーの計測中は赤色のLEDが点灯します。");
    //赤色のLEDを点灯させる
    digitalWrite(RED_LED, HIGH);
    flag = measure_lux2;
    //タイマー類をリセット
    pre_time = now_time;
    pre_time2 = now_time;
    pre_time3 = now_time;
  }
  if (flag == measure_lux2)
  {
    //計測を行う関数を呼び出す
    measureLux();
  }
  if (flag == measure_temp)
  {
    Serial.println("温度センサーの計測を開始します。");
    Serial.println("センサーの計測中は赤色のLEDが点灯します。");
    digitalWrite(RED_LED, HIGH);
    flag = measure_temp2;
    //タイマー類をリセット
    pre_time = now_time;
    pre_time2 = now_time;
    pre_time3 = now_time;
  }
  if(flag == measure_temp2){
    //計測を行う関数を呼び出す
    measureTemp();
  }
}
//照度センサーの計測
void measureLux()
{
  if (now_time - pre_time <= 15000)
  {                                 // 15秒間
    if (now_time - pre_time2 >= 40) // 40ms間隔
    {
      //配列を１つずつ左にシフト
      shift_array(array_lux, index_max);
      //最後に計測値を正規化した値を追加
      array_lux[index_max - 1] = map(getLux(), lux_min, lux_max, 0, 255);
      pre_time2 = now_time; //時刻をリセット
    }
    if (now_time - pre_time3 >= 200) // 200ms間隔
    {
      //区間平均を計算
      float ave = calAverage(array_lux);
      //区間平均を送信
      Serial.println(ave);
      //時刻をリセット
      pre_time3 = now_time;
    }
  }
  else
  {
    Serial.println("照度センサーの計測を終了します。");
    digitalWrite(RED_LED, LOW);
    flag = measure_temp;
  }
}

//温度センサーの計測
void measureTemp()
{
  if (now_time - pre_time <= 15000)
  {                                 // 15秒間
    if (now_time - pre_time2 >= 40) // 40ms間隔
    {
      //配列を１つずつ左にシフト
      shift_array(array_temp, index_max);
      //最後に計測値を正規化した値を追加
      array_temp[index_max - 1] = map(getTem(), temp_min, temp_max, 0, 255);
      pre_time2 = now_time; //時刻をリセット
    }
    if (now_time - pre_time3 >= 200) // 200ms間隔
    {
      //区間平均を計算
      float ave = calAverage(array_temp);
      //区間平均を送信
      Serial.println(ave);
      //時刻をリセット
      pre_time3 = now_time;
    }
  }
  else
  {
    Serial.println("温度センサーの計測を終了します。");
    digitalWrite(RED_LED, LOW);
    flag = end;
  }
}

//温度のキャリブレーション
void calibrationTemp()
{
  if (now_time - pre_time <= 10000)
  {
    // 10秒間キャリブレーションを行う
    float temp = getTem(); //温度を取得
    if (temp < temp_min)
    {
      temp_min = temp;
    }
    if (temp > temp_max)
    {
      temp_max = temp;
    }
  }
  else
  {
    Serial.println("温度センサのキャリブレーションが完了しました。");
    //キャリブレーション結果を送信
    sendCalibrationData(temp_min, temp_max);
    digitalWrite(YELLOW_LED, LOW);
    flag = measure_lux;
  }
}

//照度のキャリブレーション
void calibrationLux()
{
  if (now_time - pre_time <= 10000)
  { // 10秒間キャリブレーションを行う
    float lux = getLux();
    if (lux < lux_min)
    {
      lux_min = lux;
    }
    if (lux > lux_max)
    {
      lux_max = lux;
    }
  }
  else //キャリブレーション終了時
  {
    Serial.println("光センサのキャリブレーションが完了しました。");
    //キャリブレーション結果を送信
    sendCalibrationData(lux_min, lux_max);
    digitalWrite(YELLOW_LED, LOW);
    flag = calibTem;
  }
}
//キャリブレーション結果を送信する
void sendCalibrationData(float min, float max)
{
  Serial.print("最小値:");
  Serial.print(min);
  Serial.print(",");
  Serial.print("最大値");
  Serial.println(max);
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
float getLux()
{
  int val = analogRead(A0); // A2の情報を読み取る
  float vo = changeVoltage(val);
  float lx = changeLx(vo);
  return lx;
}

//データを取得し、温度に変換する
float getTem()
{
  int val = analogRead(A1); // A1の情報を読み取る
  float vo = changeVoltage(val);
  float tem = changeTemp(vo);
  return tem;
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
//電圧を温度に変換する
float changeTemp(float volt)
{
  return (volt * 1000.0 - 600.0) / 10.0; //電圧→温度
}