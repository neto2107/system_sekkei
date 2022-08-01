#include <MsTimer2.h>


const int D9 = 9;
unsigned int prevTime;
unsigned int nowTime;

float top = 31250;
void initPWM1() {
// PWM freq = 16MHz / r (prescaler) / 2 (phase-correct) / (top)
// when r=256
//  1Hz -> top = 31250 (60rpm)
// 10Hz -> top = 3125 (600rpm)
TCCR1A = 0b10100000;
// TCCR1B = 0b00010001; //r=1
// TCCR1B = 0b00010010; //r=8
// TCCR1B = 0b00010011; //r=64
TCCR1B = 0b00010100;    //r=256
//TCCR1B = 0b00010101; //r=1024
ICR1 = map(1,0,10,3125,31250); //
}

void setup() {
  initPWM1();
  analogWrite(D9, 255 * 0.03);

  nowTime= millis();
  prevTime = nowTime; 
}

void loop() {
  nowTime = millis();
  if(nowTime - prevTime<4000){
    ICR1 = map(1,0,10,3125,31250);
  }else if(nowTime - prevTime<8000){
    ICR1 = map(2,0,10,3125,31250);
  }else if(nowTime - prevTime<12000){
    ICR1 = map(5,0,10,3125,31250);
  }else if(nowTime - prevTime<16000){
    ICR1 = map(10,0,10,3125,31250);
  }else{
    prevTime = nowTime;
  }  
  }
