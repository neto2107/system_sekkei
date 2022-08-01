// <演習5.2>-4 データの表示
X = csvRead("data/kadai6-1.csv");	// データを読み込む
T = X(:,1)*0.04;		// サンプリング周期 0.04 [sec]
Y = X(:,2);			// モータ回転数 [rpm]
U = X(:,3);			// duty比 [%]
Z = X(:,4);         //目標回転数
figure(1)
clf
plot(T,Y,'r.-');		// 赤いドットと線で


plot(T,Z,'b-');			// 青い線で
a=gca();
a.data_bounds(:,2)=[1500,3500]; //Y軸の値を0-100にする
xs2eps(1,"fige5241.eps");	// 図を eps ファイルとして保存

figure(2);
clf
plot(T,U,'g-');
a=gca();
a.data_bounds(:,2)=[1500,3500]; //Y軸の値を0-100にする
//a=get("current_axes");
//a.data_bounds = [0,0;40,50];	// axis を設定
xs2eps(2,"fige5242.eps");	// 図を eps ファイルとして保存

