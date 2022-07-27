#include "Adafruit_TCS34725.h"

Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_2_4MS, TCS34725_GAIN_60X);

#define cal_time 5000
int flg = 0;

float red=0, green=0, blue=0;
int red_c,green_c,blue_c; //キャリブレーション後変数


//キャリブレーション用変数
unsigned int r_min = 30000,g_min=30000,b_min=30000;
unsigned int r_max = 0,g_max=0,b_max=0;

//NN関数のために新たに追加する項目
#define max_colors 4
unsigned int ave_colors[max_colors][3]={
  {4,6,11},  //黒色
  {255,255,255},//白色
  {178,0,0}, //赤色
  {97,255,69} //緑色
};

void setup() {
  Serial.begin(9600);
  tcs.begin();
//
Serial.println("Start Auto Calibration for 5s");
    auto_calib();
Serial.println("End Auto Calibration");
}

// The commented out code in loop is example of getRawData with clear value.
// Processing example colorview.pde can work with this kind of data too, but It requires manual conversion to 
// [0-255] RGB value. You can still uncomments parts of colorview.pde and play with clear value.
void loop() {

  tcs.getRGB(&red, &green, &blue);
   //uint16_t red, blue, green, clr;
   //tcs.getRawData(&red, &green, &blue, &clr);
   //キャリブレーション
    map_color();
  Serial.print("R:\t"); Serial.print(red_c); 
  Serial.print("\tG:\t"); Serial.print(green_c); 
  Serial.print("\tB:\t"); Serial.print(blue_c);
  Serial.print("\n");
  Nearest_Neighbor();
  delay(100);
}

//値をマッピングをする変数
void map_color(){
   red_c = map(red,r_min,r_max,0,255);
   green_c = map(green,g_min,g_max,0,255);
   blue_c = map(blue,b_min,b_max,0,255);
}

//k近傍法による色判別
void Nearest_Neighbor(){
  int count =0;
  int color = -1;
  float minDistance = 999999;
  float distance;

  for(int i = 0; i<max_colors; i++){
    distance = sqrt((red_c - ave_colors[i][0])*(red_c - ave_colors[i][0])+
    (green_c - ave_colors[i][1])*(green_c - ave_colors[i][1])+(blue_c - ave_colors[i][2])*(blue_c - ave_colors[i][2]));

    if(distance < minDistance){
      minDistance = distance;
      color = i;
    }
  }

  //プロセッシングに判別結果を数値として送信
  Serial.print("\tJ:\t");
  Serial.println(color);
}

void auto_calib(){
  unsigned long timeInit;
  timeInit = millis();
  
  while(1){
    tcs.getRGB(&red, &green, &blue);
    if(red>r_max) r_max = red;
    if(red<r_min) r_min =red;
//    Serial.print("r_min,r_max =");
//    Serial.print(r_min);Serial.print(",");Serial.println(r_max);
    
    if(green>g_max) g_max = green;
    if(green<g_min) g_min = green;
//    Serial.print("g_min,g_max =");
//    Serial.print(g_min);Serial.print(",");Serial.println(g_max);
    
    if(blue>b_max) b_max = blue;
    if(blue<b_min) b_min =blue;
//    Serial.print("b_min,b_max =");
//    Serial.print(b_min);Serial.print(",");Serial.println(b_max);
    delay(100);
    if((millis()-timeInit)>cal_time){
      flg = 1; break;
    }
  }

}
