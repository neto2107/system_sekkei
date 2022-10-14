import processing.serial.*;

Serial port1;

String myString1 = null;
float red=0, green=0, blue=0;
int LF = 10;                                                      // LF（Linefeed）のアスキーコード

void setup() {
  size(640, 480);                                                 // 幅 640px, 高さ 480px のウインドウを生成
  port1 = new Serial(this, "/dev/cu.usbserial-A505SYNT", 9600);   // Serial クラスのインスタンスを生成
  port1.clear();
  port1.bufferUntil(0x0d);                                        // LF = 0x0d までバッファ
}

void draw() { 
  background(0);                                                  // 背景色を黒に
  textAlign(CENTER,CENTER);                                       // 文字の配置をウインドウの中心に

  // グラフの表示．　色の値をy軸の向きに逆に取ることで，上向きに上下するグラフを描画している
  fill(red,0,0); rect(10,470,100,-red);                           // 赤色のグラフ
  fill(0,green,0); rect(120,470,100,-green);                      // 緑色のグラフ
  fill(0,0,blue); rect(230,470,100,-blue);                        // 青色のグラフ
  fill(red, green, blue); rect(360,110,260,260);                  // RGB値を用いた四角描画

  // RGB値を，テキストで画面に表示（グラフの上）
  fill(255);
  text("red=",20,10); text(int(red),40,10);
  text("green=",130,10); text(int(green),160,10);
  text("blue=",240,10); text(int(blue),265,10);

  if(myString1 != null){
    text(myString1, 200, 100);                                    // シリアル通信で受信したテキストの表示
  }
}

// シリアルポートにデータが到着するたびに呼び出される割り込み関数
void serialEvent(Serial p) {
  if (p == port1 && (p.available() > 0)) {
    myString1 = port1.readStringUntil(LF);                        // 文字データの最後まで読み込み
    myString1 = port1.readString();
    if(myString1 != null){                                        // 文字が入ってたら
      myString1 = trim(myString1);                                // 行末の改行 '¥n' を削除
 
      float data[] = float(split(myString1, ','));                // カンマで区切られた値を分割
      if (data.length == 3){                                      // 受信した数値の数が３つだったら
        // キャリブレーションあり
        red = map(data[0], -5000, 5000, 0, 255);                  // 値を0〜255にマッピング
        green = map(data[1], -5000, 5000, 0, 255);                // 値を0〜255にマッピング
        blue = map(data[2], -5000, 5000, 0, 255);                 // 値を0〜255にマッピング
      }
    }
  }     
}
