import processing.serial.*; //Serialライブラリを読み込む
Serial port; //Serialクラスのオブジェクトを宣言

int val0,val1;
int[] drawPoint;//Arduinoからのデータを格納しておく関数
int indexNow;//次のdrawpointの格納位置を示す変数

void setup(){
 size(800,300);
 frameRate(60);
 drawPoint = new int[800];
 port = new Serial(this,"COM5",9600); //Serialクラスのインスタンス生成
 indexNow = 0;
}

void draw(){
  background(255); //背景を白
  //属性を設定
  strokeWeight(2);
  stroke(255,0,0);
  for(int i = 1;i <= 800; i++){
    point(i,map(drawPoint[i-1],0,1023,300,0));
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
      drawPoint[indexNow] = recvInt(p);//drawPointに受信した値を格納する
      println(drawPoint[indexNow]);
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
  
