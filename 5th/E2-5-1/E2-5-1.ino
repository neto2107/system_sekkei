int this_bite = 48;
int i;

void setup(){
  Serial.begin(9600);
  i = 0; //カウンターの初期化
}

void loop(){
  if(i==0){
    Serial.write(this_bite);//ASCIIコード
    Serial.println(); //改行
    Serial.println(this_bite);//10進数
    Serial.println(this_bite,HEX); //16進数
    Serial.println(BIN); //2進数
    i++; //カウントアップ

  }
}