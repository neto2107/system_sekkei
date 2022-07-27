#include <Adafruit_TCS34725.h> //カラーセンサのライブラリ

//カラーセンサのglobal変数
Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_2_4MS, TCS34725_GAIN_60X);
float r, g, b;
unsigned int r_min, g_min, b_min; //r,g,bの最小値
unsigned int r_max, g_max, b_max; //r,g,bの最大値

void calib() {
  //rの最大値と最小値を更新
  if (r < r_min) r_min = r;
  if (r_max < r) r_max = r;
  
  //gの最大値と最小値を更新
  if (g < g_min) g_min = g;
  if (g_max < g) g_max = g;
  
  //bの最大値と最小値を更新
  if (b < b_min) b_min = b;
  if (b_max < b) b_max = b;
  
  //r,g,bの最大値と最小値を表示
  Serial.print("r_min:"); Serial.println(r_min);
  Serial.print("g_min:"); Serial.println(g_min);
  Serial.print("b_min:"); Serial.println(b_min);
  Serial.print("r_max:"); Serial.println(r_max);
  Serial.print("g_max:"); Serial.println(g_max);
  Serial.print("b_max:"); Serial.println(b_max);
  Serial.println();
  delay(1000);
}

void setup() {
  Serial.begin(9600);
  r_min = g_min = b_min = 30000; //r,g,bの最小値の初期値。適当な大きな値にしておく
  r_max = g_max = b_max = 0;     //r,g,bの最大値の初期値。適当な小さな値にしておく
  tcs.begin();
}

void loop() {
  tcs.getRGB(&r, &g, &b);
  calib();
}
