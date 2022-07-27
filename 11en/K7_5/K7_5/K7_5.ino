#include "Adafruit_TCS34725.h"

Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_2_4MS, TCS34725_GAIN_60X);

#define cal_time 5000
int flg = 0;

float red=0, green=0, blue=0;


//キャリブレーション用変数
unsigned int r_min = 30000,g_min=30000,b_min=30000;
unsigned int r_max = 0,g_max=0,b_max=0;

void setup() {
  Serial.begin(9600);
  tcs.begin();
//Serial.println("Start Auto Calibration for 5s");
    auto_calib();
//Serial.println("End Auto Calibration");
Serial.println("R,G,B");
}

// The commented out code in loop is example of getRawData with clear value.
// Processing example colorview.pde can work with this kind of data too, but It requires manual conversion to 
// [0-255] RGB value. You can still uncomments parts of colorview.pde and play with clear value.
void loop() {
  float red_c,green_c,blue_c; //キャリブレーション後変数
  tcs.getRGB(&red, &green, &blue);
   //uint16_t red, blue, green, clr;
   //tcs.getRawData(&red, &green, &blue, &clr);
   //キャリブレーション
   red_c = map(red,r_min,r_max,0,255);
   green_c = map(green,g_min,g_max,0,255);
   blue_c = map(blue,b_min,b_max,0,255);
   Serial.print(int(red_c)); Serial.print(",");
   Serial.print(int(green_c)); Serial.print(",");
    Serial.print(int(blue_c));
    Serial.print("\n");
  delay(100);
  if(red_c > 240 && green_c>240 && blue_c>240){//白
    Serial.println("White");
  }else if(red_c <20 && green_c<20 && blue_c<20){//黒
    Serial.println("Black"); 
  }else if(red_c <180 && red_c  > 140 && green_c<70 && green_c > 50 && blue_c<70 && blue_c > 50){//赤
    Serial.println("Red");

  }else if(red_c <80 && red_c  > 40 && green_c<180 && green_c > 140 && blue_c<100 && blue_c > 60){//緑
    Serial.println("Green");

  }else if(red_c <40 && red_c  > 0 && green_c<70 && green_c > 50 && blue_c<180 && blue_c > 130){ //青
    Serial.println("Blue");
  }else if(red_c <270 && red_c  > 210 && green_c<270 && green_c > 210 && blue_c<130 && blue_c > 90){//イエロー
    Serial.println("Yellow");
  }else if (red_c <160 && red_c  > 120 && green_c<50 && green_c > 10 && blue_c<100 && blue_c > 70){//マゼンタ
    Serial.println("マゼンタ");
  }else if(red_c <40 && red_c  > 0 && green_c<150 && green_c > 110 && blue_c<220 && blue_c > 180){
    Serial.println("シアン");
  }else{
    Serial.println("Others");
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
