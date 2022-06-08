byte birthday[2] = {0, 0};
const char zero = 48;          // 0のASCII文字コード
const char slash = 47;         // /の文字コード
unsigned long preTime = -4000; //読み込み終了までの待ち時間（最初の実行時に実行されない様にする)
void setup()
{

  Serial.begin(9600); //シリアル通信開始と転送レート指定
  //そのまま始めると文字化けするので対策
  Serial.println();
}

boolean readAfterFlag = false;
void loop()
{
  unsigned long nowTime = millis();
  first_message();
  if (Serial.available() > 0)
  {
    int recv = Serial.read(); //受信データの読み込み
    read_birthday(recv);
    preTime = nowTime;
  }
  else if (nowTime - preTime >= 2000 && nowTime - preTime <= 3000)
  { //受信が終わるまでの時間待機
    set_birthday();
    Serial.print(birthday[0]);
    Serial.print("月");
    Serial.print(birthday[1]);
    Serial.println("日");
    Serial.print("加算した値:");
    Serial.println(birthday[0] + birthday[1]);
    reset_flag();
  }

  // Serial.println(Serial.available());
}

//最初だけ実行する処理
boolean first_flag = true;
void first_message()
{
  if (first_flag)
  {
    Serial.println("誕生日を入力してください。");
    first_flag = false;
  }
}

//日を読み込むときのフラグ
byte counter = 0;
//数値を一時的に格納しておく配列
byte cache[4] = {0, 0, 0, 0};
//誕生日を読み込みキャッシュに格納
void read_birthday(int recv)
{
  switch (checkRecv(recv))
  {
  case 0:
    cache[counter] = recv - zero;
    counter++;
    break;
  case 1:
    counter = 2;
  default:
    break;
  }
}
// cacheの値を誕生日に変換
void set_birthday()
{
  //変換しやすい様にcache[1]が0の時はcache[0]の値を
  //シフトしてくる
  if (cache[1] == 0)
  {
    cache[1] = cache[0];
    cache[0] = 0;
  }
  //変換しやすい様にcache[3]が0の時はcache[2]の値を
  //シフトしてくる
  if (cache[3] == 0)
  {
    cache[3] = cache[2];
    cache[2] = 0;
  }
  // cacheを誕生日に変換
  birthday[0] = cache[1] + cache[0] * 10;
  birthday[1] = cache[3] + cache[2] * 10;

  //誕生日が正常な値かを検査
  if (!(birthday[0] > 0 && birthday[0] <= 12))
  {
    Serial.println("誕生月が不正です");
    reset_flag();
  }
  //日についてはメモリ削減のため簡易検査
  if (!(birthday[1] > 0) && birthday[1] <= 31)
  {
    Serial.println("誕生日が不正です");
    reset_flag();
  }
}

//受信した値が適切な値かどうかを調べる
//戻り値が0⇨数値　戻り値が1⇨/ 戻り値が-1 不正な値
int checkRecv(int recv)
{
  if (recv >= zero && recv <= zero + 9)
  {
    return 0;
  }
  else if (recv == slash)
  {
    return 1;
  }
  else
  {
    Serial.write("不正な値が入力されました。");
    reset_flag();
    return -1;
  }
}

//フラグのリセット、再度受付可能な状態にする
void reset_flag()
{
  first_flag = true;
  readAfterFlag = false;
  counter = 0;
  preTime = -4000;
  for (int i = 0; i < 4; i++)
  {
    cache[i] = 0;
  }
  for (int j = 0; j < 2; j++)
  {
    birthday[j] = 0;
  }
  //受信バッファをクリアする
  while (Serial.available())
  {
    Serial.read();
  }
}