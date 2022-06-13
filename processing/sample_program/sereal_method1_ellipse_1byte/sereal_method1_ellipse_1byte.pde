import processing.serial.*; // Serial ライブラリを取り込む
Serial port; // Serial クラスのオブジェクトを宣言
int val0, val1;
void setup()
{
  size(256, 256); // サイズ 256 × 256 のウィンドウ生成
  port = new Serial(this, "/dev/ttyACM0", 9600);//Serial クラスのインスタンス生成
}
void draw()
{
  background(0); // 背景を黒
  ellipse(125, 125, val0, val1); // 中心 (125,125) ,横幅 val0 ,縦幅 val1 の楕円を描
    println("R"); // 描画タイミング(確認用)
}
// シリアルポートにデータが到着するたびに呼び出される割り込み関数
void serialEvent(Serial p) { // p にはデータが到着したシリアルポートに対応するイン
    if ( p.available() >= 3 ) { // 受信バッファに到着しているデータ数が 3 以上なら
    if ( p.read() == 'H' ) { // 読み込んだ 1 バイトが文字 ’H’ なら
      val0 = p.read(); // 受信バッファから 1 バイト読み込み
      val1 = p.read(); // 受信バッファから 1 バイト読み込み
      p.clear(); // 念のため受信バッファをクリア
      println("<-");
      // データ受信タイミング(確認用)
    }
  }
}
