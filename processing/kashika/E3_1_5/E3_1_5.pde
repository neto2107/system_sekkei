import processing.serial.*; //Serialライブラリを読み込む
Serial port; //Serialクラスのオブジェクトを宣言

int val0,val1;

void setup(){
 size(256,256);
 frameRate(60);
 
 port = new Serial(this,"COM5",9600); //Serialクラスのインスタンス生成
}

void draw(){
  background(0); //背景を黒
  ellipse(125,125,val0,val1); //中心を(125,125)として幅をval0,高さをval1とする楕円を描く
  println("R"); //描画タイミング
}

//シリアルポートにデータが到着するたびに呼び出される割り込み関数
void serialEvent(Serial p){//pにはデータが到着したシリアルポートに対応するインスタンスが代入される。
  if(p.available() >= 3){ //受信バッファに到着しているデータ数が3以上なら
    if(p.read() == 'H') //読み込んだ1バイトがHなら
    {
      val0 = p.read(); //受信バッファから1バイト読み込み
      val1 = p.read(); 
      p.clear(); //バッファをクリア
      println("<-"); //データ受信タイミング
    }
    
  }
}
  
