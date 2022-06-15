int val0, val1;
unsigned long int timePrev, timeNow;
byte inByte; //Processingからの送信要求を受け取る変数

void setup()
{
  Serial.begin(9600); //シリアル通信を9600bpsで初期化
  timePrev = millis();
}

void loop()
{
  timeNow = millis();
  val0 = analogRead(0); // A0ピンの変換結果を取得(0-1023)
  val1 = analogRead(1); // A1ピンのAD変換結果を取得

  if (timeNow - timePrev >= 16)
  {                  // 16msごとに実行
    if(Serial.available()>0){//送信要求を受け取ったか確認
      inByte = Serial.read();//受信済みの信号を読み込む
      if(inByte ==0xff){//受け取った値が0xffなら（正規の送信要求なら）
        Serial.write('H');  //文字'h'をバイナリデータとして送信
        sendInt(val0);
      
        timePrev = timeNow; // 1ステップ前の経過時間を更新
      }
    }
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