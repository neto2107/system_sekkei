int cnt = 0;                              // 送信回数の確認用カウンタ
  
void setup()
{
  Serial.begin(9600);                     // シリアル通信の初期化
}

void loop()
{
  String data = "count:" + String(cnt); // 送信回数送信用の文を作成
  Serial.println(data);                 // シリアル通信により文字列送信（改行コード有り）

  cnt += 1;                             // 送信回数のカウンタをインクリメント
  //delay(50);                            // 50ms待つ
}
