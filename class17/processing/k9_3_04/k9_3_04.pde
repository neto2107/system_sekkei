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
  //白で現在の状態を表示
  fill(255, 255, 255); 
  textSize(30);
  text(Integer.toString(moving_robot), 50, 50);                                      // 現在動いているロボット番号を表示
}

boolean sof_f = false;//SOFを発見したかどうかのフラグ
int inByte;
// シリアルポートにデータが到着するたびに呼び出される割り込み関数
void serialEvent(Serial p) {
  int l = p.available(); //pにどれだけのデータがあるかを格納
  while (l>0) {//受信バッファないにデータがある時

    if (sof_f == false) { // SoF を発見していない場合
      if (p.read() == 'H') { // SoF:Start of Frame の検査
        sof_f = true; // SoF の発見を記録
      }
      l--;//受信バッファ数を修正
    }
    //SOFを発見した場合
    if (sof_f == true) {
      //port1なら
      if (p == port1 && l >= 1) {
        //受信データを読み込む
        inByte = p.read();
        println("<--");
        //受信データがロボットの準備完了を表すものなら、
        if (inByte == 'S') {
          //ロボット１に動くように指示する
          port1.write(0x32);
          println("-->");
          //現在ロボットが動いていることをセット
          moving_robot=1;
        }
        //受信データがロボットの動作完了を表すものなら、
        if (inByte == 'E') {
          //次のロボットに動くように指示する
          port2.write(0x32);
          //動いているロボットの情報を更新
          moving_robot=2;
                    println("-->");
        }
        sof_f = false; //sofをfalseに戻す
      }
      //受信したポートが2なら
      if (p == port2 && l >= 1) {
        //データを読み込む
        inByte = p.read();
        println("<--");
        //受信したデータが動作完了を表すものなら、
        if (inByte == 'E') {
          //次のロボットにうごくように指示する
          port3.write(0x32);
          //動いているロボットの状態を変更する
          moving_robot=3;
                    println("-->");
        }
        //sofをfalseにセット
        sof_f = false;
      }
      //受信したportが3なら
      if (p == port3 && l >= 1) {
        //データを読み込む
        inByte = p.read();
        println("<--");
        //受信したデータが、ロボットの動作完了を表すものなら、
        if (inByte == 'E') {
          //動作ロボットをリセット
          moving_robot=0;
        }
        //sofをfalseに戻す。
        sof_f = false;
      }
    }
  }
}
