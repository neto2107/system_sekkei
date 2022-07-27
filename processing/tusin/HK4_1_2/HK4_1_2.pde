/*
* 学籍番号:6120011749
* 氏名:奥井翔麻
*/
import processing.serial.*; //Serialライブラリを読み込む
Serial port; //Serialクラスのオブジェクトを宣言
//設定温度の初期値
 int v_min = 0;
 int v_max = 1023;
 
 final int temp_max = 200;//温度センサーの最大値(AD変換値)
 final int temp_min = 170;//温度センサーの最小値(AD変換値)

int val0,val1;
int[] drawPoint;//Arduinoからのデータを格納しておく変数
int [] drawPoint2;//Arduinoからのデータを格納しておく変数
int drawNow;//描画位置を示す変数
float v; //現在の電圧値

Time t; //描画位置を計算するのに使う時刻
int xpos;

void setup(){
 size(800,300);
 frameRate(60);
 //配列を初期化
 drawPoint = new int[800];
 drawPoint2 = new int[800];
 port = new Serial(this,"/dev/tty.usbserial-A90174GY",9600); //Serialクラスのインスタンス生成

 t = new Time(); //Timeクラスのインスタンスを生成

}

void draw(){
  //描画位置を演算
  calDrawNow(); //描画位置を計算
  xpos = width * drawNow/20000;
   background(255); //背景を白
  drawMath();
  //属性を設定
  strokeWeight(2);
  stroke(255,0,0);
  //計測結果を描画
  for(int i = 1;i <= 800; i++){
    //描画のY座標を計算
    stroke(255,0,0);
    float drawY =map(drawPoint[i-1],v_min,v_max,300,0);
    //点を描画(照度センサ
    point(i,drawY);
    //描画する色を変更
    stroke(0,0,255);
    //温度センサーの値を描画
    float drawY2 =map(drawPoint2[i-1],v_min,v_max,300,0);
    point(i,drawY2);
   
  }
  //属性の計算
  stroke(0,255,0);
  strokeWeight(1);
  //現在位置を描画
  line(xpos,0,xpos,height);
  //println("R"); //描画タイミング
  t.updateTime(); //現在時刻をアップデート
  
}
int sof_f = 0;//SOFを発見したかどうかのフラグ
//シリアルポートにデータが到着するたびに呼び出される割り込み関数
void serialEvent(Serial p){//pにはデータが到着したシリアルポートに対応するインスタンスが代入される。
  int l = p.available();//受信バッファないのデータ数を取得
  while(l>0){//受信バッファないにデータがある時
    if (sof_f == 0) { // SoF を発見していない場合
      int sof = p.read();
      if (sof == 'H') { // SoF:Start of Frame の検査
        sof_f = 1; // SoF の発見を記録('H'(照度センサーの値を受け取り):1
      }else if(sof == 'I'){
        sof_f = 2; //'I'(温度センサーの値を受け取り):2
      }
      l--;//受信バッファ数を修正
    }
  
  if(sof_f == 1){//照度センサーの値読み取り
    if(l >= 2){ //受信バッファに到着しているデータ数が3以上なら
        //受信したデータを格納(照度センサー)
        int recv = recvInt(p);
        
        //drawPointに受信した値を確認する
        drawPoint[xpos] = recv;//配列に受け取った値を入れる
        //print(recv); //デバッグ様
        //println(recv2);
        
        l-=2; //受信した分を減らす
        println("<-H"); //データ受信タイミング
        port.write(0xff); //バイトデータを送信(1byte)
        sof_f=0;
      }else{
        break;
      }
  }else if(sof_f ==2){//温度センサーの値読み取り
      if(l >= 2){ //受信バッファに到着しているデータ数が3以上なら
        //受信したデータを格納(温度センサー)
        int recv2 = recvInt(p);
        //drawPointに受信した値を確認する
        drawPoint2[xpos] =(int) map(recv2,temp_min,temp_max,0.0,1024.0);//変化がわかりやすい様に値を調整
        //print(recv); //デバッグ様
        //println(recv2);
        
        l-=2; //受信した分を減らす
        println("<-I"); //データ受信タイミング
        port.write(0xff); //バイトデータを送信(1byte)
        sof_f=0;
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
//20秒経過したら表示をクリアする
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
  for(int i = 0 ; i<800;i++){
    drawPoint[i] = 0;
    drawPoint2[i] = 0;
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
