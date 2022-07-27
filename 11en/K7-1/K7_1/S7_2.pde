/* For use with the colorview Arduino example sketch 
   Update the Serial() new call to match your serial port
   e.g. COM4, /dev/usbserial, etc!
*/

import processing.serial.*;
import java.awt.datatransfer.*;
import java.awt.Toolkit;
import grafica.*;

Serial port;

GPlot plot; //グラフ全体
 GPointsArray data,gData;
 
void setup(){
 size(640,480);
 // remember to replace COM20 with the appropriate serial port on your computer ex Serial.list()[1]
 port = new Serial(this, "COM5", 9600); 
 port.clear();
 
 plot = new GPlot(this,0,0,640,480);
 
 //GPointsArrayを生成し、データ(GPoint)を追加
  data = new GPointsArray();
  gData = new GPointsArray();
 
 //GPlotにGPointsArrayを関連付ける
 plot.addPoints(data);
 plot.addPoints(gData);
}
 
String buff = "";

int wRed, wGreen, wBlue, wClear;
String hexColor = "ffffff";
 

void draw(){
 //background(wRed,wGreen,wBlue);
 // check for serial, and process
 //グラフ描画開始
 plot.beginDraw();
 
 //Box,Point XY軸を描画
 plot.drawBox();
 plot.drawPoints();
 plot.drawXAxis();
 plot.drawYAxis();
 
 //描画終了
 plot.endDraw();
 
 while (port.available() > 0) {
   serialEvent(port.read());
 }
}
 
void serialEvent(int serial) {
 if(serial != '\n') {
   buff += char(serial);
 } else {
   int cRed = buff.indexOf("R");
   int cGreen = buff.indexOf("G");
   int cBlue = buff.indexOf("B");
   int clear = buff.indexOf("C");
      
   if(clear >=0){
     String val = buff.substring(clear+3);
     val = val.split("\t")[0];
     wClear = Integer.parseInt(val.trim());
   } else { return; }
   
   if(cRed >=0){
     String val = buff.substring(cRed+3);
     val = val.split("\t")[0]; 
     wRed = Integer.parseInt(val.trim());
     plot.setPointColor(color(255,0,0));
     data.add(wClear,wRed);
   } else { return; }
   
   if(cGreen >=0) {
     String val = buff.substring(cGreen+3);
     val = val.split("\t")[0]; 
     wGreen = Integer.parseInt(val.trim());
     plot.setPointColor(color(0,255,0));
     data.add(wClear,wGreen);
   } else { return; }
   
   if(cBlue >=0) {
     String val = buff.substring(cBlue+3);
     val = val.split("\t")[0]; 
     wBlue = Integer.parseInt(val.trim());
     plot.setPointColor(color(0,0,255));
     data.add(wClear,wBlue);
   } else { return; }
   
   //wRed *= 255; wRed /= wClear;
   //wGreen *= 255; wGreen /= wClear; 
   //wBlue *= 255; wBlue /= wClear; 

   print("Red: "); print(wRed);
   print("\tGrn: "); print(wGreen);
   print("\tBlue: "); print(wBlue);
   print("\tClr: "); println(wClear);



   plot.addPoints(data);

   //hexColor = hex(color(wRed, wGreen, wBlue), 6);
   //print("\t HEX: ");
   //print(hexColor);
   println();
   buff = "";
 }
}
