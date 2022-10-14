#include <Pushbutton.h>               // ボタンライブラリの読み込み
#include <ZumoBuzzer.h>               // ブザーライブラリの読み込み

const int buzzerPin = 3;              // ブザーは 3 番ピン
const int ledPin = 13;                // LED は 13 番ピン

Pushbutton button(ZUMO_BUTTON);       // Pushbutton クラスのインスタンスを生成
ZumoBuzzer buzzer;                    // ZumoBuzzer クラスのインスタンスを生成

void setup() {
  pinMode(ledPin, OUTPUT);            // 13 番ピンを出力モードに設定
  button.waitForButton();             // ユーザボタンが押されるまで待機
  buzzer.play("L16 cdegreg4");        // ブザーにて音楽を鳴らす
  delay(500);                         // 500ms待つ
}

void loop() {
  if (button.isPressed() == true) {   // ボタンが押されている状態なら
    digitalWrite(ledPin, HIGH);       // LED を点灯
    buzzer.play(">>a32");             // 音を鳴らす
  } else {                            // ボタンが押されていなかったら
    digitalWrite(ledPin, LOW);        // LED を消灯
  }  
}
