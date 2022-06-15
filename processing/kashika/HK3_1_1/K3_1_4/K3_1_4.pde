import processing.serial.*; //Serialライブラリを読み込む
Serial port; //Serialクラスのオブジェクトを宣言

int val0, val1;
Vec2 metor;//温度メーターの座標を格納しているクラス

void setup() {
  //サイズやフレームレートを設定
  size(500, 500);
  frameRate(60);

  port = new Serial(this, "/dev/tty.usbmodem11401", 9600); //Serialクラスのインスタンス生成

  metor = new Vec2(0.0, 100, 1); //極座標モードでインスタンス生成
}

void draw() {
  background(255); //背景を白
  //属性を設定
  strokeWeight(2);
  stroke(0, 0, 0);
  //円を描画する
  ellipse(200,200,200,200);
  
  //属性を指定
  stroke(0, 255, 0);
  strokeWeight(1);
  //メーターの線を引く
  line(200, 200, metor.getX()+200, (-metor.getY())+200);
  println(metor.getX());
  
  //文字を描画する
  fill(0);
  textSize(20);
  //電圧値を取得して文字列に変換する
  String t = changeVoltage(val0) + "V";
  text(t,200,400);
  noFill();
  println("R"); //描画タイミング
}



//シリアルポートにデータが到着するたびに呼び出される割り込み関数
void serialEvent(Serial p) {//pにはデータが到着したシリアルポートに対応するインスタンスが代入される。
  if (p.available() >= 3) { //受信バッファに到着しているデータ数が3以上なら
    if (p.read() == 'H') //読み込んだ1バイトがHなら
    {
      //drawPointに受信した値のうち0-1023の値を0-360に変換して格納する
      val0 = recvInt(p);
      metor.setDegree(map(val0, 0, 1023, 0, 2*PI));
      p.clear(); //バッファをクリア
      println("<-"); //データ受信タイミング
    }
  }
}
//ArduinoからInt型の値を受信する
int recvInt(Serial p) {
  int high = p.read();//上位8ビットを読み込む
  int low = p.read();//下位8ビットを読み込む
  int val = (high << 8) + low;//上位8ビットシフトして足すことで、1つの変数に戻す
  if (32767 < val)//負の値かどうかが保存されないので、該当するときは場合分け
    val -=65536;//65536 = 2^16

  return val;//得られたInt型の値を返す
}
//平面ベクトルを扱うクラス
class Vec2 {
  //直交座標系
  private int x;
  private int y;
  //極座標系
  private float rad;//角度
  private int r;//距離
  //最初に与えられた値が直交座標ならtrue,極座標ならfalse
  private boolean flag;
  //コンストラクタ
  Vec2(int x, int y) {
    this.x = x;
    this.y = y;
    this.flag = true; //極座標モード
  }
  //極座標のコンストラクタ
  //mode:0 ラジアンで角度入力,mode:1 弧度法で角度入力
  Vec2(float degree, int r) {
    this.rad = degree;
    this.r = r;
    this.flag = false; //直交座標モード
  }
  //極座標のコンストラクタ
  //mode:0 ラジアンで角度入力,mode:1 弧度法で角度入力
  Vec2(float degree, int r, int mode) {
    if (mode ==0) {
      this.rad = degree;
    } else {
      this.rad = (degree/360.0)* 2 * PI;
    }
    this.r = r;
    this.flag = false; //直交座標モード
  }

  //ゲッター
  public int getX() {
    if (flag) {
      return x;
    } else {
      //極座標を直交座標に変換して返す
      return int( this.r * sin(this.rad));
    }
  }
  public int getY() {
    if (flag) {
      return this.y;
    } else {
      //極座標を直交座標に変換して返す
      return int( this.r * cos(this.rad));
    }
  }
  //セッター
  public void setDegree(float rad) {

    this.rad = rad % (2*PI);
  }
  //modeが0の時、ラジアン,modeが１の時、弧度法
  public void setDegree(float degree, int mode) {
    if (mode == 0) {
      this.rad = degree %(2*PI);
    } else {
      this.rad = ((degree%360.0)/360.0) * 2 * PI;
    }
  }
}


//センサー値を電圧値に変換する
// v0:ベース電圧
float v0 = 5.0;
float changeVoltage(int value)
{
  return v0 * ((float)value / 1024.0);
}
