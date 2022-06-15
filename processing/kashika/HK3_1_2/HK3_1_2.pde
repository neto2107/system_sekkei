import processing.serial.*; //Serialライブラリを読み込む
Serial port; //Serialクラスのオブジェクトを宣言
//設定温度の初期値
 int t_min = 15;
 int t_max = 35;

int val0,val1;
int[] drawPoint;//Arduinoからのデータを格納しておく関数
int indexNow;//次のdrawpointの格納位置を示す変数
float t; //現在の温度

void setup(){
 size(800,300);
 frameRate(60);
 drawPoint = new int[800];
 port = new Serial(this,"/dev/tty.usbserial-A90174GY",9600); //Serialクラスのインスタンス生成
 indexNow = 0;
}

void draw(){
  background(255); //背景を白
  drawMath();
  //属性を設定
  strokeWeight(2);
  stroke(255,0,0);
  for(int i = 1;i <= 800; i++){
    float drawY =map(drawPoint[i-1],0,t_max - t_min,300,0);
    if(t>t_max){//表示領域を超えたら
      //表示範囲を補正して再表示
      reset();
      t_min += 5;
      t_max += 5;
    }
    if(t<t_min){//表示領域を下回ったら
      //表示範囲を補正して再表示
      reset();
      t_min -= 5;
      t_max -= 5;
    }
    point(i,drawY);
  }
  stroke(0,255,0);
  strokeWeight(1);
  line(indexNow,0,indexNow,height);
  println("R"); //描画タイミング
}

//シリアルポートにデータが到着するたびに呼び出される割り込み関数
void serialEvent(Serial p){//pにはデータが到着したシリアルポートに対応するインスタンスが代入される。
  if(p.available() >= 3){ //受信バッファに到着しているデータ数が3以上なら
    if(p.read() == 'H') //読み込んだ1バイトがHなら
    {
      //drawPointに受信した値のうち170-190の値を0-1023に変換して格納する
      t = calData(recvInt(p));
      drawPoint[indexNow] = (int) map(t,t_min,t_max,0,t_max - t_min);
      println(t);
      incIndex(); //配列の現在位置を更新する
      
      p.clear(); //バッファをクリア
      println("<-"); //データ受信タイミング
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
  if(indexNow < 799){//indexNowが配列の最後まで行っていなければ
    indexNow++; //indexNowを増加させる
  }else{//indexNowが配列の最後まで到達したら、
    indexNow = 0;//indexNowを0に戻す
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
  

//罫線と対応する温度を描く
void drawMath(){
  fill(0);
  for(int i = 0;i<=t_max - t_min; i++){
    stroke(200,200,200);
    int y =int(map(i,0,t_max - t_min,300,0));
    line(0,y,800,y);
    
    
    textSize(10);
    text(t_min + i,10,y);
  }
  noFill();
}
