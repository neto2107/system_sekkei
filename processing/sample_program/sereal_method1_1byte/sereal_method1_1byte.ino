int val0, val1;
unsigned long int timePrev, timeNow;
void setup()
{
  Serial.begin(9600); // シリアル通信を 9600bps で初期化
  timePrev = millis(); // 経過時間の初期値
}
void loop()
{
  timeNow = millis(); // 現在の経過時間
  val0 = analogRead(0); // A0 ピンの AD 変換結果を取得 (0-1023)
  val1 = analogRead(1); // A1 ピンの AD 変換結果を取得 (0-1023)
  if ( timeNow - timePrev >= 50 ) { // 50ms ごとに実行
    val0 = val0 / 4; // 0-255 の値に変換
    val1 = val1 / 4; // 0-255 の値に変換
    Serial.write( 'H' ); // 文字 ’H’ をバイナリデータとして送信
    Serial.write(val0); // 1 バイトのバイナリデータとして値を送信
    Serial.write(val1); // 1 バイトのバイナリデータとして値を送信
    timePrev = timeNow; // 1 ステップ前の経過時間を更新
  }
}
