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
int[] drawPoint;//Arduinoからのデータを格納しておく関数
int indexNow;//次のdrawpointの格納位置を示す変数
int drawNow;//描画位置を示す変数
float v; //現在の電圧値

Time t; //描画位置を計算するのに使う時刻
Time t2;//送信間隔を格納しておく変数


void setup(){
 size(800,300);
 frameRate(60);
 drawPoint = new int[1000];
 port = new Serial(this,"/dev/tty.usbserial-A90174GY",9600); //Serialクラスのインスタンス生成
 indexNow = 0;
 t = new Time(); //Timeクラスのインスタンスを生成
 t2 = new Time(); 
}

void draw(){
  //描画位置を演算
  calDrawNow(); //描画位置を計算
  int xpos = width * drawNow/20000;
  background(255); //背景を白
  drawMath();
  //属性を設定
  strokeWeight(2);
  stroke(255,0,0);
  //計測結果を描画
  for(int i = 1;i <= indexNow; i++){
    //描画のY座標を計算
    float drawY =map(drawPoint[i-1],v_min,v_max,300,0);
    //点を描画
    point((int)map(i,0,indexNow,0,xpos),drawY);
  }
  //属性の計算
  stroke(0,255,0);
  strokeWeight(1);
  //現在位置を描画
  line(xpos,0,xpos,height);
  println("R"); //描画タイミング
  t.updateTime(); //現在時刻をアップデート
  t2.updateTime();
  
  //送信が長期的にない時に、送信要求を送信
  if(t2.diffTime()>1000){
    sendSignal();
  }
  
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
    if(p.available() >= 2){ //受信バッファに到着しているデータ数が2以上なら
  
        //受信に成功したらタイマーをリセット
        t2.reflesh();
        int recv = recvInt(p);
        v = changeVoltage(recv);
        //drawPointに受信した値を確認する
        drawPoint[indexNow] = recv;
        
        println(recv);
        incIndex(); //配列の現在位置を更新する
        l-=2; //受信した分を減らす
        println("<-"); //データ受信タイミング
        port.write(0xff); //バイトデータを送信(1byte)
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
//indexNowを増加させる関数
void incIndex(){
  if(indexNow <999){//indexNowが配列の最後まで行っていなければ
    indexNow++;
  }else{//indexNowが配列の最後まで到達したら、
  //描画位置をリセット
    reset();
  }
}

void calDrawNow(){
  if(t.diffTime()<20000){
    drawNow=t.diffTime();
  }else{
    drawNow = 0;
    t.reflesh();
    reset();
  }
}

//表示位置などをリセットする関数
void reset(){
  indexNow = 0;
  for(int i = 0 ; i<800;i++){
    drawPoint[i] = 0;
  }
}

//センサー値を温度に変換する
float calData(int val){
  float vo = changeVoltage(val);
  float Temp = changeTemp(vo);
  return Temp;
}

//センサー値を電圧値に変換する
//v0:ベース電圧
final float v0 = 5.0;
float changeVoltage(int value){
  return v0 * (value/1024.0);
}

//電圧を温度に変換する
float changeTemp(float volt){
 return (volt * 1000.0 - 600.0)/10.0;//電圧→温度
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
    reset();
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
