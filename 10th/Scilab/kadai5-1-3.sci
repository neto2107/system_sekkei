// <課題5.1>-3 ゲイン・時定数の読み取り
// <課題5.1>-2 データの表示を兼ねる
X = csvRead("data/kadai6-1.csv");	// データを読み込む. ファイル名は適宜変更せよ.
T = X(:,1)*0.04;		// サンプリング周期 0.04 [sec]
Y = X(:,2);			// モータ回転数 [rpm]
U = X(:,3);			// duty比 [%]
Yf= X(:,4);			// モータ回転数のフィルタ値 [rpm]
figure(1);
plot(T,Y,'b.');			// 青いドットで
plot(T,Yf,'r-');		// 赤線で
xs2eps(1,"figk5121.eps");	// 図を eps ファイルとして保存
N=size(X,1);
// <課題5.1>-2用
xmin = 0.0;	// 1周期の始まりよりちょっと前
xmax = 40.0;	// 1周期の終わりよりちょっと後
ymin = 0;
ymax = 5000;
yhigh = 0.0;	// 10.5～11.5[s] のYから
ylow  = 0.0;	// 14.5～15.75[s] のYから
T0 = 0;	// Uのグラフから
T1 = 0;	// y1=ylow+(1-e^{-1})(yhigh-ylow)とYの交点から
// ここまで
// <課題5.1>-3 ---ここから---
// つぎのコメントを外して, 読み取った値を代入せよ.
/*-------------
xmin = 2.0;	// 1周期の始まりよりちょっと前
xmax = 11.0;	// 1周期の終わりよりちょっと後
ymin = 1800;
ymax = 3400;
yhigh = 3190.0;	// 4～7[s] のYから
ylow  = 1940.0;	// 8.5～11[s] のYから
T0 = 2.84;	// Uのグラフから
T1 = 3.36;	// y1=ylow+(1-e^{-1})(yhigh-ylow)とYの交点から
-------------*/
plot([xmin,xmax],[yhigh,yhigh],'m-');
plot([xmin,xmax],[ylow,ylow],'m-');
plot([T0,T0],[ymin,ymax],'m-');
dy = yhigh-ylow;
y1 = ylow + 0.632*dy;
plot([xmin,xmax],[y1,y1],'m-');	// y1
plot([T1,T1],[ymin,ymax],'m-');
K=(yhigh-ylow)/0.05;
Ts=T1-T0;
uave = U'*ones(N,1)/N;
yave = Y'*ones(N,1)/N;
U1 = U-ones(N,1)*uave;
// <課題5.1>-4 ---ここから---
s=poly(0,'s');
Gs = syslin('c',K,1+Ts*s);	// 伝達関数の定義
Yhat1 = csim(U1'/100,T',Gs);	// 入力 U1 に対する応答の計算
Yhat=Yhat1'+ones(N,1)*yave;	// オフセット処理
plot(T,Yhat,'g:');
// <課題5.1>-4 ---ここまで---
a=get("current_axes");
a.data_bounds = [xmin,ymin;xmax,ymax];
xs2eps(1,"figk5131.eps");
// <課題5.1>-3---ここまで----
figure(2);
plot(T,U,'b-');			// 青い直線で
a=get("current_axes");
a.data_bounds = [0,0;40,50];	// axis を設定
xs2eps(2,"figk5122.eps");	// 図を eps ファイルとして保存
