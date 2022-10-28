void linetrace_bang_bang()
{
  static float lightMin = 0 ; // 各自で設定
  static float lightMax = 0; // 各自で設定 （わざとエラーが出るようにしてある）
  static float speed = 100; // パラメーター
  static float Kp = 2.0; // パラメーター
  float lightNow;
  float speedDiff;

  lightNow = (red_G + green_G + blue_G ) / 3.0;
  if ( lightNow < (lightMin + lightMax) / 2.0 ) // 右回転
    speedDiff = -Kp * speed;
  else // 左回転
    speedDiff = Kp * speed;
  motorL_G = speed - speedDiff;
  motorR_G = speed + speedDiff;
}


void linetrace_P()
{
  static float lightMin = 0 ; // 各自で設定
  static float lightMax = 255; // 各自で設定 （わざとエラーが出るようにしてある）
  static float speed = 100; // パラメーター
  static float Kp = 0; // パラメーター
  float lightNow;//現在のカラーセンサーのグレースケール値
  float speedDiff;//補正用の速度の偏差
  //カラーセンサーの値をグレースケールに変換
  lightNow = (red_G + green_G + blue_G ) / 3.0;
    //現在のグレースケール-明るさの平均値を計算することでKp(どれだけ回転させるかを決めるパラメータ)を算出
  //平均より小さい→暗い（ライン上にある)→負の値
  //平均より大きい→明るい(ライン上にない。)→正の値
  Kp = lightNow - ((lightMin + lightMax) / 2.0)-20;

  //回転方向によって(motor_mode_Gによって)分岐させる
  if(motor_mode_G ==0){
    speedDiff = Kp/70 *(float)speed;//反時計回り
  }else{
    speedDiff = -Kp/70 *(float)speed;//時計回り(speedDiffの正負を判定させる。)
  }
    //モーターの回転速度をセット
  motorL_G = speed - speedDiff;
  motorR_G = speed + speedDiff;
}

//その場で回転させる
void linestate_P(){
    static float lightMin = 0 ; // 各自で設定
  static float lightMax = 255; // 各自で設定 （わざとエラーが出るようにしてある）
  static float speed = 150; // パラメーター
  static float Kp = 0; // パラメーター
  float lightNow;//現在のカラーセンサーのグレースケール値
  float speedDiff;//補正用の速度の偏差
  //カラーセンサーの値をグレースケールに変換
  lightNow = (red_G + green_G + blue_G ) / 3.0;
  //現在のグレースケール-明るさの平均値を計算することでKp(どれだけ回転させるかを決めるパラメータ)を算出
  //平均より小さい→暗い（ライン上にある)→負の値
  //平均より大きい→明るい(ライン上にない。)→正の値
  Kp = lightNow - ((lightMin + lightMax) / 2.0)-20;

  //回転方向によって(motor_mode_Gによって)分岐させる
  if(motor_mode_G ==0){
    speedDiff = Kp/70 *(float)speed;//反時計回り
  }else{
    speedDiff = -Kp/70 *(float)speed;//時計回り
  }
  //モーターの回転速度をセット
  motorL_G = - speedDiff;//speedDiffのみにすることで、その場で回転(旋回)するようにする
  motorR_G =  + speedDiff;
}

void task_A()
{
  static unsigned long startTime = 0; // static変数，時間計測ははunsigned long

  // この変数は1秒停止の開始時間を覚えておくために使用
  switch ( mode_G ) {
    case 0:
      mode_G = 1;//状態1に遷移
      break;  // break文を忘れない（忘れるとその下も実行される）

    case 1:
      linetrace_P(); // ライントレース（各自で作成）
      if ( identify_color( 220, 230, 130 ) ) { // 赤かどうか　（引数の値は各自で設定）
        mode_G = 2;//赤を検出したら状態2へ遷移
      }
      break;

    // 各自で作成
    case 2: //赤を検出した(奇数回目)ら反対方向に変更
      motor_mode_G = 1; //回転方向を時計回りに変更
      mode_G = 3;//状態3に遷移させる

      break;
    
    case 3://反対側を向くまでその場で旋回
    //青になるまで判定
      linestate_P();//その場で回転させる(linetraceではない)
      if(identify_color(170,230,200)){//青かどうか
        mode_G = 4;//青を検出したら(反対方向を向いたら)状態4に遷移
      }
      break;
    
    case 4://赤を検出するまでライントレース
      linetrace_P(); // ライントレース（各自で作成）
      if ( identify_color( 220, 230, 130 ) ) { // 赤かどうか　（引数の値は各自で設定）
        mode_G = 5; //赤を検出したら状態5へ遷移
      }
      break;
    
    case 5://回転方向を変更する

      motor_mode_G = 0; //時計回りに変更
      mode_G = 6;//状態6へ遷移
      break;

    case 6: //反対方向に回転させる
      linestate_P();//その場で回転させる
      if(identify_color(170,230,200)){//青を検出したら(反対方向を向いたら)
        mode_G = 1;//初期状態に戻る
      }
      break;
      
  }
}


int identify_color( int red, int green, int blue )
{
  float d2;
  float d2_max = 53; // パラメーター（適宜調整）

  d2 = pow(red - red_G, 2) + pow(green - green_G, 2) + pow(blue - blue_G, 2);
  if ( d2 < d2_max * d2_max )
    return 1;
  else
    return 0;
}

int maintainState( unsigned long period )
{
  static int flagStart = 0; // 0:待ち状態，1:現在計測中
  static unsigned long startTime = 0;

  if ( flagStart == 0 ) {
    startTime = timeNow_G; // 計測を開始したtimeNow_Gの値を覚えておく
    flagStart = 1; // 現在計測中にしておく
  }

  if ( timeNow_G - startTime > period ) { // 計測開始からの経過時間が指定時間を越えた
    flagStart = 0; // 待ち状態に戻しておく
    startTime = 0; // なくても良いが，形式的に初期化
    return 1;
  }
  else
    return 0;
}
