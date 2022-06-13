import processing.serial.*; //Serialライブラリを読み込む
Serial port; //Serialクラスのオブジェクトを宣言

int val0,val1;

void setup(){
 size(1024,1024);
 frameRate(60);
 
 port = new Serial(this,"COM5",9600); //Serialクラスのインスタンス生成
}

void draw(){
  background(0); //背景を黒
  ellipse(width/2,height/2,val0,val1); //中心を(125,125)として幅をval0,高さをval1とする楕円を描く
  println("R"); //描画タイミング
}

//シリアルポートにデータが到着するたびに呼び出される割り込み関数
void serialEvent(Serial p){//pにはデータが到着したシリアルポートに対応するインスタンスが代入される。
  if(p.available() >= 5){ //受信バッファに到着しているデータ数が5以上なら
    if(p.read() == 'H') //読み込んだ1バイトがHなら
    {
      val0 = recvInt(p);
      val1 = recvInt(p);
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
  
