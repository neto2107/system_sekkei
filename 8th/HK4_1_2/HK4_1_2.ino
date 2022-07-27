/*
* 学籍番号:6120011749
* 氏名:奥井翔麻
*/
int val0, val1;
unsigned long int timePrev,timePrev2, timeNow;
byte inByte; //Processingからの送信要求を受け取る変数

const int LED_PIN = 13; //LEDピンを定義
void setup()
{
  Serial.begin(9600); //シリアル通信を9600bpsで初期化
  timePrev = millis();//時間を初期化する
  timePrev2 = millis();
  pinMode(LED_PIN, OUTPUT); //LED_PINをアウトプットに指定
}

void loop()
{
  timeNow = millis();
  val0 = analogRead(0); // A0ピンの変換結果を取得(0-1023)
  val1 = analogRead(1); //A1ピンのAD変換結果を取得(0-1023)
  if (timeNow - timePrev >= 50)
  {                  // 50msごとに実行(光センサーの計測値を送信)
    if(Serial.available()>0){//送信要求を受け取ったか確認
      inByte = Serial.read();//受信済みの信号を読み込む
      if(inByte ==0xff){//受け取った値が0xffなら（正規の送信要求なら）
        Serial.write('H');  //文字0x20をバイナリデータとして送信
        sendInt(val0);
        digitalWrite(LED_PIN, HIGH);//正常に通信が行われていることを示すLEDを点灯させる。
        timePrev = timeNow; // 1ステップ前の経過時間を更新
      }
    }
  }else if(timeNow - timePrev2 >= 200){ //200msごとに実行(温度センサーの計測値を送信)
    if(Serial.available()>0){//送信要求を受け取ったか確認
    inByte = Serial.read();//受信済みの信号を読み込む
    if(inByte ==0xff){//受け取った値が0xffなら（正規の送信要求なら）
      Serial.write('I');  //文字0x21をバイナリデータとして送信
      sendInt(val1);
      digitalWrite(LED_PIN, HIGH);//正常に通信が行われていることを示すLEDを点灯させる。
      timePrev2 = timeNow; // 1ステップ前の経過時間を更新
    }
  }
  }
  if(timeNow - timePrev>=100){ //100ms以上送信要求がなかったら、通信途絶と判断し、LEDを消灯させる
    digitalWrite(LED_PIN, LOW);
  }

  if(timeNow - timePrev >=1000 && timeNow - timePrev2>1000){ //１秒以上送信要求がないなら、データを送信
        Serial.write('H');  //文字'h'をバイナリデータとして送信
        sendInt(val0);
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