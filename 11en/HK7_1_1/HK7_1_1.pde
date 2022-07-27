final int AMOUNT = 8;

int x[][]= {{4,6,11},  //黒色
  {255,255,255},//白色
  {178,0,0}, //赤
  {97,255,69}, //緑
  {0,16,121}, //青
  {135,255,255},//シアン
  {188,49,126},//マゼンタ
  {255,255,103}}; //イエロー

void setup() {
  size(500, 500, P3D);
  background(100);
  noFill();
  stroke(255);
  strokeWeight(2);
}

void draw() {
  //背景色描画
  background(100);
  //中心を決める
  translate(width/2, height/2);
  //時間経過で回転させる
  rotateY(frameCount / 200.0);
  //線の太さを１に
  strokeWeight(1);
  //色を白にする
  stroke(255);
  //ボックスを描画する
  box(300);
  //太さを20にする
  strokeWeight(20);
  //色の位置の3次元描画
  for (int i = 0; i < AMOUNT; i++) {
    //点の色を同じ色に変える
    stroke(x[i][0],x[i][1],x[i][2]);
    //点を描画する
    point(x[i][0]-122, x[i][1]-122, x[i][2]-122);
  }
}
