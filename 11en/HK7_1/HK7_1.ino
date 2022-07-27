#include "Adafruit_TCS34725.h"

Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_2_4MS, TCS34725_GAIN_60X);

#define cal_time 5000
int flg = 0;
String colorList[] = {"Black","White","Red","Green","Blue","Cyan","Mazenta","Yello"};

float red=0, green=0, blue=0;
int red_c,green_c,blue_c; //キャリブレーション後変数


//キャリブレーション用変数
unsigned int r_min = 30000,g_min=30000,b_min=30000;
unsigned int r_max = 0,g_max=0,b_max=0;

//NN関数のために新たに追加する項目
#define max_colors 8
unsigned int ave_colors[max_colors][3]={
  {4,6,11},  //黒色
  {255,255,255},//白色
  {178,0,0}, //赤
  {97,255,69}, //緑
  {0,16,121}, //青
  {135,255,255},//シアン
  {188,49,126},//マゼンタ
  {255,255,103} //イエロー

};

void setup() {
  Serial.begin(9600);
  tcs.begin();
//
Serial.println("Start Auto Calibration for 5s");
    auto_calib();
Serial.println("End Auto Calibration");
//近傍法の平均を自動で出す。
    auto_avg();
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

  switch(color){
    case 0:
      Serial.println("black");
      break;
    case 1:
      Serial.println("White");
      break;
    case 2:
      Serial.println("Red");
      break;
    case 3:
      Serial.println("Green");
      break;
    case 4:
      Serial.println("Blue");
      break;
    case 5:
      Serial.println("Cyan");
      break;
    case 6:
      Serial.println("Magenta");
      break;
    case 7:
      Serial.println("Yellow");
      break;
    default:
      Serial.println("not detected");
      break;
  }
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

void auto_avg(){
  Serial.println("start_auto_avarege");
  for(int i = 0; i < max_colors; i++){
    Serial.print("start");Serial.println(colorList[i]);
    int tmp[3][3];
    for(int j = 0; j <3 ; j++){
      delay(3000); //2秒待機
      Serial.println("get_data");
      //値の読み取り
      tcs.getRGB(&red, &green, &blue);
      map_color();
      tmp[j][0] = red_c;
      tmp[j][1] = green_c;
      tmp[j][2] = blue_c; 
    }
    Serial.print("cal_average");
    int sum[3]{0,0,0};
    for(int k = 0; k < 3; k++){
      //合計値を計算
      sum[0]+= tmp[k][0];
      sum[1]+= tmp[k][1];
      sum[2]+= tmp[k][2];
    }
    //平均を求めて代入
    ave_colors[i][0] = sum[0]/3;
    ave_colors[i][1] = sum[1]/3;
    ave_colors[i][2] = sum[2]/3;
    //色ごとの結果を表示
      Serial.print("end");Serial.print(colorList[i]);
      Serial.print("(R,G,B)");
      Serial.print(ave_colors[i][0]);Serial.print(",");
      Serial.print(ave_colors[i][1]);Serial.print(",");
      Serial.print(ave_colors[i][2]);Serial.println("");
  }
   Serial.println("end_auto_avarege");
}
