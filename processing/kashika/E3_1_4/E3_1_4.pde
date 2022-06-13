int xn = 20; //分割数
int x;

size(800, 120);
background(255); //背景を白にする

stroke(0); //線の色を黒にする
line(0,0,width,0);//(0,0)-(width,0)まで線を引く
line(0,height,width,height);//(0,height)-(width,height)まで線を引く

//格子を描画する
for(int i = 0; i<= xn;i++){
  x = i * width/xn; //線のx座標
  line(x,0,x,height);
  
}

//線の色を赤にする
stroke(255,0,0);
//矩形の中に楕円を描画する
for( int j = 0; j<= xn; j++){
  x = width/(2*xn) + j * width/xn; //楕円の中心のx座標を算出
  ellipse(x,height/2,width/xn,height);
}
