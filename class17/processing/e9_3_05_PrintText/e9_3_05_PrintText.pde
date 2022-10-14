import processing.serial.*;

Serial port1;

String myString1 = null;
int LF = 10;                                                    // LF（Linefeed）のアスキーコード

void setup() {
  size(600, 200);                                               //幅 600px, 高さ 200px のウインドウを生成
  port1 = new Serial(this, "/dev/cu.usbserial-A505SYNT", 9600); //Serial クラスのインスタンスを生成
  port1.clear();
  port1.bufferUntil(0x0d);                                      // LF = 0x0d までバッファ
}

void draw() { 
  background(0);                                                 //背景色を黒に
  textSize(30);                                                  //文字の大きさを 30 に 
  textAlign(CENTER,CENTER);                                      //文字の配置をウインドウの中心に

  if(myString1 != null){
    text(myString1, 200, 100);
  }

}

// シリアルポートにデータが到着するたびに呼び出される割り込み関数
void serialEvent(Serial p) {
  if (p == port1 && (p.available() > 0)) {
    myString1 = port1.readStringUntil(LF);                      //文字データの最後まで読み込み
    myString1 = port1.readString();
    if(myString1 != null){                                      //文字が入ってたら
      myString1 = trim(myString1);                              //行末の改行 '¥n' を削除
    }
  }     

}
