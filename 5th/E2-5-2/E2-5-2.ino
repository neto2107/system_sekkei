int val;


void setup(){
  Serial.begin(9600);
  val = 32;
}

void loop(){


  if(val <= 127){
    Serial.write(val);//valの値に対応したASCIIコード
    Serial.print(",");//,を送信
    Serial.println(val); //valの値を文字に変換した結果+改行
    val++;
  }


}