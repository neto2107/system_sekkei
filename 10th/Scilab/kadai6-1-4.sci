// <演習5.2>-4 データの表示
X = csvRead("data/kadai6-1-4/K_p_nomal3.csv");	// データを読み込む
Y = csvRead("data/kadai6-1-4/K_i_double2.csv"); //K_pを2倍
Z = csvRead("data/kadai6-1-4/K_i_harf2.csv"); //K_pを2倍


T = X(:,1)*0.04;		// サンプリング周期 0.04 [sec]
Gx = X(:,2);			// モータ回転数 [rpm]
Gy = Y(:,2);
Gz = Z(:,2);

Hx = X(:,4);
Hy = Y(:,4);
Hz = Z(:,4);

figure(1)
clf
plot(T,Hx,'black-');
plot(T,Gx,'r-');		// 赤いドットと線で
plot(T,Gy,'b-');
plot(T,Gz,'g-');

a=gca();
a.data_bounds(:,2)=[1500,3500]; //Y軸の値を0-100にする
a.data_bounds(:,1)=[10,20];
xs2eps(1,"fige5241.eps");	// 図を eps ファイルとして保存

figure(2);
clf
plot(T,Hx,'r-');
plot(T,Hy,'b-');
plot(T,Hz,'g-');
a=gca();
a.data_bounds(:,2)=[1500,3500]; //Y軸の値を0-100にする

//a=get("current_axes");
//a.data_bounds = [0,0;40,50];	// axis を設定
xs2eps(2,"fige5242.eps");	// 図を eps ファイルとして保存
