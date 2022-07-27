// list 5.2 PWM制御
const int D6 = 6;    // PWM出力ディジタルピンの定義
float duty = 0.25;    // duty変数の定義. duty比変更

void setup() {analogWrite(D6,duty*255);}

void loop() {}
