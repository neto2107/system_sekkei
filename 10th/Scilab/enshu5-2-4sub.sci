// <演習5.2>-4 データの表示
X = csvRead("data/kadai5-2.csv");	// データを読み込む
T = X(:,1)*0.04;		// サンプリング周期 0.04 [sec]
X1 = X(:,2);			// x2
X2 = X(:,3);			// x3
X3 = X(:,4);
X4 = X(:,5);

figure(2)
plot(T,X1,'r-');		// 赤いドットと線で
//xs2eps(1,"fige5241.eps");	// 図を eps ファイルとして保存
//figure(2)
plot(T,X2,'b-');			// 青い線で

plot(T,X3,'g-');            //緑の線で

plot(T,X4,'y-');            //黄色の線で
a=get("current_axes");
//a.data_bounds = [0,0;40,50];	// axis を設定
xs2eps(2,"fige5242.eps");	// 図を eps ファイルとして保存

