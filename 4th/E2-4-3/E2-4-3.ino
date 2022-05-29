#include <MsTimer2.h>

// Switch on LED on pin 13 each second
//使用するピンの宣言
const int LED1_PIN = 13;
const int LED2_PIN = 9;
void flash() {
  static boolean output = HIGH;
  
  digitalWrite(LED1_PIN, output);
  output = !output;
}

void setup() {
  pinMode(LED1_PIN, OUTPUT);
  pinMode( LED2_PIN, OUTPUT);

  MsTimer2::set(500, flash); // 500ms period
  MsTimer2::start();
}

void loop() {
  digitalWrite(LED2_PIN,HIGH);
  delay(500); //500ms待機
  digitalWrite(LED2_PIN,LOW);
  delay(500); //500ms待機
}
