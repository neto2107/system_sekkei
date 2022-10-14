import processing.serial.*;

Serial port1;                                                       // 1台目のZumoのシリアル通信用
Serial port2;                                                       // 2台目のZumoのシリアル通信用
Serial port3;                                                       // 2台目のZumoのシリアル通信用

String myString1 = null;
String myString2 = null;  //2つめの結果表示よう
float red=0, green=0, blue=0;
float red2 = 0, green2 = 0, blue2 = 0;
int LF = 10;     // LF（Linefeed）のアスキーコード

int moving_robot= 0;

int zumo_id = 0;

int graph_width =100;                                               // グラフの幅を定義

void setup() {
  size(100, 100);                                                  // 幅 1200px, 高さ 800px のウインドウを生成
  port1 = new Serial(this, "/dev/tty.usbserial-A901745Z", 9600);     // Serial クラスのインスタンスを生成
  //port1.clear();

  //  ＊＊＊　ヒント：ここでport2の「ポート指定，クリア，LFまでバッファ」を行う　＊＊＊//
  port2 = new Serial(this, "/dev/tty.usbserial-A400FRGN", 9600);     // Serial クラスのインスタンスを生成
  port3 = new Serial(this, "/dev/tty.usbserial-A104RFI3", 9600);     // Serial クラスのインスタンスを生成


  background(0);                                                    // 背景色を黒に
}

void draw() { 

  fill(255, 255, 255); 
  textSize(30);
  text(Integer.toString(moving_robot), 50, 50);                                      // シリアル通信で受信したテキストの表示
}

boolean sof_f = false;//SOFを発見したかどうかのフラグ
boolean sof_s = false;
int inByte;
// シリアルポートにデータが到着するたびに呼び出される割り込み関数
void serialEvent(Serial p) {
  int l = p.available();
  while (l>0) {//受信バッファないにデータがある時

    if (sof_f == false) { // SoF を発見していない場合
      if (p.read() == 'H') { // SoF:Start of Frame の検査
        sof_f = true; // SoF の発見を記録
      }
      l--;//受信バッファ数を修正
    }

    if (sof_f == true) {

      if (p == port1 && l >= 1) {
        inByte = p.read();
        println("<--");
        if (inByte == 'S') {
          port1.write(0x32);
          println("-->");
          moving_robot=1;
        }
        if (inByte == 'E') {
          port2.write(0x32);
          moving_robot=2;
                    println("-->");
        }
        sof_f = false;
      }
      if (p == port2 && l >= 1) {
        inByte = p.read();
        println("<--");
        if (inByte == 'E') {
          port3.write(0x32);
          moving_robot=3;
                    println("-->");
        }
        sof_f = false;
      }
      if (p == port3 && l >= 1) {
        inByte = p.read();
        println("<--");
        if (inByte == 'E') {
          moving_robot=0;
        }
        sof_f = false;
      }
    }
  }
}
