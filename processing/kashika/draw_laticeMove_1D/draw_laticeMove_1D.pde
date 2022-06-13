
int xn = 20; //分割数
int x;
int cn_x;

void setup() {
  size(800, 120);
  frameRate(5);
  cn_x = 0;
}

void draw() {
  background(255); //背景を白にする
  //格子を描画する
  stroke(0); //線の色を黒にする
  line(0, 0, width, 0);//(0,0)-(width,0)まで線を引く
  line(0, height, width, height);//(0,height)-(width,height)まで線を引く

  for (int i = 0; i<= xn; i++) {
    x = i * width/xn; //線のx座標
    line(x, 0, x, height);
  }

  //線の色を赤にする
  strokeWeight(3);
  stroke(255, 0, 0);
  //矩形の中に楕円を描画する
    x = width/(2*xn) + cn_x * width/xn; //楕円の中心のx座標を算出
    ellipse(x, height/2, width/xn, height);
    
    if(cn_x < xn){ //まだ円が右端まで言ってないなら
      cn_x ++;//1つ右にずらす
    }else{//まだが右端に達していたら、
      cn_x = 0;//一番左に移動する
    }

}
