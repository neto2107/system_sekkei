/*
* 学籍番号:6120011749
* 氏名:奥井翔麻
*/
import processing.serial.*; //Serialライブラリを読み込む
Serial port; //Serialクラスのオブジェクトを宣言
//設定温度の初期値
 int v_min = 0;
 int v_max = 1023;

int val0,val1;
int drawNow;//描画位置を示す変数

boolean startFlag;//通信の開始を管理するフラグ

float v; //現在の電圧値
FilePrinter printer;

Illuminance lux; //照度センサーの値を扱うクラス

Time t; //描画位置を計算するのに使う時刻
int xpos;

void setup(){
 size(800,300);
 frameRate(60);
 port = new Serial(this,"/dev/tty.usbserial-A90174GY",9600); //Serialクラスのインスタンス生成
 printer = new FilePrinter("deta.csv","経過時間","照度センサーの値");//FilePrinerクラスのインスタンス生成
 lux = new Illuminance(0); //Illuminanceクラスのインスタンス生成
 startFlag = false; //startFlagの初期化
 t = new Time(); //Timeクラスのインスタンスを生成

}

void draw(){
  //描画位置を演算
  
  
  //20秒経過したら
  if(t.diffTime()>20000){
    //内容を書き込んで終了
    printer.close();
    println("書き込み完了");
    
  }
  //println("R"); //描画タイミング
  t.updateTime(); //現在時刻をアップデート
  
}
boolean sof_f = false;//SOFを発見したかどうかのフラグ
//シリアルポートにデータが到着するたびに呼び出される割り込み関数
void serialEvent(Serial p){//pにはデータが到着したシリアルポートに対応するインスタンスが代入される。

  int l = p.available();//受信バッファないのデータ数を取得
  
  while(l>0){//受信バッファないにデータがある時
    if (sof_f == false) { // SoF を発見していない場合
      if (p.read() == 'H') { // SoF:Start of Frame の検査
        sof_f = true; // SoF の発見を記録
      }
      l--;//受信バッファ数を修正
    }
  
  if(sof_f == true){
    if(l >= 2){ //受信バッファに到着しているデータ数が2以上なら
    
        //通信を開始した時に初期化する
        if(startFlag == false){
          startFlag = true;
          t.reflesh();//経過時間をリセット
        }
        //受信したデータを格納
        int recv = recvInt(p);
        lux.setVal(recv);//受信した値をluxに格納する
        t.updateTime();
        if(t.diffTime() < 20000 && startFlag ==true){//経過時間が20秒いないなら
          printer.printFile(t.diffTime(),recv);//時刻とセンサー値をファイルに書き込む
          println("<-"); //データ受信タイミング
          println(recv);//drawPointに受信した値を確認する
        }
        
        l-=2; //受信した分を減らす
        p.clear();

        port.write(0xff); //バイトデータを送信(1byte)
        sof_f=false;
      }else{
        break;
      }
  }
  }
}
//ArduinoからInt型の値を受信する
int recvInt(Serial p){
  int high = p.read();//上位8ビットを読み込む
  int low = p.read();//下位8ビットを読み込む
  int val = (high << 8) + low;//上位8ビットシフトして足すことで、1つの変数に戻す
  if(32767 < val)//負の値かどうかが保存されないので、該当するときは場合分け
    val -=65536;//65536 = 2^16
    
  return val;//得られたInt型の値を返す
}


//罫線を描く
void drawMath(){
  fill(0);
  for(int i = 0;i<=v_max - v_min; i+=100){
    stroke(200,200,200);
    int y =int(map(i,0,v_max - v_min,300,0));
    line(0,y,800,y);
    
  }
  noFill();
}

//マウスボタンが押されたら呼び出される割り込み関数
void mousePressed(){
  sendSignal();
}

//Arduinoに送信要求を送ると共に、描画を初期化する
void sendSignal(){
    port.write(0xff);//送信要求を送信
  //初期化
    drawNow = 0;
    t.reflesh();
  println("->");//データ送信タイミング
}

//時間を扱うクラス
class Time{
  //クラスフィールド
  private int nowTime;
  private int prevTime;
  //クラスメソッド
  //コンストラクタ
  public Time(){
    //nowTimeとprevTimeを初期化
    this.nowTime = millis();
    this.prevTime = this.nowTime;
  }
  //経過時間をリセットする
  public void reflesh(){
    this.updateTime();
    this.prevTime = this.nowTime;
  }
  //現在時刻をアップデートする
  public void updateTime(){
    this.nowTime = millis();
  }
  //経過時間を返す
  public int diffTime(){
    return this.nowTime - this.prevTime;
  }
}
//ファイルの書き込みを担うクラス
class FilePrinter{
  private PrintWriter output;
  private int indexNum; //何列あるか
  //コンストラクタ
  FilePrinter(String fileName,String index){
    this.output = createWriter(fileName);//出力先ファイルの指定
    output.println(index);
    indexNum = 1; //１列
  }
  //コンストラクタ(値が2つ)
  FilePrinter(String fileName,String index,String index2){
    this.output = createWriter(fileName);//出力先ファイルの指定
    output.print(index);
    output.print(",");
    output.println(index2);
    indexNum = 2;//2列
  }
  //値をファイルに書き込み(1変数)
  public void printFile(int val){
    if(indexNum == 1){//値がの時は実行
    output.println(val);
    }else{
      println("値の数が間違っています");//値の数が異なっている時はエラーを返す
    }
  }
  //値をファイルに書き込み(2変数)
  public void printFile(int val,int val2){
    if(indexNum == 2){//値の数があっている時は実行
    output.print(val);
    output.print(",");
    output.println(val2);
    }else{
      println("値の数が間違っています");//値の数が異なっている場合はエラーを返す
    }
  }
  
  //終了関数
  public void close(){
    output.flush();
    output.close();
  }
}
