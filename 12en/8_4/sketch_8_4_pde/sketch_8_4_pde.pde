import processing.serial.*; //Serialライブラリを読み込む
Serial port; //Serialクラスのオブジェクトを宣言

int v0; //センサーの値
PFont font;

void setup(){
  size(600,200);
   port = new Serial(this,"COM5",9600); //Serialクラスのインスタンス生成
    
    //フォントを読み込む
   font = createFont("Arial",30);


}
  

void draw(){
  //基準座標を設定
  translate(0,100);
  background(255);
  //文字の描画
   textSize(20);
   textFont(font,50);
   fill(0);
   text(v0,100,0);
   //矩形の描画
   fill(0,0,255);
   rect(200,-40,v0*10,40);
   
  println("R"); //描画タイミング
}

//シリアルポートにデータが到着するたびに呼び出される割り込み関数
void serialEvent(Serial p){//pにはデータが到着したシリアルポートに対応するインスタンスが代入される。
  if(p.available() >= 3){ //受信バッファに到着しているデータ数が3以上なら
    if(p.read() == 'H') //読み込んだ1バイトがHなら
    {
      //受信した値を格納
      v0 = recvInt(p);
      println(v0);
      //値が異常に大きくならないようにする
      if(v0>30){
        v0=30;
      }
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
