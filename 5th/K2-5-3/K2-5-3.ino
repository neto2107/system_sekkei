const int index_max=10; //記憶しておく値の個数
float array_lux[index_max];//照度を記録しておく配列
float sum_lux; //照度の平均値
void setup()
{
  //シリアル通信を開始
  Serial.begin(9600);
}

void loop()
{
  
}

void shift_array(float array[],int index_max){
  for(int i = 0; i<index_max; i++){
    array[i]=array[i+1]; //１つ後ろから前方にシフト
  }
}

//データ
float getData(){
  int val = analogRead(A0);
  float vo= changeVoltage(val);
  float lx = changeLx(vo);
  return lx;
}

float sendData(float lx){
  Serial.println(lx);

}

//センサー値を電圧値に変換する
//v0:ベース電圧
float v0 = 5.0;
float changeVoltage(int value){
  return v0 * (value/1024.0);
}
//電圧から照度を求める
float changeLx(int voltage){
  return 222 * voltage;
}