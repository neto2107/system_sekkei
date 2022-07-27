/*
* 学籍番号:6120011749
* 氏名:奥井翔麻
*/
abstract class Sensors{ //センサーの抽象クラス
  //コンストラクタ
  protected float v0; //ベース電圧
  protected int val; //センサーから取得した生の値
  private float v; //センサー値を電圧に変換した値
  
  public Sensors(int val){
    this.v0 = 5.0; //ベース電圧を5vに設定
    this.val = val;//初期値を指定
    this.v = 0;
  }
  //ベース電圧を設定できる様にする
  public Sensors(int val,int v0){
    this.v0 = v0; //ベース電圧を5vに設定
    this.val = val;//初期値を指定
    this.v = 0;
  }
  public void setVal(int val){ //センサー値をセットするメソッド
    this.val = val;
  }
  //AD変換値をそのまま取得する
  public int getVal(){
    return this.val;
  }
  //電圧値を取得する
  public float getVoltage(){
    this.v = v0 * (val/1024.0);
    return v;
  }
  
}
//温度計を扱うクラス
public class Thermometer extends Sensors{
  private float temp;//温度
  Thermometer(int val){
    super(val);
    temp = 0;
  }
  //温度を取得する
  public float getTemp(){
    //電圧値を温度に変換する
    temp = (this.getVoltage() * 1000.0 -600.0) /10.0;
    return temp;
  }
}
//照度計を扱うクラス
public class Illuminance extends Sensors{
  //private float lux;//照度
  Illuminance(int val){
    super(val);
    //lux = 0;
  }
  //照度を求めるのは現状ではやる方がわからなかったので保留
}
