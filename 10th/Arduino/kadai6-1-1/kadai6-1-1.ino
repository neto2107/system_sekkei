// 課題6.1.1 I-P制御
#include <MsTimer2.h>

const int D2 = 2;
const int D3 = 3;
const int D6 = 6;    // PWM出力ディジタルピンの定義
float refH = 3000.0;    // 目標回転速度1
float refL = 2000.0;    //目標回転速度2
float U0 = 0.2;         //動作店をduty比0.2近傍
float Y0 = 2300.0;    
float ref = refL;       //現時点での目標回転速度
float duty = 0.0;
int Ts = 40;  //[msec]
boolean isHigh=false;
unsigned long timen;//モーターの読み取り時刻
unsigned long timep=0;
unsigned long timeln;//LEDの点灯時刻
int iteration = 0;
volatile float motrpm = 0;
volatile float motrpmf = 0;
volatile boolean isD2High = false;
volatile boolean isD3High = false;

float sum_e = 0;
float Kp = 0.00006*1.0; //自身で求めた値に変更
float Ki = 0.00011*((float)Ts/1000.0)*0.5; //自身で求めた値に変更

//モーターの入力を検出する
void countUp() {
  timen = millis();
  if ((isD2High)&&(digitalRead(D2)==LOW)) {
    motrpm = 1000.0*60.0/(float)(timen - timep);
    motrpmf = 0.75*motrpmf+0.25*motrpm;
    timep = timen;
    isD2High = false;
  } else if ((!isD2High)&&(digitalRead(D2)==HIGH)) {
    isD2High = true;
  }
}

void countUp2(){
  timeln = millis();
  if((isD3High)&&(digitalRead(D3)==LOW)){
    isD3High = false;
  }else if((!isD3High)&&(digitalRead(D3)==HIGH)){
    isD3High = true;
  }
}

void sendUnsigned(unsigned x) {
  Serial.write(x>>8);   // send High
  Serial.write(x&255);  // send Low
}

void control() {
  float e;
  
  iteration++;
  if (iteration>=100) {
    iteration=0;
    if (isHigh) {
      isHigh=false;
      ref = refL;
    } else {
      isHigh=true;
      ref =  refH;
    }
  }
  e = ref-motrpmf;
  sum_e += e;
  duty = Kp*(Y0-motrpmf)+Ki*sum_e+U0;
  if (duty<0) {
    duty = 0.0;
  }
  if (duty>0.5) {
    duty = 0.5;
  }
  analogWrite(D6,duty*255.0);
//  unsigned int x1 = (unsigned int)motrpmf;
//  unsigned int x2 = (unsigned int)(duty*255.0);
//  unsigned int x3 = (unsigned int)ref;
//  Serial.write('H');
//  sendUnsigned(x1);
//  sendUnsigned(x2);
//  sendUnsigned(x3);
    Serial.println(timeln-timen);
}

void setup() {
  analogWrite(D6,0);
  timep = millis();
  attachInterrupt(0,countUp,CHANGE);
  attachInterrupt(1,countUp2,CHANGE);
  MsTimer2::set(Ts, control); // 40ms period
  MsTimer2::start();  
  Serial.begin(115200);
}

void loop() {}
