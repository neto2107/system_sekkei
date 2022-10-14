import processing.serial.*;

Serial port1;                                                       // 1台目のZumoのシリアル通信用
Serial port2;                                                       // 2台目のZumoのシリアル通信用

String myString1 = null;
float red=0, green=0, blue=0;
int LF = 10;                                                        // LF（Linefeed）のアスキーコード

int zumo_id = 0;

int graph_width =100;                                               // グラフの幅を定義

void setup() {
  size(1200, 800);                                                  // 幅 1200px, 高さ 800px のウインドウを生成
  port1 = new Serial(this, "/dev/tty.usbmodem1301", 9600);     // Serial クラスのインスタンスを生成
  port1.clear();
  port1.bufferUntil(0x0d);                                          // LF = 0x0d までバッファ

  // ＊＊＊　ヒント：ここでport2の「ポート指定，クリア，LFまでバッファ」を行う　＊＊＊//
  port2 = new Serial(this, "/dev/tty.usbmodem1401", 9600);     // Serial クラスのインスタンスを生成
  port2.clear();
  port2.bufferUntil(0x0d);                                          // LF = 0x0d までバッファ

  background(0);                                                    // 背景色を黒に

  fill(100, 100, 100); 
  rect(width/2, 0, width/2, height/2);         // 右上の領域を塗りつぶす
  fill(200, 200, 200); 
  rect(0, height/2, width/2, height/2);        // 左下の領域を塗りつぶす
  fill(150, 150, 150); 
  rect(width/2, height/2, width/2, height/2);  // 右下の領域を塗りつぶす
}

void draw() { 
  if (zumo_id == 1) {                                                 // データを受信したときだけ書き換える（1番目のZumo）
    fill(0, 0, 0); 
    rect(0, 0, width/2, height/2);                     // 対象画面の初期化（黒く塗りつぶす）

    // グラフなどの描画
    fill(red, 0, 0); 
    rect(10, height/2, 100, -red);
    fill(0, green, 0); 
    rect(120, height/2, 100, -green);
    fill(0, 0, blue); 
    rect(230, height/2, 100, -blue);

    fill(red, green, blue); 
    rect(350, 30, 200, 200);

    if (myString1 != null) {
      text(myString1, 10, 10);                                      // シリアル通信で受信したテキストの表示
    }
  }

  // ＊＊＊　ヒント：ここに2番目のZumo用の処理を書く　＊＊＊//
  if (zumo_id == 2) {
     pushMatrix(); //原点の情報を保存
     translate(width/2,0);
    fill(0, 0, 0); 
    rect(0, 0, width/2, height/2);                     // 対象画面の初期化（黒く塗りつぶす）

    // グラフなどの描画
    fill(red, 0, 0); 
    rect(10, height/2, 100, -red);
    fill(0, green, 0); 
    rect(120, height/2, 100, -green);
    fill(0, 0, blue); 
    rect(230, height/2, 100, -blue);

    fill(red, green, blue); 
    rect(350, 30, 200, 200);

    if (myString1 != null) {
    text(myString1, 10, 10); // シリアル通信で受信したテキストの表示
    popMatrix();//原点の情報を読み出し
    }
  }
}

// シリアルポートにデータが到着するたびに呼び出される割り込み関数
void serialEvent(Serial p) {
  if ((p == port1 || p == port2) && (p.available() > 0)) {          // 割り込みシリアル通信が，port1か，port2で，なおかつデータが入っている時
    zumo_id = 1;                                                    // データをやり取りしたロボットのIDを記憶

    if (p == port1)  zumo_id = 1;                                    // データをやり取りしたロボットのIDを記憶
    else if (p == port2) zumo_id = 2; 

    myString1 = p.readStringUntil(LF);                              //文字データの最後まで読み込み
    if (myString1 != null) {                                          //文字が入ってたら
      myString1 = trim(myString1);                                  //行末の改行 '¥n' を削除

      float data[] = float(split(myString1, ','));                  // カンマで区切られた値を分割

      // 受信した数値の数が３つで，NaN (Not a Number) ではない時
      if (data.length == 3 && data[0] != Float.NaN && data[1] != Float.NaN && data[2] != Float.NaN) {
        // キャリブレーションあり
        red = map(data[0], -5000, 1000, 0, 255);                    // 値を0〜255にマッピング
        green = map(data[1], -5000, 1000, 0, 255);                  // 値を0〜255にマッピング
        blue = map(data[2], -5000, 1000, 0, 255);                   // 値を0〜255にマッピング
      }
    }
  }
}
