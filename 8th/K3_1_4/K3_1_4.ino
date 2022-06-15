int val0;
unsigned long int timePrev, timeNow;

void setup()
{
  Serial.begin(9600); //シリアル通信を9600bpsで初期化
  timePrev = millis();
}

void loop()
{
  timeNow = millis();
  val0 = analogRead(0); // A0ピンの変換結果を取得(0-1023)

  if (timeNow - timePrev >= 16)
  {                  // 16msごとに実行

    Serial.write('H');  //文字'h'をバイナリデータとして送信
    sendInt(val0);
  
    timePrev = timeNow; // 1ステップ前の経過時間を更新
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