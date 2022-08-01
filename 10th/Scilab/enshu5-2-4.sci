// <演習5.2>-4 データの表示
X = csvRead("datae523.csv");	// データを読み込む
T = X(:,1)*0.04;		// サンプリング周期 0.04 [sec]
Y = X(:,2);			// モータ回転数 [rpm]
U = X(:,3);			// duty比 [%]
figure(1)
clf
plot(T,Y,'r.-');		// 赤いドットと線で
xs2eps(1,"fige5241.eps");	// 図を eps ファイルとして保存
figure(2)
clf
plot(T,U,'b-');			// 青い線で
a=get("current_axes");
a.data_bounds = [0,0;40,50];	// axis を設定
xs2eps(2,"fige5242.eps");	// 図を eps ファイルとして保存

